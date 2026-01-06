#pragma once
#include "DValueSetterNode.h"
#include "SGCore/Utils/Signal.h"

namespace SGCore::UI::DValue
{
    /**
     * Calls @ref onValueChanged signal when value is changed
     * @tparam T Value type
     */
    template<typename T>
    struct DValueSignalForwardSetterNode final : DValueSetterNode<T>
    {
        Signal<void(T)> onValueChanged {};

        void setValue(T value) override {
            onValueChanged.Call(value);
        }
    };
}
