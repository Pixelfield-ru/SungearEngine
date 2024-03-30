//
// Created by stuka on 26.04.2023.
//

#pragma once

#ifndef NATIVECORE_INPUTMANAGER_H
#define NATIVECORE_INPUTMANAGER_H

#include <iostream>
#include <memory>
#include <vector>
#include <mutex>
#include <list>
#include <memory>
#include <algorithm>
#include <thread>
#include <sgcore_export.h>

#include "SGCore/Main/CoreGlobals.h"
#include "InputListener.h"
#include "SGCore/Main/Window.h"

namespace SGCore
{
    class SGCORE_EXPORT InputManager
    {
    private:
        static inline std::list<Ref<InputListener>> m_inputListeners;
        static inline std::mutex m_keysMutex;
        static const inline Ref<InputListener> mainInputListener = MakeRef<InputListener>();

    public:
        InputManager() = delete;

        static void init() noexcept;

        static void startFrame() noexcept;

        static void keyboardKeyCallback(Window& wnd, const KeyboardKey& key, const int& scanCode, const KeyState& action, const int& mods);

        static void mouseButtonCallback(Window& wnd, const MouseButton& button, const KeyState& state, const int& mods);

        static void cursorPositionCallback(Window& window, const double& xPos, const double& yPos);

        static void addInputListener(Ref<InputListener>) noexcept;

        static void removeInputListener(const Ref<InputListener>&) noexcept;

        static Ref<InputListener> getMainInputListener() noexcept;
    };
}

//const static inline Ref<InputListener> mainInputListener = std::make_shared<InputListener>();

#endif //NATIVECORE_INPUTMANAGER_H
