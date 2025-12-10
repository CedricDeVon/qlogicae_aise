#pragma once

#include "qlogicae_aise_console/includes/application.hpp"

#include "qlogicae_aise_core/includes/all.hpp"
#include "qlogicae_core/includes/all.hpp"

int main(int, char**);

int WINAPI WinMain(
    HINSTANCE hInstance,
    HINSTANCE,
    LPSTR,
    int
)
{
    return main(__argc, __argv);
}

namespace QLogicaeAiseConsole
{

}
