//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKSHADER_H
#define SUNGEARENGINE_VKSHADER_H

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    // TODO: impl VkShader
    class VkSubPassShader : public ISubPassShader
    {
    public:
        ~VkSubPassShader() noexcept override;

        void compile(const std::string& subPassName) noexcept final;

        void bind() noexcept final;

        void useUniformBuffer(const Ref<IUniformBuffer>&) override;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) noexcept final;
    };
}

#endif //SUNGEARENGINE_VKSHADER_H
