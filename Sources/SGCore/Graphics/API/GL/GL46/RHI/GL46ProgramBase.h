//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <glad/glad.h>

#include "SGCore/Graphics/RHI/IShaderProgram.h"

namespace SGCore
{
    /// Common base of GL program objects usable in a GL46 pipeline: the linked program handle.
    /// GL46ShaderProgram owns its handle; GL46LegacyProgram borrows the handle of a legacy GL46Shader.
    class GL46ProgramBase : public IShaderProgram
    {
    public:
        [[nodiscard]] bool isValid() const noexcept override { return m_program != 0; }
        [[nodiscard]] GLuint getHandle() const noexcept { return m_program; }

    protected:
        GLuint m_program { };
    };

    /// Non-owning view of a legacy shader's program for the migration period: lets a legacy
    /// IShader (bound and fed through useX by the passes) participate in an RHI pipeline.
    class GL46LegacyProgram final : public GL46ProgramBase
    {
    public:
        explicit GL46LegacyProgram(GLuint program, const ShaderReflection& reflection) noexcept
        {
            m_program = program;
            m_reflection = reflection;
        }

        void update(GLuint program, const ShaderReflection& reflection) noexcept
        {
            m_program = program;
            m_reflection = reflection;
        }
    };
}
