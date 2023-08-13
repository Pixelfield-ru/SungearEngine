//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKRENDERER_H
#define SUNGEARENGINE_VKRENDERER_H

#include "SGCore/Graphics/API/IRenderer.h"
#include "VkShader.h"
#include "VkVertexArray.h"
#include "VkVertexBuffer.h"
#include "VkVertexBufferLayout.h"
#include "VkIndexBuffer.h"
#include "VkTexture2D.h"
#include "VkUniformBuffer.h"
#include "VkMesh.h"
#include "VkFrameBuffer.h"

//#include <vulkan/vulkan.h>

namespace Core::Graphics
{
    class VkRenderer : public IRenderer
    {
    private:
        VkRenderer() noexcept = default;

        /*VkInstance m_vkInstance = nullptr;
        VkApplicationInfo m_applicationInfo { };
        VkInstanceCreateInfo m_instanceCreateInfo { };*/

        // singleton instance
        static inline std::shared_ptr<VkRenderer> m_instance;

    public:
        VkRenderer(const VkRenderer&) = delete;
        VkRenderer(VkRenderer&&) = delete;

        void init() noexcept override;

        bool confirmSupport() noexcept final;

        void renderFrame(const glm::ivec2& windowSize) override;

        void renderMesh(const std::shared_ptr<ECS::CameraComponent>& cameraComponent,
                        const std::shared_ptr<ECS::TransformComponent>& cameraTransformComponent,
                        const std::shared_ptr<ECS::TransformComponent>& transformComponent,
                        const std::shared_ptr<ECS::MeshComponent>& meshComponent) override;

        void printInfo() noexcept override;

        /**
         * Checks for errors in GAPI.
         * @param location - Where the function is called from.
         */
        void checkForErrors(const std::source_location& location = std::source_location::current()) noexcept override;

        // TODO: create docs
        [[nodiscard]] VkShader* createShader() override;
        [[nodiscard]] VkShader* createPBRShader() override;
        [[nodiscard]] VkShader* createOnlyGeometryShader() override;
        [[nodiscard]] VkShader* createSkyboxShader() override;

        [[nodiscard]] VkVertexArray* createVertexArray() override;
        [[nodiscard]] VkVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] VkVertexBufferLayout* createVertexBufferLayout() override;
        [[nodiscard]] VkIndexBuffer* createIndexBuffer() override;
        [[nodiscard]] VkTexture2D* createTexture2D() override;
        [[nodiscard]] VkUniformBuffer* createUniformBuffer() override;
        [[nodiscard]] VkFrameBuffer* createFrameBuffer() override;

        [[nodiscard]] VkMesh* createMesh() override;

        [[nodiscard]] Memory::Assets::IMaterial* createMaterial() override;

        static const std::shared_ptr<VkRenderer>& getInstance() noexcept;
    };
}

#endif //SUNGEARENGINE_VKRENDERER_H
