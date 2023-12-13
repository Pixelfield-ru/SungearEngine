//
// Created by stuka on 13.05.2023.
//

#ifndef SUNGEARENGINE_GL46SHADER_H
#define SUNGEARENGINE_GL46SHADER_H

#include <glad/glad.h>
#include <vector>

#include "SGCore/Graphics/API/IShader.h"

namespace SGCore
{
    class GL46Shader : public IShader, public std::enable_shared_from_this<GL46Shader>
    {
    public:
        ~GL46Shader() noexcept override;

        void compile(Ref<FileAsset>) noexcept override;

        void bind() noexcept override;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) const noexcept override;

        void useUniformBuffer(const Ref<IUniformBuffer>&) override;
        void useTextureBlock(const std::string& uniformName, const uint8_t& texBlock) final;

        void useMatrix(const std::string& uniformName, const glm::mat4& matrix) final;

        void useVectorf(const std::string& uniformName, const float& x, const float& y) override;
        void useVectorf(const std::string& uniformName,
                        const float& x, const float& y, const float& z) override;
        void useVectorf(const std::string& uniformName,
                        const float& x, const float& y, const float& z, const float& w) override;

        void useVectorf(const std::string& uniformName, const glm::vec2& vec) override;
        void useVectorf(const std::string& uniformName, const glm::vec3& vec) override;
        void useVectorf(const std::string& uniformName, const glm::vec4& vec) override;

        void useFloat(const std::string& uniformName, const float& f) override;
        void useInteger(const std::string& uniformName, const size_t& i) override;

    private:
        GLuint m_programHandler = 0;

        // vertex, fragment, geometry, compute, tesselation control and tesselation evaluation shaders
        std::vector<GLuint> m_shaderPartsHandlers;

        GLuint createShaderPart(const GLenum& type, const std::string& finalShaderCode) noexcept;
    };
}

#endif //SUNGEARENGINE_GL46SHADER_H
