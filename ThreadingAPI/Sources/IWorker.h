//
// Created by ilya on 03.04.24.
//

#ifndef THREADINGAPI_IWORKER_H
#define THREADINGAPI_IWORKER_H

#include "Event.h"

namespace SGCore
{
    struct IWorker : public std::enable_shared_from_this<IWorker>
    {
        friend struct Thread;
        
        template<auto F>
        void setExecutableFunction()
        {
            const auto fnPtr = F;
            const size_t hash = std::hash<const char*>()(static_cast<const char*>(static_cast<const void*>(&fnPtr)));
            
            m_onExecuteListener->m_hash = hash;
            
            std::cout << "hash1: " << hash << std::endl;
            
            m_executableFunction = F;
        }
        
        std::function<void(std::shared_ptr<IWorker> worker)> m_onExecutedFunction;
        
    private:
        std::function<void()> m_executableFunction;
        
        EventListener<void()> m_onExecuteListener = MakeEventListener<void()>([this]() {
            if(m_executableFunction)
            {
                m_executableFunction();
            }
            if(m_onExecutedFunction)
            {
                m_onExecutedFunction(shared_from_this());
            }
        });
    };
}

#endif //THREADINGAPI_IWORKER_H
