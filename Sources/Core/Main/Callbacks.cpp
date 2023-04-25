#include "Callbacks.h"

struct SGCallbacks
{
    SGCoreInitCallback coreInit;
    SGWindowCloseCallback windowClose;
    SGWindowIconifyCallback windowIconify;
    SGWindowKeyCallback keyCallback;
    SGFramePostRenderCallback frameRender;
};

inline SGCallbacks sgCallbacks;

void sgSetCoreInitCallback(const SGCoreInitCallback& callback) noexcept
{
    sgCallbacks.coreInit = callback;
}

void sgSetWindowCloseCallback(const SGWindowCloseCallback& callback) noexcept
{
    sgCallbacks.windowClose = callback;
}

void sgSetWindowIconifyCallback(const SGWindowIconifyCallback& callback) noexcept
{
    sgCallbacks.windowIconify = callback;
}

void sgSetWindowKeyCallback(const SGWindowKeyCallback& callback) noexcept
{
    sgCallbacks.keyCallback = callback;
}

void sgSetFramePostRenderCallback(const SGFramePostRenderCallback& callback) noexcept
{
    sgCallbacks.frameRender = callback;
}

// ---------------------------------------

void sgCallCoreInitCallback()
{
    if(sgCallbacks.coreInit)
    {
        sgCallbacks.coreInit();
    }
}

void sgCallWindowCloseCallback(GLFWwindow* wnd)
{
    if(sgCallbacks.windowClose)
    {
        sgCallbacks.windowClose(wnd);
    }
}

void sgCallWindowIconifyCallback(GLFWwindow* wnd, int iconified)
{
    if(sgCallbacks.windowIconify)
    {
        sgCallbacks.windowIconify(wnd, iconified);
    }
}

void sgCallWindowKeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
{
    if(sgCallbacks.keyCallback)
    {
        sgCallbacks.keyCallback(wnd, key, scanCode, action, mods);
    }
}

void sgCallFramePostRenderCallback()
{
    if(sgCallbacks.frameRender)
    {
        sgCallbacks.frameRender();
    }
}