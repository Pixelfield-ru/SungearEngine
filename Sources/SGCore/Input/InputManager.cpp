//
// Created by stuka on 26.04.2023.
//

#include "InputManager.h"
#include "SGCore/Main/Core.h"

void InputManager::init() noexcept
{
    addInputListener(mainInputListener);
}

void InputManager::startFrame() noexcept
{
    for(const auto& inputListener : m_inputListeners)
    {
        inputListener->startFrame();
    }
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

void InputManager::addInputListener(std::shared_ptr<InputListener> inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(m_keysMutex);
    m_inputListeners.push_back(std::move(inputListener));
}

void InputManager::removeInputListener(const std::shared_ptr<InputListener>& inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(m_keysMutex);
    m_inputListeners.remove(inputListener);
}

std::shared_ptr<InputListener> InputManager::getMainInputListener() noexcept
{
    return mainInputListener;
}

void InputManager::cursorPositionCallback(GLFWwindow* window, double xpos, double ypos)
{
    /*if(Core::Main::Core::getWindow().getConfig().m_hideAndCentralizeCursor)
    {
        int windowSizeX;
        int windowSizeY;

        Core::Main::Core::getWindow().getSize(windowSizeX, windowSizeY);
        Core::Main::Core::getWindow().setCursorPosition((float) windowSizeX / 2.0f, (float) windowSizeY / 2.0f);
    }*/
}
