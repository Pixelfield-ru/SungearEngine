//
// Created by stuka on 28.01.2025.
//

#pragma once

#include <vector>

#include "SGCore/Transformations/Transform.h"
#include "UIElementCache.h"

namespace SGCore::UI
{
    struct UIElement;
    struct UITransformTree;
    struct CSSStyle;

    struct UITransformTreeElement
    {
        friend struct UITransformTree;

        Transform m_transform;

        UIElementCache m_elementLastCache;
        UIElementCache m_elementCurrentCache;

        std::vector<std::int64_t> m_children;
        std::int64_t m_parent = -1;

    private:
        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, UITransformTreeElement*, UITransformTreeElement&, UIElement*, UIElement&>)
        void iterateImpl(FuncT&& func, UITransformTree& transformTree, UITransformTreeElement* parentTreeElement, UIElement* parentUIElement, UIElement& thisUIElement, bool& breakToken) noexcept
        {
            if(breakToken) return;

            func(parentTreeElement, *this, parentUIElement, thisUIElement);

            for(size_t i = 0; i < Impl::getUIElementChildrenCount(thisUIElement) && i < m_children.size(); ++i)
            {
                auto& childTreeElement = Impl::getElementFromTransformTree(transformTree, m_children[i]);
                auto& currentUIElement = Impl::getUIElementChild(thisUIElement, i);

                childTreeElement.iterateImpl(func, transformTree, this, &thisUIElement, currentUIElement, breakToken);

                if(breakToken) return;
            }
        }

        struct Impl
        {
            static UITransformTreeElement& getElementFromTransformTree(UITransformTree& transformTree, std::int64_t index) noexcept;
            static size_t getTransformTreeSize(const UITransformTree& transformTree) noexcept;
            static size_t getUIElementChildrenCount(const UIElement& element) noexcept;
            static UIElement& getUIElementChild(const UIElement& element, size_t index) noexcept;
        };
    };
}
