import os
import csv
import json
import torch
import shutil
import random
import argparse
import numpy as np
from pathlib import Path


IS_VERBOSE_LOGGING_ENABLED = False
IS_PREPARING_DATASET_ENABLED = True
IS_TRAINING_MODEL_ENABLED = True
IS_TESTING_MODEL_ENABLED = True
IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED = False


NUMBER_OF_SAMPLES = 100000
TRAINING_TESTING_RATIO = 0.7
TRAINING_POSITION_RATIO = 0.4
TESTING_POSITION_RATIO = 0.4

RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH = "qlogicae/.qlogicae/ai"

RELATIVE_ROOT_DATASET_FOLDER_PATH = f"{RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH}/datasets"
RELATIVE_TRAINING_FILE_PATH = "training.csv"
RELATIVE_TESTING_FILE_PATH = "testing.csv"
RELATIVE_VOCABULARY_FILE_PATH = "vocabulary.json"

FULL_TRAINING_FILE_PATH = os.path.join(RELATIVE_ROOT_DATASET_FOLDER_PATH, RELATIVE_TRAINING_FILE_PATH)
FULL_TESTING_FILE_PATH = os.path.join(RELATIVE_ROOT_DATASET_FOLDER_PATH, RELATIVE_TESTING_FILE_PATH)
FULL_VOCABULARY_FILE_PATH = os.path.join(RELATIVE_ROOT_DATASET_FOLDER_PATH, RELATIVE_VOCABULARY_FILE_PATH)


RELATIVE_ROOT_MODELS_PATH = f"{RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH}/models"
RELATIVE_MODELS_PTH_PATH = "model.pth"
RELATIVE_MODELS_ONNX_PATH = "model.onnx"

FULL_MODELS_PTH_PATH = os.path.join(RELATIVE_ROOT_MODELS_PATH, RELATIVE_MODELS_PTH_PATH)
FULL_MODELS_ONNX_PATH = os.path.join(RELATIVE_ROOT_MODELS_PATH, RELATIVE_MODELS_ONNX_PATH)


RELATIVE_ROOT_LOGS_PATH = f"{RELATIVE_QLOGICAE_DOT_QLOGICAE_AI_FOLDER_PATH}/logs"
RELATIVE_ROOT_LOGS_TRAINING_PATH = "training"
RELATIVE_LOGS_TRAINING_LOG_CSV_FILE_PATH = "log.csv"
RELATIVE_LOGS_TRAINING_LOG_JSON_FILE_PATH = "log.json"
RELATIVE_LOGS_TRAINING_LOG_PNG_FILE_PATH = "log.png"

FULL_LOGS_TRAINING_LOG_FOLDER_PATH = os.path.join(RELATIVE_ROOT_LOGS_PATH, RELATIVE_ROOT_LOGS_TRAINING_PATH)
FULL_LOGS_TRAINING_LOG_CSV_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LOG_CSV_FILE_PATH)
FULL_LOGS_TRAINING_LOG_JSON_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LOG_JSON_FILE_PATH)
FULL_LOGS_TRAINING_LOG_PNG_FILE_PATH = os.path.join(FULL_LOGS_TRAINING_LOG_FOLDER_PATH, RELATIVE_LOGS_TRAINING_LOG_PNG_FILE_PATH)


MODEL_TRAINING_SEED = 42
random.seed(MODEL_TRAINING_SEED)
np.random.seed(MODEL_TRAINING_SEED)
torch.manual_seed(MODEL_TRAINING_SEED)

EXPECTED_VOCABULARY_SIZE = 97
MAXIMUM_TRAINING_EPOCH = 50
BATCH_SIZE = 128
VALUE_SPLIT = 0.2
DEVICE_TYPE = device = "cuda" if torch.cuda.is_available() else "cpu"
GRAD_CLIP_NORM = 1.0
EARLY_STOPPING_PATIENCE = 10
SCHEDULER_FACTOR = 0.5
SCHEDULER_PATIENCE = 3
MAXIMUM_LEARNING_RATE = 1e-3
MINIMUM_LEARNING_RATE = 1e-6


def parse_console_arguments():
    parser = argparse.ArgumentParser()

    parser.add_argument(
        "--IS_VERBOSE_LOGGING_ENABLED",
        type=str,
        default="False"
    )

    parser.add_argument(
        "--IS_PREPARING_DATASET_ENABLED",
        type=str,
        default="True"
    )

    parser.add_argument(
        "--IS_TRAINING_MODEL_ENABLED",
        type=str,
        default="True"
    )

    parser.add_argument(
        "--IS_TESTING_MODEL_ENABLED",
        type=str,
        default="True"
    )

    parser.add_argument(
        "--IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED",
        type=str,
        default="False"
    )

    arguments = parser.parse_args()

    return arguments


def log_to_console(text):
    if IS_VERBOSE_LOGGING_ENABLED:
        print(text)


def clear_all_console_outputs():
    os.system("cls" if os.name == "nt" else "clear")


def write_csv(rows, filename):
    with open(filename, "w", newline="", encoding="utf-8") as f:
        writer = csv.writer(f)
        writer.writerow(["file_content","label"])
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
                print(f"Failed to delete {file_path}. Reason: {e}")
    else:
        os.makedirs(path)  

