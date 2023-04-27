//
// Created by stuka on 27.04.2023.
//

#pragma once

#ifndef NATIVECORE_INPUTLISTENER_H
#define NATIVECORE_INPUTLISTENER_H

struct SGKey
{
    int lastAction = 0;
    int currentAction = 0;
};

class InputListener
{
private:
    SGKey keyboardKeys[1024] = {};

public:
    InputListener() noexcept;

    void notifyKeyboard(const int&, const int&) noexcept;
    void notifyMouse(const int&, const int&) noexcept;

    bool keyboardKeyDown(const int&) noexcept;
    bool keyboardKeyPress(const int&) noexcept;
    bool keyboardKeyRelease(const int&) noexcept;
};


#endif //NATIVECORE_INPUTLISTENER_H
