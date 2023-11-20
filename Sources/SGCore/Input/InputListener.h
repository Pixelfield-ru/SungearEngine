//
// Created by stuka on 27.04.2023.
//

#ifndef SUNGEARENGINE_INPUTLISTENER_H
#define SUNGEARENGINE_INPUTLISTENER_H

#include <atomic>
#include <memory>
#include <GLFW/glfw3.h>

namespace SGCore
{
    class InputListener : public std::enable_shared_from_this<InputListener>
    {
    private:
        bool m_keyboardKeys[GLFW_KEY_LAST];
        bool m_mouseButtons[GLFW_MOUSE_BUTTON_LAST];

        double m_cursorPositionLastX = 0;
        double m_cursorPositionLastY = 0;

        double m_cursorPositionX = 0;
        double m_cursorPositionY = 0;

        double m_cursorPositionDeltaX = 0;
        double m_cursorPositionDeltaY = 0;

        GLFWwindow* m_focusedWindowHandler = nullptr;

    public:
        InputListener() noexcept;

        void startFrame() noexcept;

        void notifyKeyboard(GLFWwindow*, const int&, const int&) noexcept;

        void notifyMouse(GLFWwindow*, const int&, const int&) noexcept;

        bool keyboardKeyDown(const int&) noexcept;

        bool keyboardKeyPressed(const int&) noexcept;

        bool keyboardKeyReleased(const int&) noexcept;

        bool mouseButtonDown(const int&) noexcept;

        bool mouseButtonPressed(const int&) noexcept;

        bool mouseButtonReleased(const int&) noexcept;

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
