//
// Created by stuka on 27.04.2023.
//

#pragma once

#ifndef NATIVECORE_INPUTLISTENER_H
#define NATIVECORE_INPUTLISTENER_H

#include <atomic>
#include <GLFW/glfw3.h>

class InputListener
{
private:
    bool keyboardKeys[GLFW_KEY_LAST] = {};
    bool mouseButtons[GLFW_MOUSE_BUTTON_LAST] = {};

    GLFWwindow* windowHandler;
    std::mutex inputMutex;

public:
    InputListener() noexcept;

    void notifyKeyboard(GLFWwindow*, const int&, const int&) noexcept;
    void notifyMouse(GLFWwindow*, const int&, const int&) noexcept;

    bool keyboardKeyDown(const int&) noexcept;
    bool keyboardKeyPressed(const int&) noexcept;
    bool keyboardKeyReleased(const int&) noexcept;

    bool mouseButtonDown(const int&) noexcept;
    bool mouseButtonPressed(const int&) noexcept;
    bool mouseButtonReleased(const int&) noexcept;
};




#endif //NATIVECORE_INPUTLISTENER_H
