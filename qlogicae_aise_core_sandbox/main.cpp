#include "pch.hpp"

#include "main.hpp"

using namespace indicators;

int main(int, char**)
{
    SetConsoleOutputCP(CP_UTF8);
    SetConsoleCP(CP_UTF8);

    HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
    DWORD mode = 0;
    GetConsoleMode(handle, &mode);
    mode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
    SetConsoleMode(handle, mode);

    CONSOLE_CURSOR_INFO cursor_info;
    GetConsoleCursorInfo(handle, &cursor_info);
    cursor_info.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &cursor_info);

    using namespace indicators;

    ProgressSpinner spinner{
        option::PostfixText{"Testing | 0 - 0 - 0"},
        option::ForegroundColor{Color::yellow},
        option::SpinnerStates{
            std::vector<std::string>{"⠈","⠐","⠠","⢀","⡀","⠄","⠂","⠁"}
        },
        option::FontStyles{
            std::vector<FontStyle>{FontStyle::bold}
        }
    };

    const int total_count = 100;
    std::atomic<int> success_count{ 0 };
    std::atomic<int> failed_count{ 0 };

    auto job = [&spinner, &success_count, &failed_count, total_count]()
        {
            for (int i = 1; i <= total_count; ++i)
            {
                if (i % 5 == 0)
                    failed_count++;
                else
                    success_count++;

                spinner.set_option(option::PostfixText{
                    "Testing | " + std::to_string(i) + " - " +
                    std::to_string(success_count.load()) + " - " +
                    std::to_string(failed_count.load())
                    });

                spinner.tick();
                std::this_thread::sleep_for(std::chrono::milliseconds(50));
            }

            spinner.set_option(option::ForegroundColor{ Color::green });
            spinner.set_option(option::PrefixText{ "✔" });
            spinner.set_option(option::ShowSpinner{ false });
            spinner.set_option(option::ShowPercentage{ false });
            spinner.set_option(option::PostfixText{ "Completed!" });
            spinner.mark_as_completed();

            HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);
            CONSOLE_CURSOR_INFO info;
            GetConsoleCursorInfo(handle, &info);
            info.bVisible = TRUE;
            SetConsoleCursorInfo(handle, &info);
        };

    std::thread thread(job);
    thread.join();

    return 0;
}
