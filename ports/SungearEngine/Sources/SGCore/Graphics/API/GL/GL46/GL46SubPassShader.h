//
// Created by stuka on 13.05.2023.
//

#ifndef SUNGEARENGINE_GL46SUBPASSSHADER_H
#define SUNGEARENGINE_GL46SUBPASSSHADER_H

#include <glad/glad.h>
#include <vector>

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    class GL46SubPassShader : public ISubPassShader
    {
    public:
        ~GL46SubPassShader() noexcept override;

        void compile(Ref<FileAsset> fileAsset) override;

        void bind() noexcept override;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) noexcept override;

        void useUniformBuffer(const Ref<IUniformBuffer>&) override;
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
        void useInteger(const std::string& uniformName, const size_t& i) override;
        void useTextureBlock(const std::string& uniformName, const size_t& textureBlock) override;
        
        bool isUniformExists(const std::string& uniformName) const noexcept;

    private:
        GLuint compileSubShader(SGSLESubShaderType shaderType, const std::string& code);

        GLuint m_programHandler = 0;

        std::unordered_map<std::string, GLint> m_cachedLocations;

        // vertex, fragment, geometry, compute, tesselation control and tesselation evaluation shaders
        std::vector<GLuint> m_subShadersHandlers;
    };
}

#endif //SUNGEARENGINE_GL46SUBPASSSHADER_H
