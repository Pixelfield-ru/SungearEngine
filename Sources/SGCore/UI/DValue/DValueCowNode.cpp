#include "DValueCowNode.h"

template <typename T>
T& SGCore::UI::DValue::DValueCowNode<T>::getValue() {
    if (auto val = std::get_if<T>(m_value)) {
        return *val;
    }
    return std::get<DValueCacheNode<T>>(m_value).m_value;
}
