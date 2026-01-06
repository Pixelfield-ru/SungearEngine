#pragma once
#include <boost/type.hpp>

#include "DValueSetterNode.h"

namespace SGCore::UI::DValue
{
    template<typename T, typename FirstChildrenNode = DValueSetterNode<T>&, typename... TailChildrenNodes>
    struct DValueForwardSetterNode final : DValueSetterNode<T>
    {
        FirstChildrenNode m_childrenNode;
        std::tuple<TailChildrenNodes...> m_otherChildren;

        ~DValueForwardSetterNode() override = default;

        explicit(false) DValueForwardSetterNode(FirstChildrenNode& node) : DValueSetterNode<T>(node), m_childrenNode(node) {}
        explicit(false) DValueForwardSetterNode(const FirstChildrenNode&& node) : DValueSetterNode<T>(node), m_childrenNode(std::move(node)) {}

        void setValue(T value) override {
            m_childrenNode.setValue(value);
            std::apply([value](auto&&... arg) {((arg.setValue(value)), ...);}, m_otherChildren);
        }
    };
}
