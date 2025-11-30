//
// Created by stuka on 18.01.2025.
//

#ifndef SUNGEARENGINE_CSSSELECTOR_H
#define SUNGEARENGINE_CSSSELECTOR_H

#include <vector>

#include "../Parser/XML/CSS/PropertiesAlternativeValues/BorderRadiusAlternativeValue.h"
#include "SGCore/UI/TransformTree/UIElementCache.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/UI/Parser/XML/CSS/PropertyValues/CSSPropertyValue.h"

#include "../Parser/XML/CSS/PropertyValues/CSSPropertyValueKeywords.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/Font.h"

#include "SGCore/UI/Math/CSSMathNode.h"
#include "SGCore/Utils/Macroses.h"

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

        DisplayKeyword m_display = DisplayKeyword::KW_FLEX;
        FlexboxKeyword m_flexDirection = FlexboxKeyword::KW_ROW;
        FlexboxKeyword m_flexWrap = FlexboxKeyword::KW_NOWRAP;

        PropertyValue<PositionAndSizeKeyword::KW_AUTO, Ref<StyleMathNode>> m_width;
        PropertyValue<PositionAndSizeKeyword::KW_AUTO, Ref<StyleMathNode>> m_height;

        /// top, right, bottom, left
        PropertyValue<UniversalKeyword::KW_UNSET, Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>> m_padding;

        PropertyValue<PositionAndSizeKeyword::KW_UNSET, Ref<StyleMathNode>, Ref<StyleMathNode>> m_gap;

        PropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_bottomLeftBorderRadius;
        PropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_topLeftBorderRadius;
        PropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_topRightBorderRadius;
        PropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_bottomRightBorderRadius;

        PropertyValue<ColorKeyword::KW_TRANSPARENT, Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>, Ref<StyleMathNode>> m_backgroundColor;

        AssetWeakRef<Font> m_font;

        /// Used as scale.
        PropertyValue<FontSizeKeyword::KW_UNSET, Ref<StyleMathNode>> m_fontSize;

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

#endif //SUNGEARENGINE_CSSSELECTOR_H
