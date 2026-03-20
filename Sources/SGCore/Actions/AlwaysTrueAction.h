//
// Created by stuka on 14.01.2025.
//

#pragma once

#include <sgcore_export.h>

#include "IAction.h"

namespace SGCore
{
    /// Action that always returns 'true' from execute() function.
    struct SGCORE_EXPORT AlwaysTrueAction : IAction<bool()>
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
