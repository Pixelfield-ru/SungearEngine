//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_BYTEFILEASSET_H
#define SUNGEARENGINE_BYTEFILEASSET_H

#include "IAsset.h"

namespace SGCore
{
    struct ByteFileAsset : public IAsset
    {
        sg_implement_type_id(ByteFileAsset, 7)

        ~ByteFileAsset();
        
        [[nodiscard]] char* getDataBuffer() const noexcept;
        [[nodiscard]] size_t getDataBufferSize() const noexcept;
    
    protected:
        void doLoad(const InterpolatedPath& path) override;
        void doReloadFromDisk(AssetsLoadPolicy loadPolicy, Ref<Threading::Thread> lazyLoadInThread) noexcept override;

        // TODO: impl
        void doLoadFromBinaryFile(AssetManager* parentAssetManager) noexcept override;
        
        char* m_dataBuffer = nullptr;
        size_t m_dataBufferSize = 0;
    };
}

#endif //SUNGEARENGINE_BYTEFILEASSET_H
