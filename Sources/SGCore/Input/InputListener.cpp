#include "InputListener.h"

SGCore::InputListener::InputListener() noexcept
{
    // TODO: AUTO ADD TO MANAGER
}

void SGCore::InputListener::startFrame() noexcept
{
    if(!m_focusedWindow) return;

    m_cursorPositionLastX = m_cursorPositionX;
    m_cursorPositionLastY = m_cursorPositionY;

    updateCursorPosition();

    m_cursorPositionDeltaX = m_cursorPositionX - m_cursorPositionLastX;
    m_cursorPositionDeltaY = m_cursorPositionY - m_cursorPositionLastY;
}

void SGCore::InputListener::notifyKeyboard(Window& wnd, KeyboardKey key, KeyState state) noexcept
{
    m_focusedWindow = &wnd;

    onKeyboardKeyStateChanged(wnd, key, state);
}

void SGCore::InputListener::notifyMouse(Window& wnd, MouseButton button, KeyState state) noexcept
{
    m_focusedWindow = &wnd;

    onMouseButtonStateChanged(wnd, button, state);
}

bool SGCore::InputListener::keyboardKeyDown(KeyboardKey key) noexcept
{
    auto underlyingKey = std::to_underlying(key);

    return m_focusedWindow && m_focusedWindow->getKeyboardKeyState(key) == KeyState::PRESS;
}

bool SGCore::InputListener::keyboardKeyPressed(KeyboardKey key) noexcept
{
    auto underlyingKey = std::to_underlying(key);
    
    KeyState lastAction = m_keyboardKeysStates[underlyingKey];
    m_keyboardKeysStates[underlyingKey] = keyboardKeyDown(key) ? KeyState::PRESS : KeyState::RELEASE;
    
    return m_focusedWindow &&
           lastAction == KeyState::RELEASE &&
           m_keyboardKeysStates[underlyingKey] == KeyState::PRESS;
}

bool SGCore::InputListener::keyboardKeyReleased(KeyboardKey key) noexcept
{
    auto underlyingKey = std::to_underlying(key);
    
    KeyState lastAction = m_keyboardKeysStates[underlyingKey];
    m_keyboardKeysStates[underlyingKey] = keyboardKeyDown(key) ? KeyState::PRESS : KeyState::RELEASE;

    return m_focusedWindow &&
           lastAction == KeyState::PRESS &&
           m_keyboardKeysStates[underlyingKey] == KeyState::RELEASE;
}

bool SGCore::InputListener::mouseButtonDown(MouseButton button) noexcept
{
    //std::cout <<  m_focusedWindowHandler << std::endl;
    return m_focusedWindow && m_focusedWindow->getMouseButtonState(button) == KeyState::PRESS;
}

bool SGCore::InputListener::mouseButtonPressed(MouseButton button) noexcept
{
    auto underlyingButton = std::to_underlying(button);
    
    int lastAction = m_mouseButtonsStates[underlyingButton];
    m_mouseButtonsStates[underlyingButton] = mouseButtonDown(button) ? KeyState::PRESS : KeyState::RELEASE;

    return m_focusedWindow && lastAction == KeyState::RELEASE && m_mouseButtonsStates[underlyingButton] == KeyState::PRESS;
}

bool SGCore::InputListener::mouseButtonReleased(MouseButton button) noexcept
{
    auto underlyingButton = std::to_underlying(button);
    
    int lastAction = m_mouseButtonsStates[underlyingButton];
    m_mouseButtonsStates[underlyingButton] = mouseButtonDown(button) ? KeyState::PRESS : KeyState::RELEASE;

    return m_focusedWindow && lastAction == KeyState::PRESS && m_mouseButtonsStates[underlyingButton] == KeyState::RELEASE;
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
    if(!m_focusedWindow) return;
    
    m_focusedWindow->getCursorPosition(m_cursorPositionX, m_cursorPositionY);
}

