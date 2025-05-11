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
        friend struct FontSpecializationRenderer;
        friend struct UILayoutCalculator;

        Text() noexcept;

        copy_constructor(Text) = default;
        move_constructor(Text) = default;

        copy_operator(Text) = default;
        move_operator(Text) = default;

        std::u32string m_text;

        bool draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                  const Transform& elementTransform,
                  UIElementCache& elementCache) noexcept override;

        const FontGlyph* getGlyph(std::size_t glyphIndex) const noexcept;
        size_t getGlyphsCount() const noexcept;

        void clearGlyphs() noexcept;

    protected:


        /// todo: omg i have element size (m_basicSize) in selector but i cant use it because of order. i am calculating m_basicSize in draw function of struct Text and then i am calling m_selector->calculateCache() in UILayoutCalculator so m_basicSize resets
        /// todo: so i need to hold this variable
        glm::vec2 m_textSize { };
        std::vector<const FontGlyph*> m_glyphs;

        /// Used when whole text does not fit in parent element.
        /// Every element in this vector represents if glyph at index I in m_glyphs will be moved to new line.
        std::vector<bool> m_lineBreaks;

        void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMeshBaseSelector(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;

        void doGenerateBasicMesh() noexcept final;
    };
}

#endif // SUNGEARENGINE_UITEXT_H
