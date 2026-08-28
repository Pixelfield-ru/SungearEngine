//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <glad/glad.h>

#include "SGCore/Graphics/RHI/IPipelineState.h"

namespace SGCore
{
    /// On GL a pipeline is the program plus a VAO holding the vertex input *format*
    /// (glVertexArrayAttribFormat / AttribBinding); vertex buffers are attached per draw with
    /// glVertexArrayVertexBuffer, exactly like explicit APIs bind buffers per draw. Fixed-function
    /// state is applied at bind time through the renderer's cached state functions.
    class GL46PipelineState final : public IPipelineState
    {
    public:
        explicit GL46PipelineState(const PipelineStateDesc& desc) noexcept;
        ~GL46PipelineState() override;

        [[nodiscard]] GLuint getVertexArray() const noexcept { return m_vertexArray; }
        [[nodiscard]] GLuint getProgram() const noexcept;
        [[nodiscard]] GLenum getDrawMode() const noexcept { return m_drawMode; }
        [[nodiscard]] std::uint32_t getSlotStride(std::uint32_t slot) const noexcept;

    private:
        GLuint m_vertexArray { };
        GLenum m_drawMode = GL_TRIANGLES;
    };
}
