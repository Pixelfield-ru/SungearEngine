//
// Created by stuka on 07.11.2025.
//

#pragma once

#include <unordered_set>

#include "SGCore/Graphics/API/IGPUObjectsStorage.h"

namespace SGCore
{
    class GL4UniformBuffer;
    struct GL4FrameBuffer;
    struct GL4Texture2D;
    struct GL46Shader;
    struct GLVertexArray;
    class ITexture2D;

    struct GLObjectsStorage : IGPUObjectsStorage
    {
        friend struct GL4Renderer;
        friend class GL46Renderer;

        friend class GL4UniformBuffer;
        friend struct GL4FrameBuffer;
        friend struct GL4Texture2D;
        friend struct GL46Shader;
        friend struct GLVertexArray;

        void recreateAll() noexcept override;

        void clear() noexcept override;

    private:
        std::unordered_set<GL4FrameBuffer*> m_frameBuffers;
        std::unordered_set<ITexture2D*> m_textures;
        std::unordered_set<GL46Shader*> m_shaders;
        std::unordered_set<GLVertexArray*> m_vertexArrays;
        std::unordered_set<GL4UniformBuffer*> m_uniformBuffers;
    };
}
