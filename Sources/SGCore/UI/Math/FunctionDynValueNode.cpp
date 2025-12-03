//
// Created by UserPC on 03.12.2025.
//

#include "FunctionDynValueNode.h"

template <typename T>
T SGCore::UI::FunctionDynValueNode<T>::getValue() {
    return f();
}