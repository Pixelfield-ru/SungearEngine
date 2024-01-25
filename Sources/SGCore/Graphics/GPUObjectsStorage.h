//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_GPUOBJECTSSTORAGE_H
#define SUNGEARENGINE_GPUOBJECTSSTORAGE_H

#include <string>
#include <unordered_map>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/UniqueNamesManager.h"

namespace SGCore
{
    class IFrameBuffer;
    class ISubPassShader;
    class ITexture2D;

    class GPUObjectsStorage
    {
    public:
        static void addShader(const Ref<ISubPassShader>& shader) noexcept;
        static void addTexture(const Ref<ITexture2D>& texture2D) noexcept;
        static void addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept;

    private:
        static inline std::unordered_map<std::string, Weak<ISubPassShader>> m_shaders;
        static inline std::unordered_map<std::string, Weak<ITexture2D>> m_textures2D;
        static inline std::unordered_map<std::string, Weak<IFrameBuffer>> m_frameBuffers;

        static inline Ref<SGUtils::UniqueNamesManager> m_uniqueNamesManager = MakeRef<SGUtils::UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINE_GPUOBJECTSSTORAGE_H
