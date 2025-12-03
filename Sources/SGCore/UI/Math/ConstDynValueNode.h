#pragma once
#include "DynValueNode.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    // TODO: lazy update support (onValueChange signal)
    template<typename T>
    struct ConstDynValueNode final : public DynValueNode<T>
    {
        T value;
        copy_constructor(ConstDynValueNode);
        move_constructor(ConstDynValueNode);

        explicit ConstDynValueNode(T value) : value(value) {};

        T getValue() override;

        // TODO: implicit conversion T -> value & value -> T?
    };
}

