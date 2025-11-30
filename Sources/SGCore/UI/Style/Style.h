#pragma once

#include <vector>

#include "../Parser/XML/CSS/PropertiesAlternativeValues/BorderRadiusAlternativeValue.h"
#include "SGCore/UI/TransformTree/UIElementCache.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "EnumProperties.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/Font.h"

#include "SGCore/UI/Math/CSSMathNode.h"
#include "SGCore/Utils/Macroses.h"
#include "StyleProperty.h"

namespace SGCore::UI
{
    struct Style : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(Style, 34)

        friend struct ANTLRCSSListener;
        friend struct CSSFile;

        Style() noexcept;

        copy_constructor(Style) = default;
        move_constructor(Style) = default;

        copy_operator(Style) = default;
        move_operator(Style) = default;

#pragma region Properties
        // ============================================================== properties

// iterate through all style properties names
#define style_properties(prop) \
    prop(m_display, display, display) \
    prop(m_flexDirection, flex-direction, flexDirection) \
    prop(m_flexWrap, flex-wrap, flexWrap) \
    prop(m_width, width, width) \
    prop(m_height, height, height) \
    prop(m_paddingLeft, padding-left, paddingLeft) \
    prop(m_paddingRight, padding-right, paddingRight) \
    prop(m_paddingBottom, padding-bottom, paddingBottom) \
    prop(m_paddingTop, padding-top, paddingTop) \
    prop(m_padding, padding, padding) \
    prop(m_rowGap, row-gap, rowGap) \
    prop(m_collumnGap, collumn-gap, collumnGap) \
    prop(m_gap, gap, gap) \
    prop(m_bottomLeftBorderRadius, bottom-left-border-radius, bottomLeftBorderRadius) \
    prop(m_topLeftBorderRadius, top-left-border-radius, topLeftBorderRadius) \
    prop(m_topRightBorderRadius, top-right-border-radius, topRightBorderRadius) \
    prop(m_bottomRightBorderRadius, bottom-right-border-radius, bottomRightBorderRadius) \
    prop(m_borderRadius, border-radius, borderRadius) \
    prop(m_backgroundColor, background-color, backgroundColor) \
    prop(m_fontSize, font-size, fontSize)
    

        DisplayKeyword m_display = DisplayKeyword::FLEX;
        FlexboxKeyword m_flexDirection = FlexboxKeyword::ROW;
        FlexboxKeyword m_flexWrap = FlexboxKeyword::NOWRAP;

        using SizeStyleProperty = std::variant<PositionAndSizeKeyword, Ref<StyleMathNode>>;
        SizeStyleProperty m_width = PositionAndSizeKeyword::UNSET;
        SizeStyleProperty m_height = PositionAndSizeKeyword::UNSET;

        using PaddingStyleProperty = std::variant<UniversalKeyword, Ref<StyleMathNode>>;
        PaddingStyleProperty m_paddingLeft = UniversalKeyword::UNSET;
        PaddingStyleProperty m_paddingRight = UniversalKeyword::UNSET;
        PaddingStyleProperty m_paddingBottom = UniversalKeyword::UNSET;
        PaddingStyleProperty m_paddingTop = UniversalKeyword::UNSET;

        std::variant<
            std::tuple<
                RefStyleProperty<&Style::m_paddingLeft>,
                RefStyleProperty<&Style::m_paddingRight>,
                RefStyleProperty<&Style::m_paddingTop>,
                RefStyleProperty<&Style::m_paddingBottom>
            >,
            std::tuple<
                RefStyleProperty<&Style::m_paddingLeft, &Style::m_paddingRight>, 
                RefStyleProperty<&Style::m_paddingTop, &Style::m_paddingBottom>
            >,
            RefStyleProperty<&Style::m_paddingLeft, &Style::m_paddingRight, &Style::m_paddingTop, &Style::m_paddingBottom>
        > m_padding;

        
        using GapStyleProperty = SizeStyleProperty;
        GapStyleProperty m_rowGap = PositionAndSizeKeyword::UNSET;
        GapStyleProperty m_collumnGap = PositionAndSizeKeyword::UNSET;
        
        RefStyleProperty<&Style::m_rowGap, &Style::m_collumnGap> m_gap;

        struct BorderRadius final
        {
            Ref<StyleMathNode> m_radiusX;
            Ref<StyleMathNode> m_radiusY;
        };

        using BorderRadiusStyleProperty = std::variant<UniversalKeyword, BorderRadius>;
        BorderRadiusStyleProperty m_bottomLeftBorderRadius = UniversalKeyword::UNSET;
        BorderRadiusStyleProperty m_topLeftBorderRadius = UniversalKeyword::UNSET;
        BorderRadiusStyleProperty m_topRightBorderRadius = UniversalKeyword::UNSET;
        BorderRadiusStyleProperty m_bottomRightBorderRadius = UniversalKeyword::UNSET;

        std::variant<
            std::tuple<
                RefStyleProperty<&Style::m_bottomLeftBorderRadius>, 
                RefStyleProperty<&Style::m_bottomRightBorderRadius>, 
                RefStyleProperty<&Style::m_topLeftBorderRadius>,
                RefStyleProperty<&Style::m_topRightBorderRadius>
            >,
            RefStyleProperty<&Style::m_bottomLeftBorderRadius, &Style::m_bottomRightBorderRadius, &Style::m_topLeftBorderRadius, &Style::m_topRightBorderRadius>
        > m_borderRadius;

        std::variant<
            ColorKeyword, 
            std::tuple<Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>>, // rgba
            std::tuple<Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>>, // rgb
        > m_backgroundColor = ColorKeyword::UNSET;

        AssetWeakRef<Font> m_font;

        /// Used as scale.
        std::variant<FontSizeKeyword, Ref<StyleMathNode>> m_fontSize;

        // ==============================================================
#pragma endregion Properties

        // first - hash of pseudo class name. second - style
        std::unordered_map<size_t, AssetRef<Style>> m_pseudoClassesStyles;

        [[nodiscard]] const std::string& getSelector() const noexcept;
        [[nodiscard]] const std::string& getPseudoName() const noexcept;

        [[nodiscard]] size_t getSelectorHash() const noexcept;
        [[nodiscard]] size_t getPseudoClassHash() const noexcept;

        void calculateCache(const UIElementCache* parentElementCache,
                            UIElementCache& thisElementCache) noexcept;

        void setFontSpecializationSettings(const FontSpecializationSettings& settings) noexcept;
        const FontSpecializationSettings& getFontSpecializationSettings() const noexcept;

        void updateFontSettings() noexcept;

        Ref<FontSpecialization> getFontSpecialization() const noexcept;

        void copy(Style& to) const noexcept;

    protected:
        // todo: maybe vector with pseudo class handlers
        // std::vector<Ref<CSSPseudoClassHandler>> m_pseudoClassesHandlers;
        // we will pass current UIElement to handler of pseudo class and check if condition is true in virtual function.
        // if condition is true, then we must take pseudo class name from CSSPseudoClassHandler that returns true and then
        // pass current UIElement to apply(...) function of CSSPseudoClassHandler to do pseudo-class-specific actions

        /// NOT SUPPORTED
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // NOT SUPPORTED
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

    private:
        FontSpecializationSettings m_fontSpecializationSettings;
        Weak<FontSpecialization> m_fontSpecialization;

        std::string m_selector;
        std::string m_pseudoClass;

        mutable size_t m_selectorHash = 0;
        mutable size_t m_pseudoClassHash = 0;
    };
}