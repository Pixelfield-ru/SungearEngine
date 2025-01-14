//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_ALWAYSTRUEACTION_H
#define SUNGEARENGINE_ALWAYSTRUEACTION_H

#include "IAction.h"

namespace SGCore
{
    struct AlwaysTrueAction : IAction<bool>
    {
        bool execute() noexcept final
        {
            return true;
        }

        Ref<IAction<bool>> copy() noexcept final
        {
            return MakeRef<AlwaysTrueAction>();
        }
    };
}

#endif //SUNGEARENGINE_ALWAYSTRUEACTION_H
