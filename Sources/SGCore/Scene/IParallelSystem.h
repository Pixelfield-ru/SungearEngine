//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_IPARALLELSYSTEM_H
#define SUNGEARENGINE_IPARALLELSYSTEM_H

#include <thread>

#include "ISystem.h"
#include "SGUtils/Timer.h"

namespace SGCore
{
    class IParallelSystem : public ISystem
    {
    public:
        Timer m_timer;
        bool m_isAlive = true;
        
        IParallelSystem()
        {
            Ref<TimerCallback> updateCallback = MakeRef<TimerCallback>();
            updateCallback->setUpdateFunction([this](const double& dt, const double& fixedDt) {
                parallelUpdate(dt, fixedDt);
            });
            
            m_timer.setTargetFrameRate(80);
            m_timer.m_cyclic = true;
            m_timer.addCallback(updateCallback);
            
            m_thread = std::thread([this]() {
                while(m_isAlive)
                {
                    if(m_active)
                    {
                        m_timer.startFrame();
                    }
                }
            });
        }
        
        ~IParallelSystem()
        {
            m_isAlive = false;
            m_thread.join();
        }
        
        virtual void parallelUpdate(const double& dt, const double& fixedDt) noexcept { };
        
    private:
        std::thread m_thread;
    };
}

#endif //SUNGEARENGINE_IPARALLELSYSTEM_H
