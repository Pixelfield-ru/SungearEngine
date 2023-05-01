#include <iostream>

#include "../Graphics/API/OpenGL/GLRenderer.h"
#include "Core.h"
#include "../Input/InputManager.h"

void sgWndCloseFun(GLFWwindow* wnd)
{
    std::cout << "closed =)" << std::endl;
}

void sgCoreInitFun()
{
    std::cout << "core initialized =)" << std::endl;
}

void sgFramePostRenderFun()
{
    std::cout << "rendering =)" << std::endl;
}

void sgWndIconifyFun(GLFWwindow* wnd, int iconified)
{
    std::cout << "window iconified =)" << std::endl;
}

void sgWndKeyFun(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
{
    //Core::Logging::consolePrintf(Core::Logging::MessageType::SG_INFO, "key callback =): %i, %i, %i, %i", key, scanCode, action, mods);
}

int main()
{
    sgSetCoreInitCallback(sgCoreInitFun);
    sgSetWindowCloseCallback(sgWndCloseFun);
    sgSetWindowKeyCallback(sgWndKeyFun);
    //sgSetFramePostRenderCallback(sgFramePostRenderFun);
    sgSetWindowIconifyCallback(sgWndIconifyFun);

    Core::Main::Core::start();

    return 0;
}