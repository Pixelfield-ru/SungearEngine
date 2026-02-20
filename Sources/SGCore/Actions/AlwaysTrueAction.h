//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_ALWAYSTRUEACTION_H
#define SUNGEARENGINE_ALWAYSTRUEACTION_H

#include "IAction.h"

namespace SGCore
{
    /// Action that always returns 'true' from execute() function.
    struct AlwaysTrueAction : IAction<bool()>
    {
        /// Always returns true.
        bool execute() noexcept final
        {
            return true;
        }

        Ref<IAction> copy() noexcept final
        {
            return MakeRef<AlwaysTrueAction>();
        }
    };
}

#endif //SUNGEARENGINE_ALWAYSTRUEACTION_H
