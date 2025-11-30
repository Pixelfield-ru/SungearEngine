//
// Created by stuka on 17.01.2025.
//

#ifndef SUNGEARENGINE_CSSFILE_H
#define SUNGEARENGINE_CSSFILE_H

#include "SGCore/Memory/Assets/IAsset.h"
#include "SGCore/Memory/AssetRef.h"

#include "CSSStyle.h"

#include <antlr4-runtime.h>

namespace SGCore::UI
{
    struct CSSFile : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(CSSFile, 33)

        friend struct ANTLRCSSListener;

        [[nodiscard]] AssetRef<CSSStyle> findStyle(const std::string& selector) const noexcept;

        const std::vector<AssetRef<CSSStyle>>& getStyles() const noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        std::vector<AssetRef<CSSFile>> m_importedFiles;
        std::vector<AssetRef<CSSStyle>> m_styles;
    };
}

#endif //SUNGEARENGINE_CSSFILE_H
