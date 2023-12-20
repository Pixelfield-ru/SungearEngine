//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_GPUOBJECTSSTORAGE_H
#define SUNGEARENGINE_GPUOBJECTSSTORAGE_H

#include <string>
#include <unordered_map>

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IFrameBuffer;
    class IShader;
    class ITexture2D;

    class GPUObjectsStorage
    {
    public:
        void addShader(const Ref<IShader>& shader) noexcept;
        void addTexture2D(const Ref<ITexture2D>& texture2D) noexcept;
        void addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept;

    private:
        std::unordered_map<std::string, Weak<IShader>> m_shaders;
        std::unordered_map<std::string, Weak<ITexture2D>> m_textures2D;
        std::unordered_map<std::string, Weak<IFrameBuffer>> m_frameBuffers;
    };
}

#endif //SUNGEARENGINE_GPUOBJECTSSTORAGE_H
