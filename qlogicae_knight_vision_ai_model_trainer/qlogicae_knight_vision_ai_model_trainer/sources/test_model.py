import json
import csv
import numpy as np
from tqdm import tqdm
import onnxruntime as ort


from . import utilities


class PasswordExposureONNX:
    def __init__(self, onnx_path, vocab_path, vocab_size):
        with open(vocab_path, "r", encoding="utf-8") as f:
            raw = json.load(f)

        if isinstance(raw, dict):
            self.char_to_idx = {str(k): int(v) for k, v in raw.items()}
        else:
            self.char_to_idx = {ch: i for i, ch in enumerate(raw)}

        self.vocab_size = vocab_size

        self.session = ort.InferenceSession(onnx_path, providers=["CPUExecutionProvider"])
        self.input_name = self.session.get_inputs()[0].name
        self.output_name = self.session.get_outputs()[0].name

    def encode_boc(self, text):

        vec = np.zeros(self.vocab_size, dtype=np.float32)

        unk_idx = self.char_to_idx.get("<UNK>")
        if unk_idx is None:
            unk_idx = self.char_to_idx.get("<unk>")

        for ch in text:
            idx = self.char_to_idx.get(ch)
            if idx is not None:
                vec[idx] += 1.0
            elif unk_idx is not None:
                vec[unk_idx] += 1.0

        norm = np.linalg.norm(vec)
        if norm > 0:
            vec /= norm

        return vec

    def predict_probabilities(self, texts):

        if isinstance(texts, str):
            texts = [texts]

        encoded = np.stack([self.encode_boc(t) for t in texts], axis=0)

        outputs = self.session.run(
            [self.output_name],
            {self.input_name: encoded}
        )

        probs = outputs[0].flatten()
        return probs

    def predict_labels(self, texts, threshold=0.5):
        probs = self.predict_probabilities(texts)
        labels = (probs >= threshold).astype(int)

        return labels, probs
    

def execute():    
    print("> Testing Model - Starts")

    if not utilities.IS_TESTING_MODEL_ENABLED:
        print("> Testing Model - Disabled. Skipping Operation")
        print("")
        return

    model = PasswordExposureONNX(
        onnx_path=utilities.FULL_MODELS_ONNX_PATH,
        vocab_path=utilities.FULL_VOCABULARY_FILE_PATH,
        vocab_size=utilities.EXPECTED_VOCABULARY_SIZE
    )

    examples = []
    true_labels = []

    with open(utilities.FULL_TESTING_FILE_PATH, "r", encoding="utf-8") as f:
        reader = csv.DictReader(f)
        for row in reader:
            examples.append(row["file_content"])
            true_labels.append(int(row["label"]))

    labels, probs = model.predict_labels(examples)

    correct_sample_predictions = 0
    total_samples = len(true_labels)

    print("")
    print("> Wrong Predictions: ")
    for text, true_lab, pred_lab, pr in tqdm(
        list(zip(examples, true_labels, labels, probs)),
        total=total_samples,
        desc="Progress"
    ):
        if true_lab == pred_lab:
            correct_sample_predictions += 1

        if (true_lab != pred_lab):
            print("Text:", text)
            print("Actual Label:", true_lab)
            print("Predicted Label:", pred_lab)
            print("Probability:", pr)
            print("")

    wrong_sample_predictions = total_samples - correct_sample_predictions
    prediction_accuracy = correct_sample_predictions / total_samples if total_samples > 0 else 0

    correct_sample_predictions_percentage = correct_sample_predictions / total_samples * 100
    wrong_sample_predictions_percentage = wrong_sample_predictions / total_samples * 100
    prediction_accuracy_percentage = prediction_accuracy * 100

    print(f"Total Samples: {total_samples}")
    print(f"Correct Sample Predictions: {correct_sample_predictions} / {total_samples} | {correct_sample_predictions_percentage}%")
    print(f"Wrong Sample Predictions: {wrong_sample_predictions} / {total_samples} | {wrong_sample_predictions_percentage}%")
    print(f"Prediction Accuracy: {prediction_accuracy} | {prediction_accuracy_percentage}%")
    print("")
    print("> Testing Model - Complete")
    print("\n")


