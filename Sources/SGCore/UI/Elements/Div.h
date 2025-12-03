#pragma once

#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct Div : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(div)

        Div() noexcept;

        copy_constructor(Div) = default;
        move_constructor(Div) = default;

        [[nodiscard]] Ref<UIElement> copy() const noexcept override;

        copy_operator(Div) = default;
        move_operator(Div) = default;

    protected:
        void doCalculateLayout(const UIElementCache* parentSelectorCache, UIElementCache& thisSelectorCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };
}