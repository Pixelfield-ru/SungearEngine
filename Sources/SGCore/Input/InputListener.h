//
// Created by stuka on 27.04.2023.
//

#ifndef SUNGEARENGINE_INPUTLISTENER_H
#define SUNGEARENGINE_INPUTLISTENER_H

#include <atomic>
#include <memory>
#include "SGCore/Main/Window.h"

namespace SGCore
{
    class InputListener : public std::enable_shared_from_this<InputListener>
    {
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

    public:
        InputListener() noexcept;

        void startFrame() noexcept;

        void notifyKeyboard(Window&, const KeyboardKey& key, const KeyState& state) noexcept;

        void notifyMouse(Window&, const MouseButton& button, const KeyState& state) noexcept;

        bool keyboardKeyDown(const KeyboardKey&) noexcept;

        bool keyboardKeyPressed(const KeyboardKey&) noexcept;

        bool keyboardKeyReleased(const KeyboardKey&) noexcept;

        bool mouseButtonDown(const MouseButton&) noexcept;

        bool mouseButtonPressed(const MouseButton&) noexcept;

        bool mouseButtonReleased(const MouseButton&) noexcept;

        double getCursorPositionLastX() const noexcept;

        double getCursorPositionLastY() const noexcept;

        double getCursorPositionX() const noexcept;

        double getCursorPositionY() const noexcept;

        double getCursorPositionDeltaX() const noexcept;

        double getCursorPositionDeltaY() const noexcept;

        void updateCursorPosition() noexcept;
    };
}

#endif // SUNGEARENGINE_INPUTLISTENER_H
