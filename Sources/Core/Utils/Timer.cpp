//
// Created by stuka on 03.05.2023.
//

#include <chrono>
#include <GLFW/glfw3.h>

#include "Timer.h"
#include "../Main/Window.h"
#include "../Main/Core.h"

void Core::Utils::Timer::notifyObservers()
{
    for(const std::shared_ptr<Observer::IObserver>& observer : observers)
    {
        observer->notify(this);
    }
}

void Core::Utils::Timer::startFrame() noexcept
{
    if(!m_active) return;

    if(m_firstTime)
    {
        firstTimeStart();
        m_firstTime = false;
    }

    m_current = glfwGetTime();
}

void Core::Utils::Timer::endFrame()
{
    if(!m_active) return;

    long double last = m_current;
    m_current = glfwGetTime();

    m_deltaTime = m_current - last;

    //m_progress += m_deltaTime * 29.5;
    m_progress = m_current - m_startTime;

    if(m_progress > m_destination)
    {
        std::cout << "destination reached!" << std::endl;

        m_active = m_cyclic;

        Core::Main::Core::getWindow().setShouldClose(true);
    }

    std::cout << m_progress << std::endl;
}

void Core::Utils::Timer::firstTimeStart()
{
    m_startTime = glfwGetTime();
}