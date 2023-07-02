//
// Created by stuka on 24.04.2023.
//

#pragma once

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "SGCore/Main/Window.h"

#include "IShader.h"
#include "IVertexArray.h"
#include "IVertexBuffer.h"
#include "IVertexBufferLayout.h"
#include "IIndexBuffer.h"
#include "ITexture2D.h"
#include "IUniformBuffer.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/IMesh.h"
#include "SGCore/Memory/Assets/ModelAsset.h"

namespace Core::Graphics::API
{
    class IRenderer
    {
    public:
        // check usages in IVertexArray implementations and IIndexBuffer implementations
        IIndexBuffer* m_currentBoundIndexBuffer = nullptr;
        IVertexArray* m_currentBoundVertexArray = nullptr;

        virtual void init() { }

        virtual void renderFrame(const glm::ivec2& windowSize) { }

        virtual void renderMesh(const std::shared_ptr<IUniformBuffer>&, const std::shared_ptr<Memory::Assets::ModelAsset>&) { }

        virtual void printInfo() noexcept { }

        virtual void checkForErrors(std::source_location) noexcept { }

        [[nodiscard]] virtual IShader* createShader() = 0;
        [[nodiscard]] virtual IVertexBuffer* createVertexBuffer() = 0;
        [[nodiscard]] virtual IVertexArray* createVertexArray() = 0;
        [[nodiscard]] virtual IVertexBufferLayout* createVertexBufferLayout() = 0;
        [[nodiscard]] virtual IIndexBuffer* createIndexBuffer() = 0;
        [[nodiscard]] virtual ITexture2D* createTexture2D() = 0;
        [[nodiscard]] virtual IUniformBuffer* createUniformBuffer() = 0;

        [[nodiscard]] virtual IMesh* createMesh() = 0;
    };
}

#endif //NATIVECORE_IRENDERER_H
