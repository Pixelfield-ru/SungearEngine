//
// Created by stuka on 24.04.2023.
//

#ifndef SUNGEARENGINE_GL46RENDERER_H
#define SUNGEARENGINE_GL46RENDERER_H

#include "SGCore/Graphics/API/GL/GL46/GL46SubPassShader.h"
#include "SGCore/Graphics/API/GL/GLVertexArray.h"
#include "SGCore/Graphics/API/GL/GLVertexBuffer.h"
#include "SGCore/Graphics/API/GL/GLVertexBufferLayout.h"
#include "SGCore/Graphics/API/GL/GLIndexBuffer.h"
#include "SGCore/Graphics/API/GL/GL46/GL46Texture2D.h"
#include "SGCore/Graphics/API/GL/GL46/GL46UniformBuffer.h"

#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "SGCore/Graphics/API/GL/GL3/GL3MeshData.h"

#include "SGCore/Graphics/API/GL/GL4/GL4Renderer.h"

namespace SGCore
{
    class CoreMain;

    class GL46Renderer : public GL4Renderer
    {
    public:
        GL46Renderer(const GL46Renderer&) = delete;
        GL46Renderer(GL46Renderer&&) = delete;

        bool confirmSupport() noexcept override;

        [[nodiscard]] GL46SubPassShader* createSubPassShader() const override;

        [[nodiscard]] GL46Texture2D* createTexture2D() const final;

        static const std::shared_ptr<GL46Renderer>& getInstance() noexcept;
    
    private:
        // Buffer for storing matrices of the currently rendered model.
        std::shared_ptr<GL46UniformBuffer> m_modelMatricesBuffer;
        // Buffer for storing matrices of the currently main camera.
        std::shared_ptr<GL46UniformBuffer> m_cameraMatricesBuffer;
        
        GL46Renderer() noexcept = default;
        
        static inline std::shared_ptr<GL46Renderer> m_instance;
    };
}

#endif //SUNGEARENGINE_GL46RENDERER_H
