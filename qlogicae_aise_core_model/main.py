from qlogicae_aise_core_model.sources import utilities
from qlogicae_aise_core_model.sources import prepare_datasets
from qlogicae_aise_core_model.sources import train_model
from qlogicae_aise_core_model.sources import test_model

if __name__ == "__main__":    
    utilities.clear_all_console_outputs()
    
    prepare_datasets.execute()
    train_model.execute()
    test_model.execute()
