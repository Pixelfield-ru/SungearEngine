//
// Created by stuka on 13.05.2023.
//

#ifndef SUNGEARENGINE_GL46SHADER_H
#define SUNGEARENGINE_GL46SHADER_H

#include <glad/glad.h>
#include <vector>

#include "SGCore/Graphics/API/IShader.h"

namespace Core::Graphics
{
    class GL46Shader : public IShader, public std::enable_shared_from_this<GL46Shader>
    {
    public:
        ~GL46Shader() noexcept override;

        void compile(std::shared_ptr<Memory::Assets::FileAsset>) noexcept override;

        void bind() noexcept override;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) const noexcept override;

        void useMaterialTexture(const Memory::Assets::MaterialTexture&) final;
        void useUniformBuffer(const std::shared_ptr<IUniformBuffer>&) override;
        void useTexture(const std::string& uniformName, const std::uint8_t& texBlock) final;

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

    private:
        GLuint m_programHandler = 0;

        // vertex, fragment, geometry, compute, tesselation control and tesselation evaluation shaders
        std::vector<GLuint> m_shaderPartsHandlers;

        GLuint createShaderPart(const GLenum&, const std::string&) noexcept;
    };
}

#endif //SUNGEARENGINE_GL46SHADER_H
