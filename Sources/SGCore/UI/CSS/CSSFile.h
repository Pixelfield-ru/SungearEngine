//
// Created by stuka on 17.01.2025.
//

#ifndef SUNGEARENGINE_CSSFILE_H
#define SUNGEARENGINE_CSSFILE_H

#include "SGCore/Memory/Assets/IAsset.h"

namespace SGCore::UI
{
    struct CSSFile : IAsset
    {
        sg_serde_as_friend();

        sg_implement_type_id(CSSFile, 33)

    protected:
        // todo: impl
        void doLoad(const InterpolatedPath& path) final;

        // todo: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept final;

        // todo: impl
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;
    };
}

#endif //SUNGEARENGINE_CSSFILE_H
