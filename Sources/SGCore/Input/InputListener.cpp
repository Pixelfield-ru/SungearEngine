#include "InputListener.h"
#include "InputManager.h"
#include "SGCore/Main/CoreMain.h"

SGCore::InputListener::InputListener() noexcept
{
    // TODO: AUTO ADD TO MANAGER
}

void SGCore::InputListener::startFrame() noexcept
{
    if(!m_focusedWindowHandler) return;

    m_cursorPositionLastX = m_cursorPositionX;
    m_cursorPositionLastY = m_cursorPositionY;

    updateCursorPosition();

    m_cursorPositionDeltaX = m_cursorPositionX - m_cursorPositionLastX;
    m_cursorPositionDeltaY = m_cursorPositionY - m_cursorPositionLastY;
}

void SGCore::InputListener::notifyKeyboard(GLFWwindow* wnd, const int& key, const int& action) noexcept
{
    m_focusedWindowHandler = wnd;
}

void SGCore::InputListener::notifyMouse(GLFWwindow* wnd, const int& button, const int& action) noexcept
{
    m_focusedWindowHandler = wnd;
}

bool SGCore::InputListener::keyboardKeyDown(const int& key) noexcept
{
    return key >= KEY_FIRST && key <= KEY_LAST && m_focusedWindowHandler && glfwGetKey(m_focusedWindowHandler, key) == GLFW_PRESS;
}

bool SGCore::InputListener::keyboardKeyPressed(const int& key) noexcept
{
    int lastAction = m_keyboardKeys[key];
    m_keyboardKeys[key] = keyboardKeyDown(key);

    return key >= KEY_FIRST && key <= KEY_LAST && m_focusedWindowHandler && lastAction == GLFW_RELEASE && m_keyboardKeys[key] == GLFW_PRESS;
}

bool SGCore::InputListener::keyboardKeyReleased(const int& key) noexcept
{
    int lastAction = m_keyboardKeys[key];
    m_keyboardKeys[key] = keyboardKeyDown(key);

    return key >= KEY_FIRST && key <= KEY_LAST && m_focusedWindowHandler && lastAction == GLFW_PRESS && m_keyboardKeys[key] == GLFW_RELEASE;
}

bool SGCore::InputListener::mouseButtonDown(const int& button) noexcept
{
    //std::cout <<  m_focusedWindowHandler << std::endl;
    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && m_focusedWindowHandler && glfwGetMouseButton(m_focusedWindowHandler, button) == GLFW_PRESS;
}

bool SGCore::InputListener::mouseButtonPressed(const int& button) noexcept
{
    int lastAction = m_mouseButtons[button];
    m_mouseButtons[button] = mouseButtonDown(button);

    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && m_focusedWindowHandler && lastAction == GLFW_RELEASE && m_mouseButtons[button] == GLFW_PRESS;
}

bool SGCore::InputListener::mouseButtonReleased(const int& button) noexcept
{
    int lastAction = m_mouseButtons[button];
    m_mouseButtons[button] = mouseButtonDown(button);

    return button >= MOUSE_BUTTON_FIRST && button <= MOUSE_BUTTON_LAST && m_focusedWindowHandler && lastAction == GLFW_PRESS && m_mouseButtons[button] == GLFW_RELEASE;
}

double SGCore::InputListener::getCursorPositionLastX() const noexcept
{
    return m_cursorPositionLastX;
}

double SGCore::InputListener::getCursorPositionLastY() const noexcept
{
    return m_cursorPositionLastY;
}

double SGCore::InputListener::getCursorPositionX() const noexcept
{
    return m_cursorPositionX;
}

double SGCore::InputListener::getCursorPositionY() const noexcept
{
    return m_cursorPositionY;
}

double SGCore::InputListener::getCursorPositionDeltaX() const noexcept
{
    return m_cursorPositionDeltaX;
}

double SGCore::InputListener::getCursorPositionDeltaY() const noexcept
{
    return m_cursorPositionDeltaY;
}

void SGCore::InputListener::updateCursorPosition() noexcept
{
    if(!m_focusedWindowHandler) return;

    glfwGetCursorPos(m_focusedWindowHandler, &m_cursorPositionX, &m_cursorPositionY);
}

