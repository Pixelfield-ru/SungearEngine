#include "Callbacks.h"

struct SGCallbacks
{
    SGCoreInitCallback coreInit;
    SGWindowCloseCallback windowClose;
    SGWindowIconifyCallback windowIconify;
    SGWindowKeyCallback keyCallback;
    SGWindowMouseButtonCallback mouseButtonCallback;
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

void sgSetWindowMouseButtonCallback(const SGWindowMouseButtonCallback& callback) noexcept
{
    sgCallbacks.mouseButtonCallback = callback;
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

void sgCallWindowMouseButtonCallback(GLFWwindow* wnd, int key, int scanCode, int action)
{
    if(sgCallbacks.mouseButtonCallback)
    {
        sgCallbacks.mouseButtonCallback(wnd, key, scanCode, action);
    }
}

void sgCallFramePostRenderCallback()
{
    if(sgCallbacks.frameRender)
    {
        sgCallbacks.frameRender();
    }
}