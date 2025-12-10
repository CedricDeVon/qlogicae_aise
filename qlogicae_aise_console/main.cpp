#include "pch.hpp"

#include "main.hpp"

int main(int argc, char** argv)
{           
    QLogicaeCore::QLOGICAE_APPLICATION.setup();

    QLogicaeCore::CLI_IO.print_with_new_line("QLogicaeAiseConsole - Confirmed!");

    bool exit_code;
    std::cin >> exit_code;

    return 0;
}

namespace QLogicaeAiseConsole
{

}
