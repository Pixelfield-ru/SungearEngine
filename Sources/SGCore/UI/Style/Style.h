#pragma once

#include <vector>
#include "SGCore/UI/TransformTree/UIElementCache.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "EnumProperties.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/Font.h"
#include "SGCore/UI/DValue/DValueCowNode.h"
#include "SGCore/Utils/Macroses.h"
#include "SGCore/UI/DValue/DValueDestinationCacheNode.h"

namespace SGCore::UI
{
    template<auto... Members>
        struct RefStyleProperty {
        // using MemberType = std::remove_reference_t<std::invoke_result_t<decltype(Utils::FirstGenericOf<Members...>), Style>>;
    };

    struct Style
    {
        sg_serde_as_friend();

        // sg_implement_type_id(Style, 34)

        Style() noexcept;

        copy_constructor(Style) = default;
        move_constructor(Style) = default;

        copy_operator(Style) = default;
        move_operator(Style) = default;

#pragma region Properties

// iterate through all style properties names
#define style_properties(prop) \
    prop(display) \
    prop(flexDirection) \
    prop(flexWrap) \
    prop(width) \
    prop(height) \
    prop(paddingLeft) \
    prop(paddingRight) \
    prop(paddingBottom) \
    prop(paddingTop) \
    prop(padding) \
    prop(rowGap) \
    prop(columnGap) \
    prop(gap) \
    prop(bottomLeftBorderRadius) \
    prop(topLeftBorderRadius) \
    prop(topRightBorderRadius) \
    prop(bottomRightBorderRadius) \
    prop(borderRadius) \
    prop(backgroundColor) \
    prop(fontSize)


        DisplayKeyword m_display = DisplayKeyword::FLEX;
        FlexboxKeyword m_flexDirection = FlexboxKeyword::ROW;
        FlexboxKeyword m_flexWrap = FlexboxKeyword::NOWRAP;

        using SizeStyleProperty = DValue::DValueCowNode<std::variant<PositionAndSizeKeyword, float>>;
        SizeStyleProperty m_width {PositionAndSizeKeyword::UNSET};
        SizeStyleProperty m_height {PositionAndSizeKeyword::UNSET};

        using PaddingStyleProperty = DValue::DValueCowNode<std::variant<UniversalKeyword, float>>;
        PaddingStyleProperty m_paddingLeft {UniversalKeyword::UNSET};
        PaddingStyleProperty m_paddingRight {UniversalKeyword::UNSET};
        PaddingStyleProperty m_paddingBottom {UniversalKeyword::UNSET};
        PaddingStyleProperty m_paddingTop {UniversalKeyword::UNSET};

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
        GapStyleProperty m_rowGap {PositionAndSizeKeyword::UNSET};
        GapStyleProperty m_columnGap {PositionAndSizeKeyword::UNSET};
        
        RefStyleProperty<&Style::m_rowGap, &Style::m_columnGap> m_gap;

        struct BorderRadius final
        {
            DValue::DValueCacheNode<float> m_radiusX;
            DValue::DValueCacheNode<float> m_radiusY;
        };

        using BorderRadiusStyleProperty = DValue::DValueCowNode<std::variant<UniversalKeyword, BorderRadius>>;
        BorderRadiusStyleProperty m_bottomLeftBorderRadius {UniversalKeyword::UNSET};
        BorderRadiusStyleProperty m_topLeftBorderRadius {UniversalKeyword::UNSET};
        BorderRadiusStyleProperty m_topRightBorderRadius {UniversalKeyword::UNSET};
        BorderRadiusStyleProperty m_bottomRightBorderRadius {UniversalKeyword::UNSET};

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
            std::tuple<DValue::DValueCowNode<float>, DValue::DValueCowNode<float>, DValue::DValueCowNode<float>, DValue::DValueCowNode<float>>, // rgba
            std::tuple<DValue::DValueCowNode<float>, DValue::DValueCowNode<float>, DValue::DValueCowNode<float>> // rgb
        > m_backgroundColor = ColorKeyword::UNSET;

        DValue::DValueCowNode<AssetWeakRef<Font>> m_font;

        /// Used as scale.
        DValue::DValueCowNode<std::variant<FontSizeKeyword, float>> m_fontSize {10.0f};

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

        /*/// NOT SUPPORTED
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // NOT SUPPORTED
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;*/

    private:
        FontSpecializationSettings m_fontSpecializationSettings;
        Weak<FontSpecialization> m_fontSpecialization;

        std::string m_selector;
        std::string m_pseudoClass;

        mutable size_t m_selectorHash = 0;
        mutable size_t m_pseudoClassHash = 0;
    };
};