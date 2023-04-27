#include "InputListener.h"

#include "Keyboard.h"

InputListener::InputListener() noexcept
{
    Keyboard::addInputListener(this);
}

void InputListener::notifyKeyboard(const int& keyID, const int& keyAction) noexcept
{
    int lastAction = keyboardKeys[keyID].currentAction;
    keyboardKeys[keyID].currentAction = keyAction;
    keyboardKeys[keyID].lastAction = lastAction;
}

void InputListener::notifyMouse(const int& buttonID, const int& buttonState) noexcept
{
    //mouseButtonID = buttonID;
    //mouseButtonState = buttonState;
}

bool InputListener::keyboardKeyDown(const int& keyID) noexcept
{
    return keyID > 0 && keyID < 1024 && (keyboardKeys[keyID].currentAction == GLFW_PRESS || keyboardKeys[keyID].currentAction == GLFW_REPEAT);
}

bool InputListener::keyboardKeyPress(const int& keyID) noexcept
{
    bool pressed = keyboardKeys[keyID].currentAction == GLFW_PRESS && keyboardKeys[keyID].lastAction != GLFW_PRESS;
    keyboardKeys[keyID].lastAction = keyboardKeys[keyID].currentAction;
    return keyID > 0 && keyID < 1024 && pressed;
}

bool InputListener::keyboardKeyRelease(const int& keyID) noexcept
{
    bool released = keyboardKeys[keyID].currentAction == GLFW_RELEASE && keyboardKeys[keyID].lastAction != GLFW_RELEASE;
    keyboardKeys[keyID].lastAction = keyboardKeys[keyID].currentAction;
    return keyID > 0 && keyID < 1024 && released;
}