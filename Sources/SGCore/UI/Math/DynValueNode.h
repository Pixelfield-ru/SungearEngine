#pragma once
#include <vector>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI
{
    struct Style;

    template<typename T>
    struct DynValueNode
    {
        DynValueNode() = delete;

        virtual ~DynValueNode() = default;

        virtual T getValue();
    };
}