//
// Created by stuka on 27.04.2023.
//

#pragma once

#ifndef NATIVECORE_INPUTLISTENER_H
#define NATIVECORE_INPUTLISTENER_H

#include <atomic>
#include <memory>
#include <GLFW/glfw3.h>

class InputListener : public std::enable_shared_from_this<InputListener>
{
private:
    bool m_keyboardKeys[GLFW_KEY_LAST];
    bool m_mouseButtons[GLFW_MOUSE_BUTTON_LAST];

    double m_mouseLastX = 0;
    double m_mouseLastY = 0;

    double m_mouseX = 0;
    double m_mouseY = 0;

    double m_mouseDeltaX = 0;
    double m_mouseDeltaY = 0;

    GLFWwindow* m_focusedWindowHandler = nullptr;
    //std::mutex m_inputMutex;

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

    double getMouseLastX() const noexcept;
    double getMouseLastY() const noexcept;

    double getMouseX() const noexcept;
    double getMouseY() const noexcept;

    double getMouseDeltaX() const noexcept;
    double getMouseDeltaY() const noexcept;
};




#endif //NATIVECORE_INPUTLISTENER_H
