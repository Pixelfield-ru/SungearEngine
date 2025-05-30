//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_IACTION_H
#define SUNGEARENGINE_IACTION_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    /**
     * It is just wrapper of functions.
     * @tparam ReturnT
     */
    template<typename ReturnT, typename... ExecuteArgs>
    struct IAction;

    template<typename ReturnT, typename... ExecuteArgs>
    struct IAction<ReturnT(ExecuteArgs...)>
    {
        virtual ~IAction() = default;

        virtual ReturnT execute(const ExecuteArgs&... args) = 0;

        virtual Ref<IAction> copy() = 0;
    };
}

#endif //SUNGEARENGINE_IACTION_H
