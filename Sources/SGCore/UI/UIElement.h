//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_UIELEMENT_H
#define SUNGEARENGINE_UIELEMENT_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreMain.h"

namespace SGCore
{
    struct UIElement
    {
        std::vector<entt::entity> m_children;
    };
}

#endif //SUNGEARENGINE_UIELEMENT_H
