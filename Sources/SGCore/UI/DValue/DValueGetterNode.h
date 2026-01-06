#pragma once

namespace SGCore::UI::DValue
{
    /**
     * Node, where you can get value at any time
     * @tparam T Value type
     */
    template<typename T>
    struct DValueGetterNode
    {
        virtual ~DValueGetterNode() = default;
        virtual T getValue() = 0;
    };
}