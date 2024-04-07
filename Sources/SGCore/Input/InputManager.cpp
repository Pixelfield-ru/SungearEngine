//
// Created by stuka on 26.04.2023.
//

#include "InputManager.h"
#include "SGCore/Main/CoreMain.h"

void SGCore::InputManager::init() noexcept
{
    addInputListener(mainInputListener);
    
    Window::onKeyboardKeyEvent.connect<&keyboardKeyCallback>();
    Window::onMouseButtonEvent.connect<&mouseButtonCallback>();
    Window::onCursorPositionChanged.connect<&cursorPositionCallback>();
}

void SGCore::InputManager::startFrame() noexcept
{
    for(const auto& inputListener : m_inputListeners)
    {
        inputListener->startFrame();
    }

    if(CoreMain::getWindow().isHideAndCentralizeCursor())
    {
        int windowSizeX;
        int windowSizeY;

        CoreMain::getWindow().getSize(windowSizeX, windowSizeY);
        CoreMain::getWindow().setCursorPosition((float) windowSizeX / 2.0f, (float) windowSizeY / 2.0f);

        for(const auto& inputListener : m_inputListeners)
        {
            inputListener->updateCursorPosition();
        }
    }
}

void SGCore::InputManager::keyboardKeyCallback(Window& wnd, const KeyboardKey& key, const int& scanCode, const KeyState& state, const int& mods)
{
    for(const auto& inputListener : m_inputListeners)
    {
        inputListener->notifyKeyboard(wnd, key, state);
    }
}

void SGCore::InputManager::mouseButtonCallback(Window& wnd, const MouseButton& button, const KeyState& state, const int& mods)
{
    for(const auto& inputListener : m_inputListeners)
    {
        inputListener->notifyMouse(wnd, button, state);
    }
}

void SGCore::InputManager::addInputListener(Ref<InputListener> inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(m_keysMutex);
    m_inputListeners.push_back(std::move(inputListener));
}

void SGCore::InputManager::removeInputListener(const Ref<InputListener>& inputListener) noexcept
{
    const std::lock_guard<std::mutex> guard(m_keysMutex);
    m_inputListeners.remove(inputListener);
}

SGCore::Ref<SGCore::InputListener> SGCore::InputManager::getMainInputListener() noexcept
{
    return mainInputListener;
}

void SGCore::InputManager::cursorPositionCallback(Window& window, const double& xPos, const double& yPos)
{
    /*if(Core::Main::Core::getWindow().getConfig().m_hideAndCentralizeCursor)
    {
        int windowSizeX;
        int windowSizeY;

        Core::Main::Core::getWindow().getSize(windowSizeX, windowSizeY);
        Core::Main::Core::getWindow().setCursorPosition((float) windowSizeX / 2.0f, (float) windowSizeY / 2.0f);
    }*/
}
