//
// Created by stuka on 24.04.2023.
//

#ifndef NATIVECORE_IRENDERER_H
#define NATIVECORE_IRENDERER_H

#include <iostream>
#include <glm/glm.hpp>
#include <glm/common.hpp>

#include "GAPIType.h"
#include "IUniformBuffer.h"
#include "SGCore/Utils/ShadersPaths.h"
#include "SGCore/Transformations/Transform.h"

namespace SGCore
{
    class Camera;
    class Mesh;
    struct TransformBase;
    struct AtmosphereScattering;
    class RenderingBase;
    class IGizmo;

    class IMeshData;
    struct MeshDataRenderInfo;

    class IVertexBufferLayout;
    struct ICubemapTexture;
    class IIndexBuffer;
    class IVertexArray;
    class IVertexBuffer;
    class IFrameBuffer;
    class ITexture2D;
    class ISubPassShader;

    // todo: make description of funcs
    class IRenderer
    {
    public:
        // Buffer for storing matrices of the currently main camera.
        Ref<IUniformBuffer> m_viewMatricesBuffer;
        // Buffer for storing matrices of the program.
        Ref<IUniformBuffer> m_programDataBuffer;

        virtual void init() { }

        /**
         * Confirmation of GAPI support by the user's graphics card
         */
        virtual bool confirmSupport() noexcept { return false; }

        /**
         *
         * @param windowSize
         */
        virtual void prepareFrame(const glm::ivec2& windowSize) { }

        /**
         * Preparing uniform buffers for render
         * @param renderingComponent - The component that will be used as a "camera" for rendering entities.
         * @param transformComponent - The transform component of this "camera".
         */
        virtual void prepareUniformBuffers(const RenderingBase& renderingBase,
                                           const Transform& transform) { }

        virtual void renderMeshData(const Ref<IMeshData>& meshData,
                                    const MeshDataRenderInfo& meshDataRenderInfo) { }

        /**
         * Prints information about the graphics capabilities of the kernel on this GAPI and information about the GAPI itself.
         */
        virtual void printInfo() noexcept { }

        virtual void checkForErrors(const std::source_location& = std::source_location::current()) noexcept { }

        [[nodiscard]] virtual ISubPassShader* createShader() const = 0;
        [[nodiscard]] virtual ISubPassShader* createShader(const std::string& path) const = 0;

        [[nodiscard]] ISubPassShader* createShader(const ShadersPaths& shadersPaths) const
        {
            return createShader(shadersPaths.getCurrentRealization());
        };

        [[nodiscard]] virtual IVertexBuffer* createVertexBuffer() const = 0;
        [[nodiscard]] virtual IVertexArray* createVertexArray() const = 0;
        [[nodiscard]] virtual IVertexBufferLayout* createVertexBufferLayout() const = 0;
        [[nodiscard]] virtual IIndexBuffer* createIndexBuffer() const = 0;
        [[nodiscard]] virtual ITexture2D* createTexture2D() const = 0;
        [[nodiscard]] virtual ICubemapTexture* createCubemapTexture() const = 0;
        [[nodiscard]] virtual IUniformBuffer* createUniformBuffer() const = 0;
        [[nodiscard]] virtual IFrameBuffer* createFrameBuffer() const = 0;

        [[nodiscard]] virtual IMeshData* createMeshData() const = 0;

        // ------------- some settings for renderer ---------
        virtual void setDepthTestingEnabled(const bool& enabled) const noexcept { }
        // --------------------------------------------------

        [[nodiscard]] GAPIType getGAPIType() const noexcept;

    protected:
        GAPIType m_apiType = SG_API_TYPE_UNKNOWN;
    };
}

#endif //NATIVECORE_IRENDERER_H
