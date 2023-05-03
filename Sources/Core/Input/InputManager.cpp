//
// Created by stuka on 26.04.2023.
//

#include "InputManager.h"

void InputManager::keyboardKeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
{
    for(const auto& inputListener : inputListeners)
    {
        inputListener->notifyKeyboard(wnd, key, action);
    }

    sgCallWindowKeyCallback(wnd, key, scanCode, action, mods);
}

void InputManager::mouseButtonCallback(GLFWwindow* wnd, int button, int scanCode, int action)
{
    for(const auto& inputListener : inputListeners)
    {
        inputListener->notifyMouse(wnd, button, action);
    }

    sgCallWindowMouseButtonCallback(wnd, button, scanCode, action);
}

void InputManager::addInputListener(InputListener* inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(keysMutex);
    inputListeners.push_back(static_cast<std::shared_ptr<InputListener>>(inputListener));
}
