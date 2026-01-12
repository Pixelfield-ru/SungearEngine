#pragma once
#include "DValueNode.h"

namespace SGCore::UI::DValue
{
    /**
     * Caches value when it received
     * @tparam T value type
     */
    template<typename T>
    struct DValueCacheNode
    {
        T m_value;

        copy_constructor(DValueCacheNode);
        move_constructor(DValueCacheNode);

        DValueNodeInput<T> m_input {[this] (T val) {m_value = val;}};
    };
}
