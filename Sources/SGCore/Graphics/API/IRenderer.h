//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "IShader.h"
#include "IVertexArray.h"
#include "IVertexBuffer.h"
#include "IVertexBufferLayout.h"
#include "IIndexBuffer.h"
#include "ITexture2D.h"
#include "IUniformBuffer.h"

#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/ImportedScenesArch/IMesh.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/ECS/Entity.h"
#include "SGCore/Utils/Defines.h"

#include "APIType.h"
#include "IFrameBuffer.h"

namespace Core::ECS
{
    class CameraComponent;
    class MeshComponent;
    class TransformComponent;
    class ShadowsCasterComponent;
    class SkyboxComponent;
}

namespace Core::Graphics
{
    class IRenderer
    {
    protected:
        APIType m_apiType = APIType::UNKNOWN;

    public:
        // check usages in IVertexArray implementations and IIndexBuffer implementations
        // TODO: UB IS HERE
        IIndexBuffer* m_currentBoundIndexBuffer = nullptr;
        IVertexArray* m_currentBoundVertexArray = nullptr;

        virtual void init() { }

        /**
         * Confirmation of GAPI support by the user's graphics card
         */
        virtual bool confirmSupport() noexcept { }

        /**
         *
         * @param windowSize
         */
        virtual void renderFrame(const glm::ivec2& windowSize) { }

        /**
         * Renders the model using matrices from objectMatricesBuffer.
         */
        virtual void renderMesh(const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
                                const std::shared_ptr<ECS::TransformComponent>& cameraTransformComponent,
                                const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                                const std::shared_ptr<ECS::MeshComponent>& meshComponent) { }

        /**
        * Renders the model for shadow using matrices from objectMatricesBuffer.
        */
        virtual void renderMesh(const std::shared_ptr<ECS::ShadowsCasterComponent>& shadowsCasterComponent,
                                const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                                const std::shared_ptr<ECS::MeshComponent>& meshComponent) { }

        /**
        * Renders the model using skybox shader and matrices from objectMatricesBuffer.
        */
        virtual void renderMesh(const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
                                const std::shared_ptr<ECS::SkyboxComponent>& skyboxComponent,
                                const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                                const std::shared_ptr<ECS::MeshComponent>& meshComponent) { }

        /**
         * Prints information about the graphics capabilities of the kernel on this GAPI and information about the GAPI itself.
         */
        virtual void printInfo() noexcept { }

        virtual void checkForErrors(const std::source_location& = std::source_location::current()) noexcept { }

        [[nodiscard]] virtual IShader* createShader() = 0;
        [[nodiscard]] virtual IShader* createPBRShader() = 0;
        [[nodiscard]] virtual IShader* createOnlyGeometryShader() = 0;
        [[nodiscard]] virtual IShader* createSkyboxShader() = 0;

        [[nodiscard]] virtual IVertexBuffer* createVertexBuffer() = 0;
        [[nodiscard]] virtual IVertexArray* createVertexArray() = 0;
        [[nodiscard]] virtual IVertexBufferLayout* createVertexBufferLayout() = 0;
        [[nodiscard]] virtual IIndexBuffer* createIndexBuffer() = 0;
        [[nodiscard]] virtual ITexture2D* createTexture2D() = 0;
        [[nodiscard]] virtual IUniformBuffer* createUniformBuffer() = 0;
        [[nodiscard]] virtual IFrameBuffer* createFrameBuffer() = 0;

        [[nodiscard]] virtual ImportedScene::IMesh* createMesh() = 0;

        [[nodiscard]] virtual Memory::Assets::IMaterial* createMaterial() = 0;

        [[nodiscard]] APIType getAPIType() const noexcept;
    };
}

#endif //NATIVECORE_IRENDERER_H
