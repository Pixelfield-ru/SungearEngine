//
// Created by stuka on 15.02.2026.
//

#pragma once

#include "IAsset.h"
#include "SGCore/Memory/IAssetsRefsResolver.h"
#include "FramesSequence.h"

namespace SGCore
{
    class ITexture2D;
    struct ByteFileAsset;

    struct GIF : IAsset, public IAssetsRefsResolver<GIF>
    {
        sg_assets_refs_resolver_as_friend

        FramesSequence m_sequence;

        AssetRef<ByteFileAsset> getFile() const noexcept;

    protected:
        void doLoad(const InterpolatedPath& path) override;
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;

        void onMemberAssetsReferencesResolveImpl(AssetManager* updatedAssetManager) noexcept;

    private:
        AssetWeakRef<ByteFileAsset> m_file;

        int m_width {};
        int m_height {};
        int m_channelsCount {};
    };
}
