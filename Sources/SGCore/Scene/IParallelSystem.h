//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_IPARALLELSYSTEM_H
#define SUNGEARENGINE_IPARALLELSYSTEM_H

#include <SGCore/Threading/Thread.h>

#include "ISystem.h"
#include "SGCore/Utils/Timer.h"
#include "IParallelSystemSubprocess.h"
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    template<typename ParallelSystemT>
    struct IParallelSystem : ISystem
    {
        using subproc_t = IParallelSystemSubprocess<ParallelSystemT>;

        Timer m_timer;
        
        IParallelSystem()
        {
            m_thread = Threading::Thread::create();
            
            m_timer.setTargetFrameRate(80);
            m_timer.m_cyclic = true;
            m_timer.onUpdate += m_internalUpdateListener;

            m_thread->onUpdate += m_threadUpdateListener;
        }
        
        ~IParallelSystem() override
        {
            std::printf("deleting parallel system: %llu\n", std::hash<size_t>()((size_t) this));
            // m_timer.onUpdate.disconnect<&IParallelSystem::internalUpdate>(*this);
            m_timer.onUpdate -= m_internalUpdateListener;
        }
        
        void fixedUpdate(const double& dt, const double& fixedDt) noexcept override
        {
            for(const auto& subprocess : m_subprocesses)
            {
                subprocess->fixedUpdate(dt, fixedDt, static_cast<ParallelSystemT*>(this));
            }
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
                subprocess->parallelUpdate(dt, fixedDt, static_cast<ParallelSystemT*>(this));
            }
            auto t1 = now();
            
            m_executionTimes["parallelUpdate"] = timeDiff<double, std::milli>(t0, t1);
        }

        Slot<void(const double&, const double&)> m_internalUpdateListener = [this](const double& dt, const double& fixedDt) {
            internalUpdate(dt, fixedDt);
        };

        Slot<void()> m_threadUpdateListener = [this]() {
            if(m_active)
            {
                m_timer.startFrame();
            }
        };

        // TODO: MAKE SAVING FOR THIS MEMBER
        std::unordered_set<Ref<subproc_t>> m_subprocesses;
        std::mutex m_subprocessesVectorEditMutex;
    };
}

#endif //SUNGEARENGINE_IPARALLELSYSTEM_H
