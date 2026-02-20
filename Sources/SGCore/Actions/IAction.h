//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_IACTION_H
#define SUNGEARENGINE_IACTION_H

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    template<typename ReturnT, typename... ExecuteArgs>
    struct IAction;

    /**
     * Polymorphic wrapper for functions.\n
     * Use if you want to make savable polymorphic functions.
     * @tparam ReturnT Return type of function.
     * @tparam ExecuteArgs Arguments of function.
     */
    template<typename ReturnT, typename... ExecuteArgs>
    struct IAction<ReturnT(ExecuteArgs...)>
    {
        virtual ~IAction() = default;

        /**
         * Function. Executes some code and returns result with type 'ReturnT'
         * @param args Arguments of function.
         * @return Return of function.
         */
        virtual ReturnT execute(const ExecuteArgs&... args) = 0;

        /**
         * @return Copy of Action with same type of instance.
         */
        virtual Ref<IAction> copy() = 0;
    };
}

#endif //SUNGEARENGINE_IACTION_H
