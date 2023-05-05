//
// Created by stuka on 26.04.2023.
//

#include "InputManager.h"

void InputManager::init() noexcept
{
    addInputListener(mainInputListener);
}

void InputManager::keyboardKeyCallback(GLFWwindow* wnd, int key, int scanCode, int action, int mods)
{
    for(const auto& inputListener : m_inputListeners)
    {
        inputListener->notifyKeyboard(wnd, key, action);
    }

    sgCallWindowKeyCallback(wnd, key, scanCode, action, mods);
}

void InputManager::mouseButtonCallback(GLFWwindow* wnd, int button, int scanCode, int action)
{
    for(const auto& inputListener : m_inputListeners)
    {
        inputListener->notifyMouse(wnd, button, action);
    }

    sgCallWindowMouseButtonCallback(wnd, button, scanCode, action);
}

void InputManager::addInputListener(const std::shared_ptr<InputListener>& inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(m_keysMutex);
    m_inputListeners.push_back(inputListener);
}

void InputManager::removeInputListener(const std::shared_ptr<InputListener>& inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(m_keysMutex);
    m_inputListeners.remove(inputListener);
}
