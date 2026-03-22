//
// Created by ilya on 13.04.24.
//

#pragma once

#include "IAsset.h"
#include "SGCore/Memory/AssetsPackage.h"

sg_predeclare_serde()

namespace SGCore
{
    struct SGCORE_EXPORT ByteFileAsset : public IAsset
    {
        sg_implement_type_id(SGCore::ByteFileAsset)
        sg_serde_as_friend()

        ~ByteFileAsset() override;
        
        [[nodiscard]] char* getDataBuffer() const noexcept;
        [[nodiscard]] size_t getDataBufferSize() const noexcept;
    
    protected:
        void doLoad(const InterpolatedPath& path) override;
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        
        char* m_dataBuffer = nullptr;
        size_t m_dataBufferSize = 0;

        AssetsPackage::DataMarkup m_dataMarkupInPackage;
    };
}
