#pragma once

#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"
#include "../Deserialization/MetaDefenition.h"

namespace SGCore::UI
{
    struct Div : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(div)

        Div() noexcept;

        copy_constructor(Div) = default;
        move_constructor(Div) = default;

        [[nodiscard]] Scope<UIElement> copy() const noexcept override;

        copy_operator(Div) = delete;
        move_operator(Div) = delete;

    protected:
        void doCalculateLayout(const UIElementCache* parentSelectorCache, UIElementCache& thisSelectorCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };

    template<>
    struct Deserialization::MetaDef<Div>
    {
        static inline auto base_types = types_container<TypeWrapper<UIElement>>{};
        static inline auto properties_fields = std::tuple {};
    };
}