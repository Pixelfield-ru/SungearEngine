#pragma once
#include <boost/type.hpp>

#include "DValueGetterNode.h"
#include "DValueSetterNode.h"

namespace SGCore::UI::DValue
{
    /**
     * Caches value when it changes and
     * @tparam T
     */
    template<typename T>
    struct DValueCacheNode final : DValueSetterNode<T>, DValueGetterNode<T&>
    {
        T m_value;
        ~DValueCacheNode() override = default;

        T& getValue() override {return m_value;}
        void setValue(T value) override {
            m_value = value;
        }
    };

    template<typename T, typename ChildrenNode = DValueSetterNode<T&>&>
    struct DValueCacheForwardSetterNode final : DValueSetterNode<T>, DValueGetterNode<T&>
    {
        T m_value;
        ChildrenNode m_childrenNode;

        explicit(false) DValueCacheForwardSetterNode(ChildrenNode& node) : DValueSetterNode<T>(node), m_childrenNode(node) {}
        explicit(false) DValueCacheForwardSetterNode(const ChildrenNode&& node) : DValueSetterNode<T>(node), m_childrenNode(std::move(node)) {}

        T& getValue() override {return m_value;}
        void setValue(T value) override {
            m_value = value;
            m_childrenNode.setValue(m_value);
        }
    };
}
