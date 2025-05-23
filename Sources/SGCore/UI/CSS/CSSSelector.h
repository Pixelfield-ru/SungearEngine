//
// Created by stuka on 18.01.2025.
//

#ifndef SUNGEARENGINE_CSSSELECTOR_H
#define SUNGEARENGINE_CSSSELECTOR_H

#include <vector>

#include "PropertiesAlternativeValues/BorderRadiusAlternativeValue.h"
#include "SGCore/UI/TransformTree/UIElementCache.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/UI/CSS/PropertyValues/CSSPropertyValue.h"

#include "PropertyValues/CSSPropertyValueKeywords.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/Font.h"

#include "SGCore/UI/CSS/Math/CSSMathNode.h"
#include "SGCore/Utils/Macroses.h"

namespace SGCore::UI
{
    struct CSSSelector : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(CSSSelector, 34)

        friend struct ANTLRCSSListener;
        friend struct CSSFile;

        CSSSelector() noexcept;

        copy_constructor(CSSSelector) = default;
        move_constructor(CSSSelector) = default;

        copy_operator(CSSSelector) = default;
        move_operator(CSSSelector) = default;

        DisplayKeyword m_display = DisplayKeyword::KW_FLEX;
        FlexboxKeyword m_flexDirection = FlexboxKeyword::KW_ROW;
        FlexboxKeyword m_flexWrap = FlexboxKeyword::KW_NOWRAP;

        CSSPropertyValue<PositionAndSizeKeyword::KW_AUTO, Ref<CSSMathNode>> m_width;
        CSSPropertyValue<PositionAndSizeKeyword::KW_AUTO, Ref<CSSMathNode>> m_height;

        /// top, right, bottom, left
        CSSPropertyValue<UniversalKeyword::KW_UNSET, Ref<CSSMathNode>, Ref<CSSMathNode>, Ref<CSSMathNode>, Ref<CSSMathNode>> m_padding;

        CSSPropertyValue<PositionAndSizeKeyword::KW_UNSET, Ref<CSSMathNode>, Ref<CSSMathNode>> m_gap;

        CSSPropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_bottomLeftBorderRadius;
        CSSPropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_topLeftBorderRadius;
        CSSPropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_topRightBorderRadius;
        CSSPropertyValue<UniversalKeyword::KW_UNSET, BorderRadiusAlternativeValue> m_bottomRightBorderRadius;

        CSSPropertyValue<ColorKeyword::KW_TRANSPARENT, Ref<CSSMathNode>, Ref<CSSMathNode>, Ref<CSSMathNode>, Ref<CSSMathNode>> m_backgroundColor;

        AssetWeakRef<Font> m_font;

        /// Used as scale.
        CSSPropertyValue<FontSizeKeyword::KW_UNSET, Ref<CSSMathNode>> m_fontSize;

        [[nodiscard]] const std::string& getName() const noexcept;

        void calculateCache(const UIElementCache* parentElementCache,
                            UIElementCache& thisElementCache) noexcept;

        void setFontSpecializationSettings(const FontSpecializationSettings& settings) noexcept;
        const FontSpecializationSettings& getFontSpecializationSettings() const noexcept;

        void updateFontSettings() noexcept;

        Ref<FontSpecialization> getFontSpecialization() const noexcept;

    protected:
        /// NOT SUPPORTED
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // NOT SUPPORTED
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

    private:
        FontSpecializationSettings m_fontSpecializationSettings;
        Weak<FontSpecialization> m_fontSpecialization;

        std::string m_name;
    };
}

#endif //SUNGEARENGINE_CSSSELECTOR_H
