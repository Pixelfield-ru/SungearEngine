//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKSHADER_H
#define SUNGEARENGINE_VKSHADER_H

#include "SGCore/Graphics/API/IShader.h"

namespace SGCore
{
    // TODO: impl VkShader
    struct VkShader : public IShader
    {
        ~VkShader() noexcept override;

        void destroy() noexcept final;

        void bind() const noexcept final;

        void useUniformBuffer(const Ref<IUniformBuffer>&) override;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) noexcept final;

    private:
        void doCompile() noexcept final;
    };
}

#endif //SUNGEARENGINE_VKSHADER_H
