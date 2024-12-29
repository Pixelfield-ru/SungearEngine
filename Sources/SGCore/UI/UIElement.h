//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_UIELEMENT_H
#define SUNGEARENGINE_UIELEMENT_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreMain.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct UIElement : ECS::Component<UIElement, const UIElement>
    {
        std::vector<ECS::entity_t> m_children;
    };
}

#endif //SUNGEARENGINE_UIELEMENT_H
