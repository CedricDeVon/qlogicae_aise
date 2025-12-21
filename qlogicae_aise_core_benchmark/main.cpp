#include "pch.hpp"

#include "main.hpp"

int main(int, char**)
{   
    QLogicaeCore::Result<void>
        void_result;
    
    QLogicaeCore::Result<bool>
        boolean_result;
    
    QLogicaeCore::Result<double>
        double_result;
    
    double
        prediction_result = 0.0;

    size_t
        index_1,
        index_2,
        index_3,
        index_4,
        vocabulary_file_size = 97;
    
    std::string
        onnx_model_file_path = "model.onnx";
    
    std::string
        vocabulary_file_path = "vocabulary.json";
    
    std::string
        predict_text = "std::string password = \"SecretPassword_1234\"";


    QLogicaeAiseCore::ENCODING_MANAGER.setup(
        void_result
    );

    QLogicaeAiseCore::ENCODING_MANAGER.load_vocabulary_file_content(
        boolean_result,
        vocabulary_file_path,
        vocabulary_file_size
    );
        
    QLogicaeAiseCore::NeuralNetworkModel& neural_network_model =
        QLogicaeAiseCore::NeuralNetworkModel::get_instance();

    neural_network_model.setup(
        onnx_model_file_path
    );

    ankerl::nanobench::Bench
        bench_1;
    bench_1
        .minEpochIterations(1'000);    
    bench_1.run("QLogicaeAiseCore::NeuralNetworkModel::predict() | 1", [&]()
        {
            prediction_result = neural_network_model.predict(
                predict_text
            );
        }
    );


    ankerl::nanobench::Bench
        bench_2;
    bench_2
        .minEpochIterations(1'000);
    bench_2.run("QLogicaeAiseCore::NeuralNetworkModel::predict() | 10", [&]()
        {
            for (index_1 = 0; index_1 < 10; ++index_1)
            {
                prediction_result = neural_network_model.predict(
                    predict_text
                );
            }
        }
    );



    ankerl::nanobench::Bench
        bench_3;
    bench_3
        .minEpochIterations(1'000);
    bench_3.run("QLogicaeAiseCore::NeuralNetworkModel::predict() | 100", [&]()
        {
            for (index_2 = 0; index_2 < 100; ++index_2)
            {
                prediction_result = neural_network_model.predict(
                    predict_text
                );
            }
        }
    );



    ankerl::nanobench::Bench
        bench_4;
    bench_4
        .minEpochIterations(100);
    bench_4.run("QLogicaeAiseCore::NeuralNetworkModel::predict() | 1'000", [&]()
        {
            for (index_3 = 0; index_3 < 1'000; ++index_3)
            {
                prediction_result = neural_network_model.predict(
                    predict_text
                );
            }
        }
    );



    ankerl::nanobench::Bench
        bench_5;
    bench_5
        .minEpochIterations(1);
    bench_5.run("QLogicaeAiseCore::NeuralNetworkModel::predict() | 1'000'000", [&]()
        {
            for (index_4 = 0; index_4 < 1'000'000; ++index_4)
            {
                prediction_result = neural_network_model.predict(
                    predict_text
                );
            }
        }
    );

    
    return 0;
}

namespace QLogicaeAiseCoreBenchmark
{

}
