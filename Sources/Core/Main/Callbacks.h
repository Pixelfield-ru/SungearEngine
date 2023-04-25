#pragma once

#ifndef NATIVECORE_CALLBACKS_H
#define NATIVECORE_CALLBACKS_H

#include <GLFW/glfw3.h>

typedef void(*SGCoreInitCallback)();
typedef void(*SGWindowCloseCallback)(GLFWwindow*);
typedef void(*SGWindowIconifyCallback)(GLFWwindow*, int);
typedef void(*SGFramePostRenderCallback)();

void sg_set_core_init_callback(const SGCoreInitCallback&) noexcept;
void sg_set_window_close_callback(const SGWindowCloseCallback&) noexcept;
void sg_set_window_iconify_callback(const SGWindowIconifyCallback&) noexcept;
void sg_set_frame_post_render_callback(const SGFramePostRenderCallback&) noexcept;

void sg_call_core_init_callback();
void sg_call_window_close_callback(GLFWwindow*);
void sg_call_window_iconify_callback(GLFWwindow*, int);
void sg_call_frame_post_render_callback();

#endif //NATIVECORE_CALLBACKS_H
