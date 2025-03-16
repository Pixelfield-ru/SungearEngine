//
// Created by stuka on 16.03.2025.
//

#ifndef SUNGEARENGINE_UITEXT_H
#define SUNGEARENGINE_UITEXT_H

#include "SGCore/Memory/Assets/FontSpecialization.h"
#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct Text : UIElement
    {
        Text() noexcept;

        copy_constructor(Text) = default;
        move_constructor(Text) = default;

        copy_operator(Text) = default;
        move_operator(Text) = default;

        std::u16string m_text;

        bool draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                  const Transform& elementTransform,
                  UIElementCache& elementCache) noexcept override;

    protected:
        void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMeshBaseSelector(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;

        void doGenerateBasicMesh() noexcept final;
    };
}

#endif // SUNGEARENGINE_UITEXT_H
