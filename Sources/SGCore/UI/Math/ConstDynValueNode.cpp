#include "ConstDynValueNode.h"

template <typename T>
T SGCore::UI::ConstDynValueNode<T>::getValue() {
    return value;
}