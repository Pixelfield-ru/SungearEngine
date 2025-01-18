//
// Created by stuka on 18.01.2025.
//

#ifndef SUNGEARENGINE_CSSSELECTOR_H
#define SUNGEARENGINE_CSSSELECTOR_H

#include <vector>
#include <SGCore/Memory/Assets/IAsset.h>

#include "SGCore/UI/CSS/PropertyEnumTypes/CSSFlexDirectionType.h"

namespace SGCore::UI
{
    struct CSSSelector : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(CSSSelector, 34)

        friend struct ANTLRCSSListener;
        friend struct CSSFile;

        CSSFlexDirectionType m_flexDirection = CSSFlexDirectionType::FDT_ROW;

        // -1 -1 means auto size
        glm::vec2 m_elementSize = { -1, -1 };

        [[nodiscard]] const std::string& getName() const noexcept;

    protected:
        /// DOES NOT SUPPORTED
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // DOES NOT SUPPORTED
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

    private:
        std::string m_name;
    };
}

#endif //SUNGEARENGINE_CSSSELECTOR_H
