//
// Created by Ilya on 20.12.2023.
//

#ifndef SUNGEARENGINE_GPUOBJECTSSTORAGE_H
#define SUNGEARENGINE_GPUOBJECTSSTORAGE_H

#include <SGCore/pch.h>

#include "SGCore/Main/CoreGlobals.h"
#include "SGUtils/UniqueNamesManager.h"

namespace SGCore
{
    class IFrameBuffer;
    class ISubPassShader;
    class ITexture2D;

    class GPUObjectsStorage
    {
        friend class IShader;

    public:
        static void addShader(const Ref<ISubPassShader>& shader) noexcept;
        static void addTexture(const Ref<ITexture2D>& texture2D) noexcept;
        static void addFramebuffer(const Ref<IFrameBuffer>& frameBuffer) noexcept;

    private:
        static void onSomeObjectNameChanged(const std::string& newName) noexcept;

        static inline std::vector<Weak<ISubPassShader>> m_shaders;
        static inline std::vector<Weak<ITexture2D>> m_textures2D;
        static inline std::vector<Weak<IFrameBuffer>> m_frameBuffers;

        static inline Ref<UniqueNamesManager> m_uniqueNamesManager = MakeRef<UniqueNamesManager>();

        static inline EventListener<void(const std::string&)> m_someNameChangedListener = [](const std::string& newName)
                                     {
                                         GPUObjectsStorage::onSomeObjectNameChanged(newName);
                                     };

        [[maybe_unused]] static inline bool s_staticInit = []() {
            GPUObjectsStorage::m_uniqueNamesManager->subscribeToSomeNameChangedEvent(
                    GPUObjectsStorage::m_someNameChangedListener
            );

            return true;
        }();
    };
}

#endif //SUNGEARENGINE_GPUOBJECTSSTORAGE_H
