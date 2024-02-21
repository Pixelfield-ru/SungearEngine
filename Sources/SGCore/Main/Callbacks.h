#ifndef NATIVECORE_CALLBACKS_H
#define NATIVECORE_CALLBACKS_H

#define GLFW_INCLUDE_NONE

#include <GLFW/glfw3.h>

// glfw standard callbacks -------------------------------------------------
typedef void(*SGCoreInitCallback)();
typedef void(*SGWindowCloseCallback)(GLFWwindow*);
typedef void(*SGWindowIconifyCallback)(GLFWwindow*, int);
typedef void(*SGWindowKeyCallback)(GLFWwindow*, int, int, int, int);
typedef void(*SGWindowMouseButtonCallback)(GLFWwindow*, int, int, int);
// -------------------------------------------------------------------------
// update callbacks ---------------------------------------------------------
typedef void(*SGFixedUpdateCallback)(const double& dt, const double& fixedDt);
typedef void(*SGUpdateCallback)(const double& dt);
// --------------------------------------------------------------------------

// glfw standard callbacks -------------------------------------------------
void sgSetCoreInitCallback(const SGCoreInitCallback&) noexcept;
void sgSetWindowCloseCallback(const SGWindowCloseCallback&) noexcept;
void sgSetWindowIconifyCallback(const SGWindowIconifyCallback&) noexcept;
void sgSetWindowKeyCallback(const SGWindowKeyCallback&) noexcept;
void sgSetWindowMouseButtonCallback(const SGWindowMouseButtonCallback&) noexcept;

void sgSetFixedUpdateCallback(const SGFixedUpdateCallback& callback) noexcept;
void sgSetUpdateCallback(const SGUpdateCallback& callback) noexcept;

void sgCallCoreInitCallback();
void sgCallWindowCloseCallback(GLFWwindow*);
void sgCallWindowIconifyCallback(GLFWwindow*, int);
void sgCallWindowKeyCallback(GLFWwindow*, int, int, int, int);
void sgCallWindowMouseButtonCallback(GLFWwindow*, int, int, int);

void sgCallFixedUpdateCallback(const double& dt, const double& fixedDt);
void sgCallUpdateCallback(const double& dt);

#endif //NATIVECORE_CALLBACKS_H
