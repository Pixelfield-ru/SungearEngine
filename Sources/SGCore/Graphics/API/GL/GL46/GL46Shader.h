//
// Created by stuka on 13.05.2023.
//

#pragma once

#ifndef SUNGEARENGINE_GL46SHADER_H
#define SUNGEARENGINE_GL46SHADER_H

#include "OpenGL/include/glad/glad.h"
#include <vector>

#include "SGCore/Graphics/API/IShader.h"

namespace Core::Graphics::GL
{
    class GL46Shader : public IShader
    {
    private:
        GLuint m_programHandler = 0;

        // vertex, fragment, geometry, compute, tesselation control and tesselation evaluation shaders
        std::vector<GLuint> m_shaderPartsHandlers;

        GLuint createShaderPart(const GLenum&, const std::string&) noexcept;

    public:
        ~GL46Shader() noexcept override;

        void compile(std::shared_ptr<Memory::Assets::FileAsset>) noexcept override;

        void bind() noexcept override;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) const noexcept override;
    };
}

#endif //SUNGEARENGINE_GL46SHADER_H
