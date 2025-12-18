import os
import csv
import json
import torch
import shutil
import random
import argparse
import numpy as np
from pathlib import Path
import matplotlib.pyplot as plt
from collections import OrderedDict


IS_PRODUCTION_ENABLED = True
IS_VERBOSE_LOGGING_ENABLED = False
IS_PREPARING_DATASET_ENABLED = True
IS_TRAINING_MODEL_ENABLED = True
IS_TESTING_MODEL_ENABLED = True
IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED = False


NUMBER_OF_SAMPLES = 1000000 if IS_PRODUCTION_ENABLED else 100000
MAXIMUM_TRAINING_EPOCH = 20 if IS_PRODUCTION_ENABLED else 20
TRAINING_TESTING_RATIO = 0.7 if IS_PRODUCTION_ENABLED else 0.7
TRAINING_POSITION_RATIO = 0.4 if IS_PRODUCTION_ENABLED else 0.4
TESTING_POSITION_RATIO = 0.4 if IS_PRODUCTION_ENABLED else 0.4
EXPECTED_VOCABULARY_SIZE = 97 if IS_PRODUCTION_ENABLED else 97
BATCH_SIZE = 128 if IS_PRODUCTION_ENABLED else 128
VALUE_SPLIT = 0.2 if IS_PRODUCTION_ENABLED else 0.2
DEVICE_TYPE = device = 'cuda' if torch.cuda.is_available() else 'cpu'
GRAD_CLIP_NORM = 1.0 if IS_PRODUCTION_ENABLED else 1.0
EARLY_STOPPING_PATIENCE = 10 if IS_PRODUCTION_ENABLED else 10
SCHEDULER_FACTOR = 0.5 if IS_PRODUCTION_ENABLED else 0.5
SCHEDULER_PATIENCE = 3 if IS_PRODUCTION_ENABLED else 3
MAXIMUM_LEARNING_RATE = 1e-3 if IS_PRODUCTION_ENABLED else 1e-3
MINIMUM_LEARNING_RATE = 1e-6 if IS_PRODUCTION_ENABLED else 1e-6
DATA_LOADER_WORKERS = min(4, max(1, os.cpu_count() - 1)) if IS_PRODUCTION_ENABLED else min(4, max(1, os.cpu_count() - 1))
MODEL_TRAINING_SEED = 42 if IS_PRODUCTION_ENABLED else 42

random.seed(MODEL_TRAINING_SEED)
np.random.seed(MODEL_TRAINING_SEED)
torch.manual_seed(MODEL_TRAINING_SEED)

TRAINING_CSV_OUTPUT_HEADERS = ['epoch', 'training_loss', 'training_accuracy', 'value_loss', 'value_accuracy', 'learning_rate']
ENCODING_TYPE = 'utf-8'

RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH = 'qlogicae/.qlogicae/application'

RELATIVE_ROOT_DATASET_FOLDER_PATH = f'{RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH}/ai/datasets'
RELATIVE_TRAINING_FILE_PATH = 'training.csv'
RELATIVE_TESTING_FILE_PATH = 'testing.csv'

FULL_TRAINING_FILE_PATH = os.path.join(RELATIVE_ROOT_DATASET_FOLDER_PATH, RELATIVE_TRAINING_FILE_PATH)
FULL_TESTING_FILE_PATH = os.path.join(RELATIVE_ROOT_DATASET_FOLDER_PATH, RELATIVE_TESTING_FILE_PATH)


RELATIVE_ROOT_AI_PATH = f'{RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH}/templates/default/build/qlogicae/application/ai'
RELATIVE_ROOT_MODELS_PATH = f'{RELATIVE_ROOT_AI_PATH}/models'
RELATIVE_ROOT_ENCODINGS_PATH = f'{RELATIVE_ROOT_AI_PATH}/encodings'

RELATIVE_MODELS_PTH_PATH = 'model.pth'
RELATIVE_MODELS_ONNX_PATH = 'model.onnx'
RELATIVE_VOCABULARY_FILE_PATH = 'vocabulary.json'

FULL_MODELS_PTH_PATH = os.path.join(RELATIVE_ROOT_MODELS_PATH, RELATIVE_MODELS_PTH_PATH)
FULL_MODELS_ONNX_PATH = os.path.join(RELATIVE_ROOT_MODELS_PATH, RELATIVE_MODELS_ONNX_PATH)
FULL_VOCABULARY_FILE_PATH = os.path.join(RELATIVE_ROOT_ENCODINGS_PATH, RELATIVE_VOCABULARY_FILE_PATH)

RELATIVE_ROOT_LOGS_PATH = f'{RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH}/ai/logs'
RELATIVE_ROOT_LOGS_TRAINING_PATH = 'training'
RELATIVE_LOGS_TRAINING_LOG_CSV_FILE_PATH = 'logs.csv'
RELATIVE_LOGS_TRAINING_LOG_JSON_FILE_PATH = 'logs.json'
RELATIVE_LOGS_TRAINING_TRAINING_LOSS_PNG_FILE_PATH = 'training_loss.png'
RELATIVE_LOGS_TRAINING_TRAINING_ACCURACY_PNG_FILE_PATH = 'training_accuracy.png'
RELATIVE_LOGS_TRAINING_VALUE_LOSS_PNG_FILE_PATH = 'value_loss.png'
RELATIVE_LOGS_TRAINING_VALUE_ACCURACY_PNG_FILE_PATH = 'value_accuracy.png'
RELATIVE_LOGS_TRAINING_LEARNING_RATE_PNG_FILE_PATH = 'learning_rate.png'
RELATIVE_LOGS_TRAINING_TRAINING_LOSS_AND_ACCURACY_PNG_FILE_PATH = 'training_loss_and_accuracy.png'
RELATIVE_LOGS_TRAINING_VALUE_LOSS_AND_ACCURACY_PNG_FILE_PATH = 'value_loss_and_accuracy.png'
RELATIVE_LOGS_TRAINING_TRAINING_AND_VALUE_LOSS_PNG_FILE_PATH = 'training_and_value_loss.png'
RELATIVE_LOGS_TRAINING_TRAINING_AND_VALUE_ACCURACY_PNG_FILE_PATH = 'training_and_value_accuracy.png'
RELATIVE_LOGS_TRAINING_TRAINING_AND_VALUE_PNG_FILE_PATH = 'training_and_value.png'
RELATIVE_LOGS_TRAINING_LEARNING_DURATION_PNG_FILE_PATH = 'learning_duration.png'

FULL_LOGS_TRAINING_LOG_FOLDER_PATH = os.path.join(RELATIVE_ROOT_LOGS_PATH, RELATIVE_ROOT_LOGS_TRAINING_PATH)
FULL_LOGS_TRAINING_LOG_CSV_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LOG_CSV_FILE_PATH)
FULL_LOGS_TRAINING_LOG_JSON_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LOG_JSON_FILE_PATH)
FULL_LOGS_TRAINING_TRAINING_LOSS_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_TRAINING_LOSS_PNG_FILE_PATH)
FULL_LOGS_TRAINING_TRAINING_ACCURACY_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_TRAINING_ACCURACY_PNG_FILE_PATH)
FULL_LOGS_TRAINING_VALUE_LOSS_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_VALUE_LOSS_PNG_FILE_PATH)
FULL_LOGS_TRAINING_VALUE_ACCURACY_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_VALUE_ACCURACY_PNG_FILE_PATH)
FULL_LOGS_TRAINING_LEARNING_RATE_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LEARNING_RATE_PNG_FILE_PATH)
FULL_LOGS_TRAINING_TRAINING_LOSS_AND_ACCURACY_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_TRAINING_LOSS_AND_ACCURACY_PNG_FILE_PATH)
FULL_LOGS_TRAINING_VALUE_LOSS_AND_ACCURACY_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_VALUE_LOSS_AND_ACCURACY_PNG_FILE_PATH)
FULL_LOGS_TRAINING_TRAINING_AND_VALUE_LOSS_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_TRAINING_AND_VALUE_LOSS_PNG_FILE_PATH)
FULL_LOGS_TRAINING_TRAINING_AND_VALUE_ACCURACY_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_TRAINING_AND_VALUE_ACCURACY_PNG_FILE_PATH)
FULL_LOGS_TRAINING_TRAINING_AND_VALUE_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_TRAINING_AND_VALUE_PNG_FILE_PATH)
FULL_LOGS_TRAINING_LEARNING_DURATION_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LEARNING_DURATION_PNG_FILE_PATH)


def load_vocab(vocab_path: str, vocabulary_size: int):
    if not os.path.isfile(vocab_path):
        raise FileNotFoundError(f'Vocabulary file not found: {vocab_path}')

    char_to_idx = OrderedDict()

    _, ext = os.path.splitext(vocab_path)
    ext = ext.lower()

    if ext == '.json':
        with open(vocab_path, 'r', encoding=ENCODING_TYPE) as f:
            char_to_idx = json.load(f)
        char_to_idx = {str(k): int(v) for k, v in char_to_idx.items()}
    else:
        with open(vocab_path, 'r', encoding=ENCODING_TYPE) as f:
            for idx, line in enumerate(f):
                token = line.strip()
                if token and token not in char_to_idx:
                    char_to_idx[token] = idx
                if len(char_to_idx) >= vocabulary_size:
                    break

    idx_to_char = {idx: char for char, idx in char_to_idx.items()}

    return char_to_idx, idx_to_char


def save_plot_png(title, x_label, y_label, data, file_output):
    plt.figure()

    for label, series in data.items():
        plt.plot(range(1, len(series) + 1), series, label=label)

    plt.xlabel(x_label)
    plt.ylabel(y_label)
    plt.title(title)
    plt.legend()
    plt.grid(True)
    plt.savefig(file_output)
    plt.close()


def parse_console_arguments():
    parser = argparse.ArgumentParser()

    arguments = parser.parse_args()

    return arguments


def log_to_console(text):
    if IS_VERBOSE_LOGGING_ENABLED:
        print(text)


def clear_all_console_outputs():
    os.system('cls' if os.name == 'nt' else 'clear')


def write_csv(rows, filename):
    with open(filename, 'w', newline='\n', encoding=ENCODING_TYPE) as f:
        writer = csv.writer(f)
        writer.writerow(['file_content','label'])
        for c,l in rows:
            writer.writerow([c,l])


def prepare_folder_recursively(path):
    if os.path.exists(path):
        for filename in os.listdir(path):
            file_path = os.path.join(path, filename)
            try:
                if os.path.isfile(file_path) or os.path.islink(file_path):
                    os.unlink(file_path)  
                elif os.path.isdir(file_path):
                    shutil.rmtree(file_path)  
            except Exception as e:
                print(f'Failed to delete {file_path}. Reason: {e}')
    else:
        os.makedirs(path)  

