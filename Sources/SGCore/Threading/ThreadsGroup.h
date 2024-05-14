//
// Created by ilya on 15.05.24.
//

#ifndef SUNGEARENGINE_THREADSGROUP_H
#define SUNGEARENGINE_THREADSGROUP_H

#include "ThreadsPool.h"

namespace SGCore::Threading
{
    // MAYBE NOT IMPL

    template<typename SelectionPolicyT>
    requires(std::is_base_of_v<ThreadsGroupSelectionPolicy, SelectionPolicyT>)
    struct BaseThreadsGroup
    {
    
    };
}

#endif // SUNGEARENGINE_THREADSGROUP_H
