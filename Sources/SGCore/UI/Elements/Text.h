#pragma once

#include "SGCore/Memory/Assets/FontSpecialization.h"
#include "SGCore/UI/AttributeValue.h"
#include "SGCore/UI/UIElement.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct Text : UIElement
    {
        SG_DECLARE_UI_ELEMENT_TYPE(text)

        friend struct FontSpecializationRenderer;
        friend struct UILayoutCalculator;

        Text() noexcept;

        copy_constructor(Text) = default;
        move_constructor(Text) = default;

        copy_operator(Text) = default;
        move_operator(Text) = default;

        std::string m_text;

        bool draw(const LayeredFrameReceiver::reg_t& cameraReceiver,
                  const Transform& elementTransform,
                  UIElementCache& elementCache) noexcept override;

        const FontGlyph* getGlyph(std::size_t glyphIndex) const noexcept;
        size_t getGlyphsCount() const noexcept;

        void clearGlyphs() noexcept;

        [[nodiscard]] Scope<UIElement> copy() const noexcept override;

        static inline auto properties_fields = std::tuple {};

    protected:
        /// todo: omg i have element size (m_basicSize) in selector but i cant use it because of order. i am calculating m_basicSize in draw function of struct Text and then i am calling m_style->calculateCache() in UILayoutCalculator so m_basicSize resets
        /// todo: so i need to hold this variable
        glm::vec2 m_textSize { };
        std::vector<const FontGlyph*> m_glyphs;

        /// Used when whole text does not fit in parent element.
        /// Every element in this vector represents if glyph at index I in m_glyphs will be moved to new line.
        std::vector<bool> m_lineBreaks;

        void doCalculateLayout(const UIElementCache* parentElementCache, UIElementCache& thisElementCache,
                               const Transform* parentTransform, Transform& ownTransform) noexcept final;

        void doGenerateMesh(const UIElementCache* parentElementCache, UIElementCache& thisElementCache) noexcept final;

        void doCopy(UIElement& to) const noexcept override;
    };

/*#define sg_deser_type Text
#define sg_deser_base UIElement
#include <SGCore/UI/Deserialization/ImplDeserializableStruct.h>*/
}
