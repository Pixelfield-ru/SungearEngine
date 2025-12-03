#pragma once
#include <functional>

#include "DynValueNode.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    template<typename T>
    struct FunctionDynValueNode final : DynValueNode<T>
    {
        std::function<T> f;

        copy_constructor(FunctionDynValueNode);
        move_constructor(FunctionDynValueNode);

        explicit FunctionDynValueNode(std::function<T> f) : f(std::move(f)) {};

        T getValue() override;
    };
}
