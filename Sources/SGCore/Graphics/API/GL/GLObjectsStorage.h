//
// Created by stuka on 07.11.2025.
//

#pragma once

#include <vector>

#include "SGCore/Graphics/API/IGPUObjectsStorage.h"

namespace SGCore
{
    class GL4UniformBuffer;
    struct GL4FrameBuffer;
    struct GL4Texture2D;
    struct GL46Shader;
    struct GLVertexArray;

    struct GLObjectsStorage : IGPUObjectsStorage
    {
        void recreateAll() noexcept override;

        void clear() noexcept override;

    private:
        std::vector<GL4FrameBuffer*> m_frameBuffers;
        std::vector<GL4Texture2D*> m_textures;
        std::vector<GL46Shader*> m_shaders;
        std::vector<GLVertexArray*> m_vertexArrays;
        std::vector<GL4UniformBuffer*> m_uniformBuffers;
    };
}
