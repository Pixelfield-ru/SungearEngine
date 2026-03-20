//
// Created by stuka on 14.01.2025.
//

#pragma once

#include "IAction.h"

namespace SGCore
{
    /// Action that always returns 'false' from execute() function.
    struct SGCORE_EXPORT AlwaysFalseAction : IAction<bool()>
    {
        /// Always returns false.
        bool execute() noexcept final
        {
            return false;
        }

        Ref<IAction> copy() noexcept final
        {
            return MakeRef<AlwaysFalseAction>();
        }
    };
}
