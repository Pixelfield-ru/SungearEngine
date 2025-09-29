//
// Created by stuka on 23.01.2025.
//

#ifndef SUNGEARENGINE_UIDIV_H
#define SUNGEARENGINE_UIDIV_H

#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct Div : UIElement
    {
        Div() noexcept;

        copy_constructor(Div) = default;
        move_constructor(Div) = default;

        copy_operator(Div) = default;
        move_operator(Div) = default;

    protected:
        void doCalculateLayout(const UIElementCache* parentSelectorCache, UIElementCache& thisSelectorCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };
}

#endif //SUNGEARENGINE_UIDIV_H
