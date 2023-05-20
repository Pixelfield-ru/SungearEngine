//
// Created by stuka on 13.05.2023.
//

#ifndef NATIVECORE_GLSHADER_H
#define NATIVECORE_GLSHADER_H

#include <glad/glad.h>

#include "../Shader.h"

namespace Core::Graphics::API::GL46
{
    class GLShader : public Shader
    {
    private:
        GLuint m_programHandler;

        // vertex, fragment, geometry, compute, tesselation control and tesselation evaluation shaders
        std::array<GLuint, 6> m_shaderPartsHandlers;

        GLuint createShaderPart(const GLenum&, const std::string_view&, const std::string&) noexcept;

    public:
        // move
        //GLShader(GLShader&&) noexcept;

        ~GLShader() noexcept;

        void compile(const std::string_view&, const std::string&) noexcept override;

        void bind() noexcept override;

        void destroy() noexcept override;

        // move
        //Core::Graphics::API::GL46::GLShader& operator=(GLShader&&) noexcept;
    };
}

#endif //NATIVECORE_GLSHADER_H
