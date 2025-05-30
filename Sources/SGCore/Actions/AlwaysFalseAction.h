//
// Created by stuka on 14.01.2025.
//

#ifndef SUNGEARENGINE_ALWAYSFALSEACTION_H
#define SUNGEARENGINE_ALWAYSFALSEACTION_H

#include "IAction.h"

namespace SGCore
{
    struct AlwaysFalseAction : IAction<bool()>
    {
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

#endif //SUNGEARENGINE_ALWAYSFALSEACTION_H
