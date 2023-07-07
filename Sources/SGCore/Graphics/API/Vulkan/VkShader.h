//
// Created by stuka on 07.07.2023.
//

#ifndef SUNGEARENGINE_VKSHADER_H
#define SUNGEARENGINE_VKSHADER_H

#include "SGCore/Graphics/API/IShader.h"

namespace Core::Graphics
{
    // TODO: impl VkShader
    class VkShader : public IShader
    {
    public:
        ~VkShader() noexcept override;

        void compile(std::shared_ptr<Memory::Assets::FileAsset>) noexcept final;

        void bind() noexcept final;

        void destroy() noexcept final;

        std::int32_t getShaderUniformLocation(const std::string& uniformName) const noexcept final;
    };
}

#endif //SUNGEARENGINE_VKSHADER_H
