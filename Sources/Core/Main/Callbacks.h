#ifndef NATIVECORE_CALLBACKS_H
#define NATIVECORE_CALLBACKS_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

typedef void(*SGCoreInitCallback)();
typedef void(*SGWindowCloseCallback)(GLFWwindow*);
typedef void(*SGWindowIconifyCallback)(GLFWwindow*, int);
typedef void(*SGWindowKeyCallback)(GLFWwindow*, int, int, int, int);
typedef void(*SGFramePostRenderCallback)();

void sgSetCoreInitCallback(const SGCoreInitCallback&) noexcept;
void sgSetWindowCloseCallback(const SGWindowCloseCallback&) noexcept;
void sgSetWindowIconifyCallback(const SGWindowIconifyCallback&) noexcept;
void sgSetWindowKeyCallback(const SGWindowKeyCallback&) noexcept;
void sgSetFramePostRenderCallback(const SGFramePostRenderCallback&) noexcept;

void sgCallCoreInitCallback();
void sgCallWindowCloseCallback(GLFWwindow*);
void sgCallWindowIconifyCallback(GLFWwindow*, int iconified);
void sgCallWindowKeyCallback(GLFWwindow*, int key, int scanCode, int action, int mods);
void sgCallFramePostRenderCallback();

#endif //NATIVECORE_CALLBACKS_H
