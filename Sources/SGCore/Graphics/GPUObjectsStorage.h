//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_GPUOBJECTSSTORAGE_H
#define SUNGEARENGINE_GPUOBJECTSSTORAGE_H

#include <string>
#include <unordered_map>

#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Utils/UniqueNamesManager.h"

namespace SGCore
{
    class IFrameBuffer;
    class IShader;
    class ITexture2D;

    class GPUObjectsStorage
    {
    public:
        static void addShader(const Ref<IShader>& shader) noexcept;
        static void addTexture(const Ref<ITexture2D>& texture2D) noexcept;
        static void addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept;

    private:
        static inline std::unordered_map<std::string, Weak<IShader>> m_shaders;
        static inline std::unordered_map<std::string, Weak<ITexture2D>> m_textures2D;
        static inline std::unordered_map<std::string, Weak<IFrameBuffer>> m_frameBuffers;

        static inline Ref<UniqueNamesManager> m_uniqueNamesManager = MakeRef<UniqueNamesManager>();
    };
}

#endif //SUNGEARENGINE_GPUOBJECTSSTORAGE_H
