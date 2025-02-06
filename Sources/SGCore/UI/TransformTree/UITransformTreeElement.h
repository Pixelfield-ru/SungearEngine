//
// Created by stuka on 28.01.2025.
//

#ifndef SUNGEARENGINE_UITRANSFORMTREEELEMENT_H
#define SUNGEARENGINE_UITRANSFORMTREEELEMENT_H

#include <vector>

#include "SGCore/Transformations/Transform.h"

namespace SGCore::UI
{
    struct UITransformTreeElement
    {
        Transform m_transform;

        std::vector<std::int64_t> m_children;
        std::int64_t m_parent = -1;
    };
}

#endif //SUNGEARENGINE_UITRANSFORMTREEELEMENT_H
