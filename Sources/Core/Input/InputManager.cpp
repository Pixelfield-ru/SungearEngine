//
// Created by stuka on 26.04.2023.
//

#include "InputManager.h"

void InputManager::keyboardKeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
{
    std::for_each(inputListeners.begin(), inputListeners.end(), [&wnd, &key, &action](const std::shared_ptr<InputListener>& inputListener)
    {
        inputListener->notifyKeyboard(wnd, key, action);
    });

    sgCallWindowKeyCallback(wnd, key, scanCode, action, mods);
}

void InputManager::mouseButtonCallback(GLFWwindow* wnd, int button, int scanCode, int action)
{
    std::for_each(inputListeners.begin(), inputListeners.end(), [&wnd, &button, &action](const std::shared_ptr<InputListener>& inputListener)
    {
        inputListener->notifyMouse(wnd, button, action);
    });

    sgCallWindowMouseButtonCallback(wnd, button, scanCode, action);
}

void InputManager::addInputListener(InputListener* inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(keysMutex);
    inputListeners.push_back(static_cast<std::shared_ptr<InputListener>>(inputListener));
}
