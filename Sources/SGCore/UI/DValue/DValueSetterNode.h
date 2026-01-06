#pragma once
#include <vector>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore::UI::DValue
{
    struct Style;

    /**
     * Node, that can be updated by new value at any time
     * @tparam T Value type
     */
    template<typename T>
    struct DValueSetterNode
    {
        DValueSetterNode() = delete;

        virtual ~DValueSetterNode() = default;

        /**
         * When called value of **this** node is changed
         * @param value new value
         */
        virtual void setValue(T value) = 0;
    };
}