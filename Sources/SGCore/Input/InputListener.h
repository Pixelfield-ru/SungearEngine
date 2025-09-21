//
// Created by stuka on 27.04.2023.
//

#ifndef SUNGEARENGINE_INPUTLISTENER_H
#define SUNGEARENGINE_INPUTLISTENER_H

#include <SGCore/pch.h>

#include "SGCore/Main/Window.h"

namespace SGCore
{
    struct InputListener : public std::enable_shared_from_this<InputListener>
    {
        Signal<void(Window& inWindow, KeyboardKey key, KeyState state)> onKeyboardKeyStateChanged;
        Signal<void(Window& inWindow, MouseButton button, KeyState state)> onMouseButtonStateChanged;

        InputListener() noexcept;

        void startFrame() noexcept;

        void notifyKeyboard(Window&, KeyboardKey key, KeyState state) noexcept;

        void notifyMouse(Window&, MouseButton button, KeyState state) noexcept;

        bool keyboardKeyDown(KeyboardKey key) noexcept;

        bool keyboardKeyPressed(KeyboardKey key) noexcept;

        bool keyboardKeyReleased(KeyboardKey key) noexcept;

        bool mouseButtonDown(MouseButton) noexcept;

        bool mouseButtonPressed(MouseButton) noexcept;

        bool mouseButtonReleased(MouseButton) noexcept;

        double getCursorPositionLastX() const noexcept;

        double getCursorPositionLastY() const noexcept;

        double getCursorPositionX() const noexcept;

        double getCursorPositionY() const noexcept;

        double getCursorPositionDeltaX() const noexcept;

        double getCursorPositionDeltaY() const noexcept;

        void updateCursorPosition() noexcept;

    private:
        KeyState m_keyboardKeysStates[std::to_underlying(KeyboardKey::KEY_LAST) + 1];
        KeyState m_mouseButtonsStates[std::to_underlying(MouseButton::MOUSE_BUTTON_LAST) + 1];

        double m_cursorPositionLastX = 0;
        double m_cursorPositionLastY = 0;

        double m_cursorPositionX = 0;
        double m_cursorPositionY = 0;

        double m_cursorPositionDeltaX = 0;
        double m_cursorPositionDeltaY = 0;

        Window* m_focusedWindow = nullptr;
    };
}

#endif // SUNGEARENGINE_INPUTLISTENER_H
