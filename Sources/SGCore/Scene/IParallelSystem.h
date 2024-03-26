//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_IPARALLELSYSTEM_H
#define SUNGEARENGINE_IPARALLELSYSTEM_H

#include <thread>
#include <unordered_set>
#include <mutex>

#include "ISystem.h"
#include "SGUtils/Timer.h"
#include "IParallelSystemSubprocess.h"
#include "SGUtils/Utils.h"

namespace SGCore
{
    template<typename ParallelSystemT>
    class IParallelSystem : public ISystem, public std::enable_shared_from_this<ParallelSystemT>
    {
        template<typename>
        friend struct EventImpl;
        
    public:
        using subproc_t = IParallelSystemSubprocess<ParallelSystemT>;

        Timer m_timer;
        bool m_isAlive = true;
        
        IParallelSystem()
        {
            m_timer.setTargetFrameRate(80);
            m_timer.m_cyclic = true;
            m_timer.onUpdate->connect<&IParallelSystem::internalUpdate>(*this);
        }
        
        ~IParallelSystem()
        {
            stopThread();
            m_timer.onUpdate->disconnect<&IParallelSystem::internalUpdate>(*this);
        }

        void startThread() noexcept
        {
            m_isAlive = true;
            m_thread = std::thread([this]() {
                m_threadID = std::hash<std::thread::id>()(std::this_thread::get_id());
                
                while(m_isAlive)
                {
                    if(m_active)
                    {
                        m_timer.startFrame();
                    }
                }
            });
        }
        
        void fixedUpdate(const double& dt, const double& fixedDt) noexcept override
        {
            for(const auto& subprocess : m_subprocesses)
            {
                subprocess->fixedUpdate(dt, fixedDt, this->shared_from_this());
            }
        }

        void stopThread() noexcept
        {
            m_isAlive = false;
            m_thread.join();
        }
        
        template<typename SubprocessT>
        requires(std::is_base_of_v<subproc_t, SubprocessT>)
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
        
        void addSubprocess(const Ref<subproc_t>& subprocess) noexcept
        {
            std::lock_guard g(m_subprocessesVectorEditMutex);
            m_subprocesses.insert(subprocess);
        }
        
        void removeSubprocess(const Ref<subproc_t>& subprocess) noexcept
        {
            std::lock_guard g(m_subprocessesVectorEditMutex);
            m_subprocesses.erase(subprocess);
        }
        
        virtual void parallelUpdate(const double& dt, const double& fixedDt) noexcept { };

    private:
        void internalUpdate(const double& dt, const double& fixedDt) noexcept
        {
            auto t0 = now();
            parallelUpdate(dt, fixedDt);
            
            std::lock_guard g(m_subprocessesVectorEditMutex);
            for(const auto& subprocess : m_subprocesses)
            {
                subprocess->parallelUpdate(dt, fixedDt, this->shared_from_this());
            }
            auto t1 = now();
            
            m_executionTimes["parallelUpdate"] = timeDiff<double, std::milli>(t0, t1);
        }
        
        std::unordered_set<Ref<subproc_t>> m_subprocesses;
        std::mutex m_subprocessesVectorEditMutex;

        std::thread m_thread;
    };
}

#endif //SUNGEARENGINE_IPARALLELSYSTEM_H
