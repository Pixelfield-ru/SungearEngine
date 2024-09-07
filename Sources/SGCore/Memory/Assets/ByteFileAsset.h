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
        ~ByteFileAsset();
        
        [[nodiscard]] char* getDataBuffer() const noexcept;
        [[nodiscard]] size_t getDataBufferSize() const noexcept;
    
    protected:
        void doLoad(const std::filesystem::path& path) override;
        
        char* m_dataBuffer = nullptr;
        size_t m_dataBufferSize = 0;
    };
}

#endif //SUNGEARENGINE_BYTEFILEASSET_H
