import csv
import json
import time
import numpy as np
from tqdm import tqdm
import onnxruntime as ort
from datetime import datetime

from . import utilities


class PasswordExposureONNX:
    def __init__(self, onnx_path, vocabulary_path, vocabulary_size):
        with open(vocabulary_path, "r", encoding=utilities.ENCODING_TYPE) as f:
            raw_json_data = json.load(f)

        if isinstance(raw_json_data, dict):
            self.character_to_index = {str(k): int(v) for k, v in raw_json_data.items()}
        else:
            self.character_to_index = {ch: i for i, ch in enumerate(raw_json_data)}

        self.vocabulary_size = vocabulary_size
        self.session = ort.InferenceSession(onnx_path, providers=["CPUExecutionProvider"])
        self.input_name = self.session.get_inputs()[0].name
        self.output_name = self.session.get_outputs()[0].name

    def encode_boc(self, text):
        vector = np.zeros(self.vocabulary_size, dtype=np.float32)

        unk_index = self.character_to_index.get("<UNK>")
        if unk_index is None:
            unk_index = self.character_to_index.get("<unk>")

        for character in text:
            index = self.character_to_index.get(character)
            if index is not None:
                vector[index] += 1.0
            elif unk_index is not None:
                vector[unk_index] += 1.0

        normalized_value = np.linalg.norm(vector)
        if normalized_value > 0:
            vector /= normalized_value

        return vector

    def predict_probabilities(self, texts):
        if isinstance(texts, str):
            texts = [texts]

        encoded = np.stack([self.encode_boc(t) for t in texts], axis=0)

        outputs = self.session.run(
            [self.output_name],
            {self.input_name: encoded}
        )

        probabilities = outputs[0].flatten()
        return probabilities

    def predict_labels(self, texts, threshold=0.5):
        probabilities = self.predict_probabilities(texts)
        labels = (probabilities >= threshold).astype(int)

        return labels, probabilities
    

def execute():    
    print("> Testing Model - Starts")
    timestamp_start = time.time_ns()

    if not utilities.IS_TESTING_MODEL_ENABLED:
        timestamp_end = time.time_ns()
        print("> Testing Model - Disabled. Skipping Operation")
        print('')
        print('Report')
        print(f"- Duration: {utilities.get_nanosecond_duration(timestamp_start, timestamp_end)} Seconds")
        print(f"- Timestamp Start: {utilities.get_timestamp_string(timestamp_start)}")
        print(f"- Timestamp End: {utilities.get_timestamp_string(timestamp_end)}")
        print("")
        return

    model = PasswordExposureONNX(
        onnx_path=utilities.FULL_MODELS_ONNX_PATH,
        vocabulary_path=utilities.FULL_VOCABULARY_FILE_PATH,
        vocabulary_size=utilities.EXPECTED_VOCABULARY_SIZE
    )

    examples = []
    true_labels = []

    with open(utilities.FULL_TESTING_FILE_PATH, "r", encoding=utilities.ENCODING_TYPE) as file:
        reader = csv.DictReader(file)
        for row in reader:
            examples.append(row[utilities.TEXT_PROPERTY])
            true_labels.append(int(row[utilities.PREDICTION_PROPERTY]))

    labels, probabilities = model.predict_labels(examples)

    correct_sample_predictions = 0
    total_samples = len(true_labels)

    print("")
    if (utilities.IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED):    
        print("> Wrong Predictions: ")

    for text, true_label, predicted_label, probability in tqdm(
        list(zip(examples, true_labels, labels, probabilities)),
        total=total_samples,
        desc="Progress"
    ):
        if true_label == predicted_label:
            correct_sample_predictions += 1

        if (utilities.IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED and (true_label != predicted_label)):
            print("Text:", text)
            print("Actual Label:", true_label)
            print("Predicted Label:", predicted_label)
            print("Probability:", probability)
            print("")

    wrong_sample_predictions = total_samples - correct_sample_predictions
    prediction_accuracy = correct_sample_predictions / total_samples if total_samples > 0 else 0

    correct_sample_predictions_percentage = correct_sample_predictions / total_samples * 100
    wrong_sample_predictions_percentage = wrong_sample_predictions / total_samples * 100
    prediction_accuracy_percentage = prediction_accuracy * 100

    timestamp_end = time.time_ns() # 
    print('> Testing Model - Complete')
    print('')
    print('Report')
    print(f"- Duration: {utilities.get_nanosecond_duration(timestamp_start, timestamp_end)} Seconds")
    print(f"- Timestamp Start: {utilities.get_timestamp_string(timestamp_start)}")
    print(f"- Timestamp End: {utilities.get_timestamp_string(timestamp_end)}")
    print(f"- Total Samples: {total_samples}")
    print(f"- Correct Sample Predictions: {correct_sample_predictions} / {total_samples} | {correct_sample_predictions_percentage}%")
    print(f"- Wrong Sample Predictions: {wrong_sample_predictions} / {total_samples} | {wrong_sample_predictions_percentage}%")
    print(f"- Prediction Accuracy: {prediction_accuracy} | {prediction_accuracy_percentage}%")
    print("")
    print("\n")

