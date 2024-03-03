#include "Callbacks.h"

struct SGCallbacks
{
    SGCoreInitCallback c_coreInit;
    SGWindowCloseCallback c_windowClose;
    SGWindowIconifyCallback c_windowIconify;
    SGWindowKeyCallback c_keyCallback;
    SGWindowMouseButtonCallback c_mouseButtonCallback;
    SGFixedUpdateCallback c_fixedUpdate;
    SGUpdateCallback c_update;
};

SGCallbacks sgCallbacks;

void sgSetCoreInitCallback(const SGCoreInitCallback& callback) noexcept
{
    sgCallbacks.c_coreInit = callback;
}

void sgSetWindowCloseCallback(const SGWindowCloseCallback& callback) noexcept
{
    sgCallbacks.c_windowClose = callback;
}

void sgSetWindowIconifyCallback(const SGWindowIconifyCallback& callback) noexcept
{
    sgCallbacks.c_windowIconify = callback;
}

void sgSetWindowKeyCallback(const SGWindowKeyCallback& callback) noexcept
{
    sgCallbacks.c_keyCallback = callback;
}

void sgSetWindowMouseButtonCallback(const SGWindowMouseButtonCallback& callback) noexcept
{
    sgCallbacks.c_mouseButtonCallback = callback;
}

void sgSetFixedUpdateCallback(const SGFixedUpdateCallback& callback) noexcept
{
    sgCallbacks.c_fixedUpdate = callback;
}

void sgSetUpdateCallback(const SGUpdateCallback& callback) noexcept
{
    sgCallbacks.c_update = callback;
}

// ---------------------------------------

void sgCallCoreInitCallback()
{
    if(sgCallbacks.c_coreInit)
    {
        sgCallbacks.c_coreInit();
    }
}

void sgCallWindowCloseCallback(GLFWwindow* wnd)
{
    if(sgCallbacks.c_windowClose)
    {
        sgCallbacks.c_windowClose(wnd);
    }
}

void sgCallWindowIconifyCallback(GLFWwindow* wnd, int iconified)
{
    if(sgCallbacks.c_windowIconify)
    {
        sgCallbacks.c_windowIconify(wnd, iconified);
    }
}

void sgCallWindowKeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
{
    if(sgCallbacks.c_keyCallback)
    {
        sgCallbacks.c_keyCallback(wnd, key, scanCode, action, mods);
    }
}

void sgCallWindowMouseButtonCallback(GLFWwindow* wnd, int key, int scanCode, int action)
{
    if(sgCallbacks.c_mouseButtonCallback)
    {
        sgCallbacks.c_mouseButtonCallback(wnd, key, scanCode, action);
    }
}

void sgCallFixedUpdateCallback(const double& dt, const double& fixedDt)
{
    if(sgCallbacks.c_fixedUpdate)
    {
        sgCallbacks.c_fixedUpdate(dt, fixedDt);
    }
}

void sgCallUpdateCallback(const double& dt, const double& fixedDt)
{
    if(sgCallbacks.c_update)
    {
        sgCallbacks.c_update(dt, fixedDt);
    }
}