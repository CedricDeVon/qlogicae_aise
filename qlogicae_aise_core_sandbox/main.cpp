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

    handle = GetStdHandle(STD_OUTPUT_HANDLE);

    CONSOLE_CURSOR_INFO info;
    GetConsoleCursorInfo(handle, &info);

    info.bVisible = FALSE;
    SetConsoleCursorInfo(handle, &info);

    indicators::ProgressSpinner spinner{
      option::PostfixText{"Checking credentials"},
      option::ForegroundColor{Color::yellow},
      option::SpinnerStates{std::vector<std::string>{"⠈", "⠐", "⠠", "⢀", "⡀", "⠄", "⠂", "⠁"}},
      option::FontStyles{std::vector<FontStyle>{FontStyle::bold}}
    };

    auto job = [&spinner]() {
        while (true) {
            if (spinner.is_completed()) {
                spinner.set_option(option::ForegroundColor{ Color::green });
                spinner.set_option(option::PrefixText{ "✔" });
                spinner.set_option(option::ShowSpinner{ false });
                spinner.set_option(option::ShowPercentage{ false });
                spinner.set_option(option::PostfixText{ "Authenticated!" });
                spinner.mark_as_completed();

                HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);

                CONSOLE_CURSOR_INFO info;
                GetConsoleCursorInfo(handle, &info);

                info.bVisible = TRUE;
                SetConsoleCursorInfo(handle, &info);

                break;
            }
            else
                spinner.tick();
            std::this_thread::sleep_for(std::chrono::milliseconds(160));
        }
        };
    std::thread thread(job);
    thread.join();

    return 0;
}
