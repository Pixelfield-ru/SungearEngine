#include "InputListener.h"
#include "InputManager.h"

InputListener::InputListener() noexcept
{
    InputManager::addInputListener(this);
}

void InputListener::notifyKeyboard(GLFWwindow* wnd, const int& key, const int& action) noexcept
{
    windowHandler = wnd;
}

void InputListener::notifyMouse(GLFWwindow* wnd, const int& button, const int& action) noexcept
{
    windowHandler = wnd;
}

bool InputListener::keyboardKeyDown(const int& key) noexcept
{
    return key >= KEY_FIRST && key <= KEY_LAST && windowHandler && glfwGetKey(windowHandler, key) == GLFW_PRESS;
}

bool InputListener::keyboardKeyPressed(const int& key) noexcept
{
    int lastAction = keyboardKeys[key];
    keyboardKeys[key] = keyboardKeyDown(key);

    return key >= KEY_FIRST && key <= KEY_LAST && windowHandler && lastAction == GLFW_RELEASE && keyboardKeys[key] == GLFW_PRESS;
}

bool InputListener::keyboardKeyReleased(const int& key) noexcept
{
    int lastAction = keyboardKeys[key];
    keyboardKeys[key] = keyboardKeyDown(key);

    return key >= KEY_FIRST && key <= KEY_LAST && windowHandler && lastAction == GLFW_PRESS && keyboardKeys[key] == GLFW_RELEASE;
}

bool InputListener::mouseButtonDown(const int& button) noexcept
{
    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && windowHandler && glfwGetMouseButton(windowHandler, button) == GLFW_PRESS;
}

bool InputListener::mouseButtonPressed(const int& button) noexcept
{
    int lastAction = mouseButtons[button];
    mouseButtons[button] = mouseButtonDown(button);

    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && windowHandler && lastAction == GLFW_RELEASE && mouseButtons[button] == GLFW_PRESS;
}

bool InputListener::mouseButtonReleased(const int& button) noexcept
{
    int lastAction = mouseButtons[button];
    mouseButtons[button] = mouseButtonDown(button);

    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && windowHandler && lastAction == GLFW_PRESS && mouseButtons[button] == GLFW_RELEASE;
}

