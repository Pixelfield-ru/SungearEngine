#pragma once

#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct UIRoot : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(root)

        UIRoot() noexcept;

        copy_constructor(UIRoot) = default;
        move_constructor(UIRoot) = default;

        [[nodiscard]] Ref<UIElement> copy() const noexcept override;

        copy_operator(UIRoot) = default;
        move_operator(UIRoot) = default;

        static UIRoot createFrom();

    protected:
        void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };
}
