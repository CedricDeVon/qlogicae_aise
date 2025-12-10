import os
import csv
import json
import math
import time
import torch
import random
import numpy as np
import torch.nn as nn
from tqdm import tqdm
from pathlib import Path
import torch.optim as optim
import matplotlib.pyplot as plt
from torch.utils.data import Dataset, DataLoader, random_split

from . import utilities

class CodeLineDataset(Dataset):
    def __init__(self, csv_path, vocabulary_path, vocabulary_size):
        self.vocabulary_size = vocabulary_size
        self.rows = []
        with open(csv_path, "r", encoding="utf-8") as f:
            reader = csv.DictReader(f)
            for r in reader:
                text = r["file_content"]
                label = float(r["label"])
                self.rows.append((text, label))

        with open(vocabulary_path, "r", encoding="utf-8") as vf:
            raw = json.load(vf)

        if isinstance(raw, dict):
            max_index = max(raw.values())
            inferred_size = max_index + 1
            if inferred_size != vocabulary_size:
                raise ValueError(
                    f"vocab.json indicates size {inferred_size} but expected {vocabulary_size}"
                )
            self.char_to_idx = {str(k): int(v) for k, v in raw.items()}
        elif isinstance(raw, list):
            if len(raw) != vocabulary_size:
                raise ValueError(
                    f"vocab.json length {len(raw)} does not match expected {vocabulary_size}"
                )
            self.char_to_idx = {ch: i for i, ch in enumerate(raw)}
        else:
            raise ValueError("vocab.json must be either a dict or a list")

        indices = sorted(set(self.char_to_idx.values()))
        if indices != list(range(vocabulary_size)):
            raise ValueError(
                "vocab.json indices must be contiguous 0..(vocabulary_size-1)"
            )

        self.X = []
        self.y = []
        for text, label in self.rows:
            vec = self.encode_boc(text)
            self.X.append(vec)
            self.y.append(label)

        self.X = torch.stack(self.X)
        self.y = torch.tensor(self.y, dtype=torch.float32).view(-1, 1)

    def encode_boc(self, text):
        cnt = torch.zeros(self.vocabulary_size, dtype=torch.float32)
        unk_idx = self.char_to_idx.get("<UNK>") or self.char_to_idx.get("<unk>")

        for ch in text:
            if ch in self.char_to_idx:
                cnt[self.char_to_idx[ch]] += 1.0
            elif unk_idx is not None:
                cnt[unk_idx] += 1.0

        norm = cnt.norm(p=2)
        if norm > 0:
            cnt = cnt / norm
        return cnt

    def __len__(self):
        return self.X.size(0)

    def __getitem__(self, idx):
        return self.X[idx], self.y[idx]


class ExposureNet(nn.Module):
    def __init__(self, input_size):
        super().__init__()
        self.net = nn.Sequential(
            nn.Linear(input_size, 128),
            nn.ReLU(),
            nn.Linear(128, 64),
            nn.ReLU(),
            nn.Linear(64, 1),
            nn.Sigmoid()
        )

    def forward(self, x):
        return self.net(x)
    

def execute():
    print("> Training Model - Starts")    

    if not utilities.IS_TRAINING_MODEL_ENABLED:
        print("> Training Model - Disabled. Skipping Operation")
        print("")
        return

    utilities.prepare_folder_recursively(utilities.RELATIVE_ROOT_MODELS_PATH)
    utilities.prepare_folder_recursively(utilities.FULL_LOGS_TRAINING_LOG_FOLDER_PATH)

    csv_path = utilities.FULL_TRAINING_FILE_PATH
    vocabulary_path = utilities.FULL_VOCABULARY_FILE_PATH
    total_epochs = utilities.MAXIMUM_TRAINING_EPOCH
    batch_size = utilities.BATCH_SIZE
    lr = utilities.MAXIMUM_LEARNING_RATE
    value_split = utilities.VALUE_SPLIT
    device = utilities.DEVICE_TYPE
    vocabulary_size = utilities.EXPECTED_VOCABULARY_SIZE

    ds = CodeLineDataset(csv_path, vocabulary_path, vocabulary_size)
    dataset_size = len(ds)
    val_size = int(math.floor(value_split * dataset_size))
    train_size = dataset_size - val_size

    train_ds, val_ds = random_split(ds, [train_size, val_size])

    train_loader = DataLoader(train_ds, batch_size=batch_size, shuffle=True)
    val_loader = DataLoader(val_ds, batch_size=batch_size)

    model = ExposureNet(ds.vocabulary_size).to(device)
    criterion = nn.BCELoss()
    optimizer = optim.Adam(model.parameters(), lr=lr)
    scheduler = optim.lr_scheduler.ReduceLROnPlateau(
        optimizer,
        mode="min",
        factor=utilities.SCHEDULER_FACTOR,
        patience=utilities.SCHEDULER_PATIENCE,
        min_lr=utilities.MINIMUM_LEARNING_RATE
    )

    training_loss_history = []
    value_loss_history = []
    training_accuracy_history = []
    value_accuracy_history = []
    learning_rate_history = []
    learning_duration_history = []

    csv_log_path = utilities.FULL_LOGS_TRAINING_LOG_CSV_FILE_PATH
    json_log_path = utilities.FULL_LOGS_TRAINING_LOG_JSON_FILE_PATH
    model_pth_path = utilities.FULL_MODELS_PTH_PATH
    model_onnx_path = utilities.FULL_MODELS_ONNX_PATH

    csv_log = open(csv_log_path, "w", newline="", encoding="utf-8")
    csv_writer = csv.writer(csv_log)
    csv_writer.writerow(
        ["epoch", "training_loss", "training_accuracy", "value_loss", "value_accuracy", "learning_rate"]
    )

    json_history = []

    best_val_loss = float("inf")
    best_epoch = 0
    
    print("")
    for epoch in range(1, total_epochs + 1):
        epoch_start = time.time()
        model.train()
        training_loss = 0.0
        train_correct = 0
        train_count = 0

        pbar = tqdm(train_loader, desc=f"Epoch {epoch}/{total_epochs} Train", leave=False)
        for xb, yb in pbar:
            xb = xb.to(device)
            yb = yb.to(device)

            optimizer.zero_grad()
            probs = model(xb)
            loss = criterion(probs, yb)
            loss.backward()

            torch.nn.utils.clip_grad_norm_(model.parameters(), utilities.GRAD_CLIP_NORM)

            optimizer.step()

            training_loss += loss.item() * xb.size(0)
            train_correct += ((probs >= 0.5).float() == yb).sum().item()
            train_count += xb.size(0)

            pbar.set_postfix(training_loss=loss.item())

        training_loss /= max(1, train_count)
        training_accuracy = train_correct / max(1, train_count)

        model.eval()
        value_loss = 0.0
        val_correct = 0
        val_count = 0
        with torch.no_grad():
            for xb, yb in val_loader:
                xb = xb.to(device)
                yb = yb.to(device)
                probs = model(xb)
                loss = criterion(probs, yb)
                value_loss += loss.item() * xb.size(0)
                val_correct += ((probs >= 0.5).float() == yb).sum().item()
                val_count += xb.size(0)

        value_loss /= max(1, val_count)
        value_accuracy = val_correct / max(1, val_count)


        scheduler.step(value_loss)

        learning_rate = optimizer.param_groups[0]["lr"]

        training_loss_history.append(training_loss * 100)
        training_accuracy_history.append(training_accuracy * 100)
        value_loss_history.append(value_loss * 100)
        value_accuracy_history.append(value_accuracy * 100)
        learning_rate_history.append(learning_rate)

        csv_writer.writerow(
            [epoch, training_loss, training_accuracy, value_loss, value_accuracy, learning_rate]
        )
        csv_log.flush()

        json_history.append({
            "epoch": epoch,
            "training_loss": training_loss,
            "training_accuracy": training_accuracy,
            "value_loss": value_loss,
            "value_accuracy": value_accuracy,
            "learning_rate": learning_rate
        })

        epoch_time = time.time() - epoch_start
        print(
            f"Epoch {epoch}/{total_epochs} | "
            f"Training Loss = {training_loss} Training Accuracy = {training_accuracy} | "
            f"Value Loss = {value_loss} Value Accuracy = {value_accuracy} | "
            f"Learning Rate = {learning_rate} | Training Duration = {epoch_time}s"
        )

        learning_duration_history.append(epoch_time)

        best_val_loss = value_loss
        best_epoch = epoch
        torch.save(model.state_dict(), model_pth_path)

    utilities.log_to_console(f"> Training Model - Saving Log Files - Starts")
    with open(json_log_path, "w", encoding="utf-8") as jf:
        json.dump(json_history, jf, indent=2)
    csv_log.close()      
    utilities.save_plot_png(
        title='Learning Rate Over Time',
        x_label='epoch',
        y_label='rate',
        data={ 'learning_rate': learning_rate_history },
        file_output=utilities.FULL_LOGS_TRAINING_LEARNING_RATE_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Learning Duration Over Time',
        x_label='epoch',
        y_label='seconds',
        data={ 'learning_duration': learning_duration_history },
        file_output=utilities.FULL_LOGS_TRAINING_LEARNING_DURATION_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Training Loss Over Time',
        x_label='epoch',
        y_label='percentage',
        data={ 'training_loss': training_loss_history },
        file_output=utilities.FULL_LOGS_TRAINING_TRAINING_LOSS_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Training Accuracy Over Time',
        x_label='epoch',
        y_label='percentage',
        data={ 'training_accuracy': training_accuracy_history },
        file_output=utilities.FULL_LOGS_TRAINING_TRAINING_ACCURACY_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Value Loss Over Time',
        x_label='epoch',
        y_label='percentage',
        data={ 'value_loss': value_loss_history },
        file_output=utilities.FULL_LOGS_TRAINING_VALUE_LOSS_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Value Accuracy Over Time',
        x_label='epoch',
        y_label='percentage',
        data={ 'value_accuracy': value_accuracy_history },
        file_output=utilities.FULL_LOGS_TRAINING_VALUE_ACCURACY_PNG_FILE_PATH
    )    
    utilities.save_plot_png(
        title='Training Loss and Accuracy',
        x_label='epoch',
        y_label='percentage',
        data={
            'training_loss': training_loss_history,
            'training_accuracy': training_accuracy_history
        },
        file_output=utilities.FULL_LOGS_TRAINING_TRAINING_LOSS_AND_ACCURACY_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Value Loss and Accuracy',
        x_label='epoch',
        y_label='percentage',
        data={
            'value_loss': value_loss_history,
            'value_accuracy': value_accuracy_history
        },
        file_output=utilities.FULL_LOGS_TRAINING_VALUE_LOSS_AND_ACCURACY_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Training and Value Loss',
        x_label='epoch',
        y_label='percentage',
        data={
            'training_loss': training_loss_history,
            'value_loss': value_loss_history
        },
        file_output=utilities.FULL_LOGS_TRAINING_TRAINING_AND_VALUE_LOSS_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Training and Value Accuracy',
        x_label='epoch',
        y_label='percentage',
        data={
            'training_accuracy': training_accuracy_history,
            'value_accuracy': value_accuracy_history
        },
        file_output=utilities.FULL_LOGS_TRAINING_TRAINING_AND_VALUE_ACCURACY_PNG_FILE_PATH
    )
    utilities.save_plot_png(
        title='Training And Value Loss and Accuracy',
        x_label='epoch',
        y_label='percentage',
        data={
            'training_loss': training_loss_history,
            'training_accuracy': training_accuracy_history,
            'value_loss': value_loss_history,
            'value_accuracy': value_accuracy_history
        },
        file_output=utilities.FULL_LOGS_TRAINING_TRAINING_AND_VALUE_PNG_FILE_PATH
    )


    utilities.log_to_console(f"> Training Model - Saving Log Files - Complete")


    utilities.log_to_console(f"> Training Model - Saving PyTorch Model '{model_pth_path}' - Starts")
    torch.save(model.state_dict(), model_pth_path)
    utilities.log_to_console(f"> Training Model - Saving PyTorch Model '{model_pth_path}' - Complete")

    model.load_state_dict(torch.load(model_pth_path, map_location=device, weights_only=True))
    model.eval()
    dummy = torch.randn(1, utilities.EXPECTED_VOCABULARY_SIZE, device=device)

    utilities.log_to_console(f"> Training Model - Saving ONNX Model '{model_onnx_path}' - Starts")
    torch.onnx.export(
        model,
        dummy,
        model_onnx_path,
        input_names=["input"],
        output_names=["prob"],
        dynamic_axes={"input": {0: "batch_size"}, "prob": {0: "batch_size"}},
        opset_version=17,
    )
    utilities.log_to_console(f"> Training Model - Saving ONNX Model '{model_onnx_path}' - Complete")
    print("")
    print("> Training Model - Complete")
    print("\n")

    return model, ds.char_to_idx

