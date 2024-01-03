//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKSHADER_H
#define SUNGEARENGINE_VKSHADER_H

#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    // TODO: impl VkShader
    class VkShader : public ISubPassShader
    {
    public:
        ~VkShader() noexcept override;

        void compile(Ref<FileAsset>) noexcept final;

        void bind() noexcept final;

        void useUniformBuffer(const Ref<IUniformBuffer>&) override;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) noexcept final;
    };
}

#endif //SUNGEARENGINE_VKSHADER_H
