//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_IPARALLELSYSTEM_H
#define SUNGEARENGINE_IPARALLELSYSTEM_H

#include <thread>
#include <unordered_set>

#include "ISystem.h"
#include "SGUtils/Timer.h"
#include "IParallelSystemSubprocess.h"
#include "SGUtils/Utils.h"

namespace SGCore
{
    template<typename ParallelSystemT>
    class IParallelSystem : public ISystem, public std::enable_shared_from_this<ParallelSystemT>
    {
    public:
        using parallel_subproc_t = IParallelSystemSubprocess<ParallelSystemT>;
        
        Timer m_timer;
        bool m_isAlive = true;
        
        IParallelSystem()
        {
            Ref<TimerCallback> updateCallback = MakeRef<TimerCallback>();
            updateCallback->setUpdateFunction([this](const double& dt, const double& fixedDt) {
                parallelUpdate(dt, fixedDt);
                
                std::lock_guard g(m_subprocessesVectorEditMutex);
                for(const auto& subprocess : m_subprocesses)
                {
                    subprocess->parallelUpdate(dt, fixedDt, this->shared_from_this());
                }
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
        
        template<typename SubprocessT>
        requires(std::is_base_of_v<parallel_subproc_t, SubprocessT>)
        Ref<SubprocessT> getSubprocess()
        {
            for(auto& system : m_subprocesses)
            {
                if(SG_INSTANCEOF(system.get(), SubprocessT))
                {
                    return std::static_pointer_cast<SubprocessT>(system);
                }
            }
            
            return nullptr;
        }
        
        void addSubprocess(const Ref<parallel_subproc_t>& subprocess) noexcept
        {
            std::lock_guard g(m_subprocessesVectorEditMutex);
            m_subprocesses.insert(subprocess);
        }
        
        void removeSubprocess(const Ref<parallel_subproc_t>& subprocess) noexcept
        {
            std::lock_guard g(m_subprocessesVectorEditMutex);
            m_subprocesses.erase(subprocess);
        }
        
        virtual void parallelUpdate(const double& dt, const double& fixedDt) noexcept { };
        
    private:
        std::unordered_set<Ref<parallel_subproc_t>> m_subprocesses;
        std::mutex m_subprocessesVectorEditMutex;
        
        std::thread m_thread;
    };
}

#endif //SUNGEARENGINE_IPARALLELSYSTEM_H
