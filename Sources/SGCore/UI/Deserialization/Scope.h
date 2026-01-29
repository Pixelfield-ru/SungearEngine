#pragma once
#include "SGCore/UI/Parser/UISourceTreeView.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI::Deserialization
{
    struct DeserScope
    {
        DeserScope* m_parentScope = nullptr; // Nullable

        // vars, that should be inlined (static expansion)
        std::optional<std::unordered_map<std::string_view, UISourceTreeViewValue&>> m_static = std::nullopt;

        // vars, that should be expanded dynamicly
        // TODO: map<name, DValueNodeOutput> or something like that

        explicit DeserScope(DeserScope* parentScope) : m_parentScope(parentScope) {}

        copy_constructor(DeserScope) {
            if (!other.isModified()) {
                m_parentScope = other.m_parentScope;
            } else {
                m_parentScope = const_cast<DeserScope*>(&other);
            }
        };
        move_constructor(DeserScope) = default;

        std::optional<std::reference_wrapper<UISourceTreeViewValue>> tryGetStatic(std::string_view key);

    private:
        /**
         * Checks, if that scope was modified. Modified scope is any scope, that provides more info than a parent scope of that scope
         * @return true, if scope is modified
         */
        [[nodiscard]] bool isModified() const;
    };
}
