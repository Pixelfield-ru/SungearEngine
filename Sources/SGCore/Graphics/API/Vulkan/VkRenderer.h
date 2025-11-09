//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKRENDERER_H
#define SUNGEARENGINE_VKRENDERER_H

#include "SGCore/Graphics/API/IRenderer.h"
#include "VkShader.h"
#include "VkVertexArray.h"
#include "VkVertexBuffer.h"
#include "VkIndexBuffer.h"
#include "VkTexture2D.h"
#include "VkUniformBuffer.h"
#include "VkMeshData.h"
#include "VkFrameBuffer.h"
#include "VkCubemapTexture.h"

//#include <vulkan/vulkan.h>

namespace SGCore
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

        void prepareFrame(const glm::ivec2& windowSize) override;

        void useState(const RenderState& newRenderState, bool forceState = false) noexcept final;
        void useBlendingState(const BlendingState& newBlendingState, bool forceState = false) noexcept final;
        void useMeshRenderState(const MeshRenderState& newMeshRenderState, bool forceState = false) noexcept final;

        void printInfo() noexcept override;

        /**
         * Checks for errors in GAPI.
         * @param location - Where the function is called from.
         */
        void checkForErrors(const std::source_location& location = std::source_location::current()) noexcept override;

        // TODO: create docs
        [[nodiscard]] VkShader* createShader() override;
        [[nodiscard]] VkVertexArray* createVertexArray() override;
        [[nodiscard]] VkVertexBuffer* createVertexBuffer() override;
        [[nodiscard]] VkIndexBuffer* createIndexBuffer() override;
        [[nodiscard]] VkTexture2D* createTexture2D() override;
        [[nodiscard]] VkCubemapTexture* createCubemapTexture() override;
        [[nodiscard]] VkUniformBuffer* createUniformBuffer() override;
        [[nodiscard]] VkFrameBuffer* createFrameBuffer() override;

        [[nodiscard]] VkMeshData* createMeshData() const override;

        void bindScreenFrameBuffer() const noexcept final;

        IGPUObjectsStorage& storage() noexcept final;
        const IGPUObjectsStorage& storage() const noexcept final;
        
        static const std::shared_ptr<VkRenderer>& getInstance() noexcept;

    private:
        std::optional<IGPUObjectsStorage*> m_dummyStorage;
    };
}

#endif //SUNGEARENGINE_VKRENDERER_H
