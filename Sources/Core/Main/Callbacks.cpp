#include "Callbacks.h"

struct SGCallbacks
{
    SGCoreInitCallback core_init;
    SGWindowCloseCallback window_close;
    SGWindowIconifyCallback window_iconify;
    SGFramePostRenderCallback frame_render;
};

inline SGCallbacks sg_callbacks;

void sg_set_core_init_callback(const SGCoreInitCallback& callback) noexcept
{
    sg_callbacks.core_init = callback;
}

void sg_set_window_close_callback(const SGWindowCloseCallback& callback) noexcept
{
    sg_callbacks.window_close = callback;
}

void sg_set_window_iconify_callback(const SGWindowIconifyCallback& callback) noexcept
{
    sg_callbacks.window_iconify = callback;
}

void sg_set_frame_post_render_callback(const SGFramePostRenderCallback& callback) noexcept
{
    sg_callbacks.frame_render = callback;
}

// ---------------------------------------

void sg_call_core_init_callback()
{
    if(sg_callbacks.core_init)
    {
        sg_callbacks.core_init();
    }
}

void sg_call_window_close_callback(GLFWwindow* wnd)
{
    if(sg_callbacks.window_close)
    {
        sg_callbacks.window_close(wnd);
    }
}

void sg_call_window_iconify_callback(GLFWwindow* wnd, int iconified)
{
    if(sg_callbacks.window_iconify)
    {
        sg_callbacks.window_iconify(wnd, iconified);
    }
}

void sg_call_frame_post_render_callback()
{
    if(sg_callbacks.frame_render)
    {
        sg_callbacks.frame_render();
    }
}