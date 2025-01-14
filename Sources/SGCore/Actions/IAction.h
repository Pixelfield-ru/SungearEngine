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
    template<typename ReturnT>
    struct IAction
    {
        virtual ReturnT execute() = 0;

        virtual Ref<IAction<ReturnT>> copy() = 0;
    };
}

#endif //SUNGEARENGINE_IACTION_H
