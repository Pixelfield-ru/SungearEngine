//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_UIELEMENT_H
#define SUNGEARENGINE_UIELEMENT_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Transformations/Transform.h"
#include "CSS/CSSSelector.h"
#include "SGCore/Memory/AssetRef.h"

namespace SGCore::UI
{
    struct UIElement
    {
        std::vector<Ref<UIElement>> m_children;
        Weak<UIElement> m_parent;

        virtual void render() noexcept = 0;

    private:
        Transform m_transform;

        AssetRef<CSSSelector> m_selector;
    };
}

#endif //SUNGEARENGINE_UIELEMENT_H
