//
// Created by stuka on 13.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL46SHADER_H
#define SUNGEARENGINE_GL46SHADER_H

#include "OpenGL/include/glad/glad.h"
#include <vector>

#include "SGCore/Graphics/API/IShader.h"

namespace Core::Graphics::API::GL::GL46
{
    class GL46Shader : public IShader
    {
    private:
        GLuint m_programHandler = 0;

        // vertex, fragment, geometry, compute, tesselation control and tesselation evaluation shaders
        std::vector<GLuint> m_shaderPartsHandlers;

        GLuint createShaderPart(const GLenum&, const std::string&) noexcept;

    public:
        // move
        //GL46Shader(GL46Shader&&) noexcept;

        ~GL46Shader() noexcept override;

        void compile(Memory::Assets::FileAsset*) noexcept override;

        void bind() noexcept override;

        void destroy() noexcept override;

        // move
        //Core::Graphics::API::GL46::GL46Shader& operator=(GL46Shader&&) noexcept;
    };
}

#endif //SUNGEARENGINE_GL46SHADER_H
