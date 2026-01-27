#ifndef DIV_H
#define DIV_H

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

        [[nodiscard]] Scope<UIElement> copy() const noexcept override;

        copy_operator(Div) = default;
        move_operator(Div) = default;

        SG_IMPL_DESERIALIZABLE(Div);

    protected:
        void doCalculateLayout(const UIElementCache* parentSelectorCache, UIElementCache& thisSelectorCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;
    };

#define sg_deser_type Div
#define sg_deser_base UIElement
#include <SGCore/UI/Deserialization/ImplDeserializableStruct.h>
}
#endif // DIV_H
