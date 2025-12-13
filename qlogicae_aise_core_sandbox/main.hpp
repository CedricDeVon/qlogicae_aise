#pragma once

#include "qlogicae_aise_core_sandbox/includes/application.hpp"

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

namespace QLogicaeAiseCoreSandbox
{

}
