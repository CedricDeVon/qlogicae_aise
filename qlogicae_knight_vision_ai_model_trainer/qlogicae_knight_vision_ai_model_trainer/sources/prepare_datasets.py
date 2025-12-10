import os
import csv
import json
import uuid
import shutil
import random
import base64
import secrets

from . import datasets
from . import utilities


def random_password():
    chars = "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789!@#$%^&*"
    length = random.randint(8, 20)
    return ''.join(secrets.choice(chars) for _ in range(length))


def random_uuid():
    return str(uuid.uuid4())


def random_hex256():
    return secrets.token_hex(32)


def random_base64_key():
    return base64.b64encode(secrets.token_bytes(32)).decode()


def random_entropy():
    return ''.join(secrets.choice("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/") for _ in range(44))


def secret_pool():
    return [
        random_password(),
        random_uuid(),
        random_hex256(),
        random_base64_key(),
        random_entropy()
    ]


def random_identifier():
    base = random.choice(datasets.base_values)
    transform = random.choice(datasets.base_forms)

    return transform(base)


def safe_format(template, varname, secret):
    n = template.count("{}")
    if n == 1:
        return template.format(secret)

    if n == 2:
        return template.format(varname, secret)

    if n == 3:
        return template.format(varname, varname, secret)

    return template


def generate_positive_sample():
    secret = random.choice(secret_pool())
    template = random.choice(datasets.positive_templates)
    varname = random.choice(datasets.positive_varnames)
    filled = safe_format(template, varname, secret)

    return filled, 1


def generate_negative_sample():
    template = random.choice(datasets.negative_templates)
    var = random_identifier()
    token = random.choice(datasets.low_entropy_strings)
    n = template.count("{}")
    if n == 1:
        return template.format(var), 0

    if n == 2:
        return template.format(var, token), 0

    return template.format(*([var] + [token]*(n-1))), 0


def generate_split(num_samples, pos_ratio):
    rows = []
    for _ in range(num_samples):
        if random.random() < pos_ratio:
            rows.append(generate_positive_sample())

        else:
            rows.append(generate_negative_sample())

    return rows


def execute():
    print("> Preparing Dataset - Starts")

    if not utilities.IS_PREPARING_DATASET_ENABLED:
        print("> Preparing Dataset - Disabled. Skipping Operation")
        print("")
        return

    utilities.log_to_console(f"> Preparing Dataset - Clearing '{utilities.RELATIVE_ROOT_DATASET_FOLDER_PATH}' folder path - Starts")
    utilities.prepare_folder_recursively(utilities.RELATIVE_ROOT_DATASET_FOLDER_PATH)
    utilities.log_to_console(f"> Preparing Dataset - Clearing '{utilities.RELATIVE_ROOT_DATASET_FOLDER_PATH}' folder path - Complete")

    train_count = int(utilities.NUMBER_OF_SAMPLES * utilities.TRAINING_TESTING_RATIO)
    test_count = utilities.NUMBER_OF_SAMPLES - train_count

    train_rows = generate_split(train_count, utilities.TRAINING_POSITION_RATIO)
    test_rows = generate_split(test_count, utilities.TESTING_POSITION_RATIO)        

    utilities.log_to_console("> Preparing Dataset - File Output - Starts")
    utilities.write_csv(train_rows, utilities.FULL_TRAINING_FILE_PATH)
    utilities.write_csv(test_rows, utilities.FULL_TESTING_FILE_PATH)
    with open(utilities.FULL_VOCABULARY_FILE_PATH, "w", encoding="utf-8") as f:
        json.dump(json.loads(datasets.vocabulary_json), f, indent=2, ensure_ascii=False)
    utilities.log_to_console("> Preparing Dataset - File Output - Complete")
    
    training_sample_count = len(train_rows)
    testing_sample_count = len(test_rows)

    training_sample_ratio = utilities.TRAINING_TESTING_RATIO
    testing_sample_ratio = 1 - utilities.TRAINING_TESTING_RATIO

    training_sample_percentage = training_sample_ratio * 100
    testing_sample_percentage = testing_sample_ratio * 100

    training_positive_label_ratio = utilities.TRAINING_POSITION_RATIO
    training_negative_label_ratio = 1 - utilities.TRAINING_POSITION_RATIO
    testing_positive_label_ratio = utilities.TESTING_POSITION_RATIO
    testing_negative_label_ratio = 1 - utilities.TESTING_POSITION_RATIO

    training_positive_label_ratio_percentage = training_positive_label_ratio * 100
    training_negative_label_ratio_percentage = training_negative_label_ratio * 100
    testing_positive_label_ratio_percentage = testing_positive_label_ratio * 100
    testing_negative_label_ratio_percentage = testing_negative_label_ratio * 100

    print("")
    print("Summary:")
    print(f"- Total Samples: {utilities.NUMBER_OF_SAMPLES}")
    print("")
    print("Training:")
    print(f"- Output File: {utilities.FULL_TRAINING_FILE_PATH}")
    print(f"- Sample Ratio: {training_sample_count} / {utilities.NUMBER_OF_SAMPLES} | {training_sample_ratio} | {training_sample_percentage}%")
    print(f"- Positive Label Ratio: {training_positive_label_ratio} | {training_positive_label_ratio_percentage}%")
    print(f"- Negative Label Ratio: {training_negative_label_ratio} | {training_negative_label_ratio_percentage}%")
    print("")
    print("Testing:")
    print(f"- Output File: {utilities.FULL_TESTING_FILE_PATH}")
    print(f"- Sample Ratio: {testing_sample_count} / {utilities.NUMBER_OF_SAMPLES} | {testing_sample_ratio} | {testing_sample_percentage}%")
    print(f"- Positive Label Ratio: {testing_positive_label_ratio} | {testing_positive_label_ratio_percentage}%")
    print(f"- Negative Label Ratio: {testing_negative_label_ratio} | {testing_negative_label_ratio_percentage}%")
    print("")
    print("> Preparing Dataset - Complete")
    print("\n")
