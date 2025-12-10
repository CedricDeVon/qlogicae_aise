#include "pch.hpp"

#include "main.hpp"

void bubble_sort(std::vector<int> data)
{
    bool is_swapped = true;
    size_t index, size = data.size();
    while (is_swapped)
    {
        is_swapped = false;
        for (index = 1; index < size; ++index)
        {
            if (data[index - 1] > data[index])
            {
                std::swap(data[index - 1], data[index]);
                is_swapped = true;
            }
        }
        --size;
    }
}

void selection_sort(std::vector<int> data)
{
    size_t index_a, index_b, minimum_index, size = data.size();
    for (index_a = 0; index_a < size; ++index_a)
    {
        minimum_index = index_a;
        for (index_b = index_a + 1; index_b < size; ++index_b)
        {
            if (data[index_b] < data[minimum_index])
            {
                minimum_index = index_b;
            }
        }
        std::swap(data[index_a], data[minimum_index]);
    }
}

int main(int argc, char** argv)
{   
    QLogicaeCore::CLI_IO.print_with_new_line("QLogicaeAiseCoreBenchmark - Confirmed!");

    std::mt19937 rng(42);
    ankerl::nanobench::Bench bench;
    std::vector<int> base_data(1000);
    std::uniform_int_distribution<int> dist(0, 10000);

    std::generate(base_data.begin(), base_data.end(), [&] { return dist(rng); });

    bench.minEpochIterations(10); 

    bench.run("Bubble Sort", [&]
        {
            auto data = base_data;
            bubble_sort(data);
            ankerl::nanobench::doNotOptimizeAway(data);
        }
    );

    bench.run("Selection Sort", [&]
        {
            auto data = base_data;
            selection_sort(data);
            ankerl::nanobench::doNotOptimizeAway(data);
        }
    );

    bool exit_code;
    std::cin >> exit_code;

    return 0;
}

namespace QLogicaeAiseCoreBenchmark
{

}
