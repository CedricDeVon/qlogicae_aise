from qlogicae_knight_vision_ai_model_trainer.sources import utilities
from qlogicae_knight_vision_ai_model_trainer.sources import prepare_datasets
from qlogicae_knight_vision_ai_model_trainer.sources import train_model
from qlogicae_knight_vision_ai_model_trainer.sources import test_model

if __name__ == "__main__":
    arguments = utilities.parse_console_arguments()

    utilities.IS_VERBOSE_LOGGING_ENABLED = arguments.IS_VERBOSE_LOGGING_ENABLED == "True"
    utilities.IS_PREPARING_DATASET_ENABLED = arguments.IS_PREPARING_DATASET_ENABLED == "True"
    utilities.IS_TRAINING_MODEL_ENABLED = arguments.IS_TRAINING_MODEL_ENABLED == "True"
    utilities.IS_TESTING_MODEL_ENABLED = arguments.IS_TESTING_MODEL_ENABLED == "True"
    utilities.IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED = arguments.IS_TESTING_MODEL_OUTPUT_VERBOSE_ENABLED == "True"

    utilities.clear_all_console_outputs()
    
    prepare_datasets.execute()
    train_model.execute()
    test_model.execute()
