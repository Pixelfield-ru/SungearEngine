#include "InputListener.h"
#include "InputManager.h"

InputListener::InputListener() noexcept
{
    //InputManager::addInputListener(std::make_shared<InputListener>(*this));
}

void InputListener::notifyKeyboard(GLFWwindow* wnd, const int& key, const int& action) noexcept
{
    m_focusedWindowHandler = wnd;
    std::cout << m_focusedWindowHandler << std::endl;
}

void InputListener::notifyMouse(GLFWwindow* wnd, const int& button, const int& action) noexcept
{
    m_focusedWindowHandler = wnd;
}

bool InputListener::keyboardKeyDown(const int& key) noexcept
{
    return key >= KEY_FIRST && key <= KEY_LAST && m_focusedWindowHandler && glfwGetKey(m_focusedWindowHandler, key) == GLFW_PRESS;
}

bool InputListener::keyboardKeyPressed(const int& key) noexcept
{
    int lastAction = m_keyboardKeys[key];
    m_keyboardKeys[key] = keyboardKeyDown(key);

    return key >= KEY_FIRST && key <= KEY_LAST && m_focusedWindowHandler && lastAction == GLFW_RELEASE && m_keyboardKeys[key] == GLFW_PRESS;
}

bool InputListener::keyboardKeyReleased(const int& key) noexcept
{
    int lastAction = m_keyboardKeys[key];
    m_keyboardKeys[key] = keyboardKeyDown(key);

    return key >= KEY_FIRST && key <= KEY_LAST && m_focusedWindowHandler && lastAction == GLFW_PRESS && m_keyboardKeys[key] == GLFW_RELEASE;
}

bool InputListener::mouseButtonDown(const int& button) noexcept
{
    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && m_focusedWindowHandler && glfwGetMouseButton(m_focusedWindowHandler, button) == GLFW_PRESS;
}

bool InputListener::mouseButtonPressed(const int& button) noexcept
{
    int lastAction = m_mouseButtons[button];
    m_mouseButtons[button] = mouseButtonDown(button);

    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && m_focusedWindowHandler && lastAction == GLFW_RELEASE && m_mouseButtons[button] == GLFW_PRESS;
}

bool InputListener::mouseButtonReleased(const int& button) noexcept
{
    int lastAction = m_mouseButtons[button];
    m_mouseButtons[button] = mouseButtonDown(button);

    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && m_focusedWindowHandler && lastAction == GLFW_PRESS && m_mouseButtons[button] == GLFW_RELEASE;
}

