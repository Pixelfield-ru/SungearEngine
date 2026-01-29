#include "Scope.h"

std::optional<std::reference_wrapper<SGCore::UI::UISourceTreeViewValue>> SGCore::UI::Deserialization::DeserScope::
tryGetStatic(std::string_view key) {
    if (m_static) {
        auto value = m_static->find(key);
        if (value != m_static->end()) {
            return value->second;
        }
    }

    if (m_parentScope != nullptr) {
        return m_parentScope->tryGetStatic(key);
    }

    return std::nullopt;
}

bool SGCore::UI::Deserialization::DeserScope::isModified() const {
    return m_static.has_value();
}
