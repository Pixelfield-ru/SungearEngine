//
// Created by stuka on 25.01.2025.
//

#ifndef SUNGEARENGINE_ROOT_H
#define SUNGEARENGINE_ROOT_H

#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct UIRoot : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(xml)

        UIRoot() noexcept;

        copy_constructor(UIRoot) = default;
        move_constructor(UIRoot) = default;

        copy_operator(UIRoot) = default;
        move_operator(UIRoot) = default;

    protected:
        void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };
}

#endif //SUNGEARENGINE_ROOT_H
