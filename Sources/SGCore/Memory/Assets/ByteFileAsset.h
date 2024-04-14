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
    protected:
        char* m_buffer = nullptr;
        size_t m_bufferSize = 0;
        
    public:
        ~ByteFileAsset();
        
        void load(const std::string& path) override;
        
        [[nodiscard]] char* getBuffer() const noexcept;
        [[nodiscard]] size_t getBufferSize() const noexcept;
    };
}

#endif //SUNGEARENGINE_BYTEFILEASSET_H
