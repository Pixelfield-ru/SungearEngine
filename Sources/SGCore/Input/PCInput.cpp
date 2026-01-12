//
// Created by stuka on 16.10.2025.
//

#include "PCInput.h"

#include <cstring>

#include "SGCore/Main/Window.h"

void SGCore::Input::PC::startFrame() noexcept
{
    if(!m_windowHandle) return;

    m_cursorPositionLastX = m_cursorPositionX;
    m_cursorPositionLastY = m_cursorPositionY;

    updateCursorPosition();

    m_cursorPositionDeltaX = m_cursorPositionX - m_cursorPositionLastX;
    m_cursorPositionDeltaY = m_cursorPositionY - m_cursorPositionLastY;
}

bool SGCore::Input::PC::keyboardKeyDown(KeyboardKey key) noexcept
{
    return getKeyboardKeyState(key) == KeyState::PRESS;
}

bool SGCore::Input::PC::keyboardKeyPressed(KeyboardKey key) noexcept
{
    const auto underlyingKey = std::to_underlying(key);

    const KeyState lastAction = m_keyboardKeysStates[underlyingKey];
    m_keyboardKeysStates[underlyingKey] = keyboardKeyDown(key) ? KeyState::PRESS : KeyState::RELEASE;

    return lastAction == KeyState::RELEASE &&
           m_keyboardKeysStates[underlyingKey] == KeyState::PRESS;
}

bool SGCore::Input::PC::keyboardKeyReleased(KeyboardKey key) noexcept
{
    const auto underlyingKey = std::to_underlying(key);

    const KeyState lastAction = m_keyboardKeysStates[underlyingKey];
    m_keyboardKeysStates[underlyingKey] = keyboardKeyDown(key) ? KeyState::PRESS : KeyState::RELEASE;

    return lastAction == KeyState::PRESS &&
           m_keyboardKeysStates[underlyingKey] == KeyState::RELEASE;
}

bool SGCore::Input::PC::mouseButtonDown(MouseButton button) noexcept
{
    return getMouseButtonState(button) == KeyState::PRESS;
}

bool SGCore::Input::PC::mouseButtonPressed(MouseButton button) noexcept
{
    const auto underlyingButton = std::to_underlying(button);

    const int lastAction = m_mouseButtonsStates[underlyingButton];
    m_mouseButtonsStates[underlyingButton] = mouseButtonDown(button) ? KeyState::PRESS : KeyState::RELEASE;

    return lastAction == KeyState::RELEASE &&
           m_mouseButtonsStates[underlyingButton] == KeyState::PRESS;
}

bool SGCore::Input::PC::mouseButtonReleased(MouseButton button) noexcept
{
    const auto underlyingButton = std::to_underlying(button);

    const int lastAction = m_mouseButtonsStates[underlyingButton];
    m_mouseButtonsStates[underlyingButton] = mouseButtonDown(button) ? KeyState::PRESS : KeyState::RELEASE;

    return lastAction == KeyState::PRESS &&
           m_mouseButtonsStates[underlyingButton] == KeyState::RELEASE;
}

void SGCore::Input::PC::updateCursorPosition() noexcept
{
    if(!m_windowHandle) return;

    getCursorPosition(m_cursorPositionX, m_cursorPositionY);
}

void SGCore::Input::PC::getCursorPosition(double& x, double& y) noexcept
{
#if SG_HAS_PC_INPUT
    glfwGetCursorPos(m_windowHandle, &x, &y);
#endif
}

void SGCore::Input::PC::setCursorPosition(double x, double y) noexcept
{
#if SG_HAS_PC_INPUT
    glfwSetCursorPos(m_windowHandle, x, y);
#endif
}

double SGCore::Input::PC::getCursorPositionLastX() noexcept
{
    return m_cursorPositionLastX;
}

double SGCore::Input::PC::getCursorPositionLastY() noexcept
{
    return m_cursorPositionLastY;
}

double SGCore::Input::PC::getCursorPositionX() noexcept
{
    return m_cursorPositionX;
}

double SGCore::Input::PC::getCursorPositionY() noexcept
{
    return m_cursorPositionY;
}

double SGCore::Input::PC::getCursorPositionDeltaX() noexcept
{
    return m_cursorPositionDeltaX;
}

double SGCore::Input::PC::getCursorPositionDeltaY() noexcept
{
    return m_cursorPositionDeltaY;
}

void SGCore::Input::PC::setupInput(Window& window) noexcept
{
    m_windowHandle = window.getNativeHandle();

    std::memset(m_keyboardKeysStates, KeyState::RELEASE, sizeof(m_keyboardKeysStates));
    std::memset(m_mouseButtonsStates, KeyState::RELEASE, sizeof(m_mouseButtonsStates));

#if SG_HAS_PC_INPUT
    glfwSetKeyCallback(m_windowHandle, nativeKeyboardKeyCallback);
    glfwSetMouseButtonCallback(m_windowHandle, nativeMouseButtonCallback);
    glfwSetCursorPosCallback(m_windowHandle, nativeMousePositionCallback);
#endif
}

void SGCore::Input::PC::nativeKeyboardKeyCallback(window_handle window, int key, int scancode, int action, int mods) noexcept
{
#if SG_HAS_PC_INPUT
    onKeyboardKeyEvent(*((Window*) glfwGetWindowUserPointer(window)), (KeyboardKey) key, scancode, (KeyState) action, mods);
#endif
}

void SGCore::Input::PC::nativeMouseButtonCallback(window_handle window, int button, int action, int mods) noexcept
{
#if SG_HAS_PC_INPUT
    onMouseButtonEvent(*((Window*) glfwGetWindowUserPointer(window)), (MouseButton) button, (KeyState) action, mods);
#endif
}

void SGCore::Input::PC::nativeMousePositionCallback(window_handle window, double xpos, double ypos) noexcept
{
#if SG_HAS_PC_INPUT
    onCursorPositionChanged(*((Window*) glfwGetWindowUserPointer(window)), xpos, ypos);
#endif
}

SGCore::Input::KeyState SGCore::Input::PC::getMouseButtonState(MouseButton button)
{
#if SG_HAS_PC_INPUT
    return (KeyState) glfwGetMouseButton(m_windowHandle, std::to_underlying(button));
#else
    return KeyState::RELEASE;
#endif
}

SGCore::Input::KeyState SGCore::Input::PC::getKeyboardKeyState(KeyboardKey key) noexcept
{
#if SG_HAS_PC_INPUT
    return (KeyState) glfwGetKey(m_windowHandle, std::to_underlying(key));
#else
    return KeyState::RELEASE;
#endif
}
