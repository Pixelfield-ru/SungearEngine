//
// Created by stuka on 28.01.2025.
//

#ifndef SUNGEARENGINE_UITRANSFORMTREE_H
#define SUNGEARENGINE_UITRANSFORMTREE_H

#include "UITransformTreeElement.h"

namespace SGCore::UI
{
    struct UITransformTree
    {
        UITransformTreeElement& getRoot() noexcept
        {
            return m_elements[0];
        }

        /// Guaranteed that m_elements[0] is root element.
        std::vector<UITransformTreeElement> m_elements;
    };
}

#endif //SUNGEARENGINE_UITRANSFORMTREE_H
