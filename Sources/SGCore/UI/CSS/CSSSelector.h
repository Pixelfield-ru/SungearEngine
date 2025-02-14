//
// Created by stuka on 18.01.2025.
//

#ifndef SUNGEARENGINE_CSSSELECTOR_H
#define SUNGEARENGINE_CSSSELECTOR_H

#include <vector>

#include "CSSSelectorCache.h"
#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/UI/CSS/PropertyValues/CSSPropertyValue.h"

#include "PropertyValues/CSSPropertyValueKeywords.h"

#include "SGCore/UI/CSS/Math/CSSMathNode.h"

namespace SGCore::UI
{
    struct CSSSelector : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(CSSSelector, 34)

        friend struct ANTLRCSSListener;
        friend struct CSSFile;

        FlexboxKeyword m_flexDirection = FlexboxKeyword::KW_ROW;
        DisplayKeyword m_display = DisplayKeyword::KW_FLEX;

        CSSPropertyValue<PositionAndSizeKeyword::KW_AUTO, Ref<CSSMathNode>> m_width;
        CSSPropertyValue<PositionAndSizeKeyword::KW_AUTO, Ref<CSSMathNode>> m_height;

        CSSPropertyValue<ColorKeyword::KW_TRANSPARENT, Ref<CSSMathNode>, Ref<CSSMathNode>, Ref<CSSMathNode>, Ref<CSSMathNode>> m_backgroundColor;

        [[nodiscard]] const std::string& getName() const noexcept;

    protected:
        /// NOT SUPPORTED
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // NOT SUPPORTED
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

    private:
        std::string m_name;
    };
}

#endif //SUNGEARENGINE_CSSSELECTOR_H
