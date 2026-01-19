//
// Created by stuka on 28.01.2025.
//

#pragma once

#include "UITransformTreeElement.h"
#include "../Elements/UIRoot.h"

namespace SGCore::UI
{
    struct UIDocument;
    struct UIRoot;

    struct UITransformTree
    {
        UITransformTreeElement& getRoot() noexcept
        {
            return m_elements[0];
        }

        /// Guaranteed that m_elements[0] is root element.
        std::vector<UITransformTreeElement> m_elements;

        template<typename FuncT>
        requires(std::is_invocable_v<FuncT, UITransformTreeElement*, UITransformTreeElement&, UIElement*, UIElement&>)
        void iterate(FuncT&& func, UIDocument& inDocument, bool& breakToken) noexcept
        {
            if(m_elements.empty()) return;

            m_elements[0].iterateImpl(func, *this, nullptr, nullptr, Impl::getRootFromDocument(inDocument), breakToken);
        }

    private:
        struct Impl
        {
            static UIRoot& getRootFromDocument(const UIDocument& inDocument) noexcept;
        };
    };
}
