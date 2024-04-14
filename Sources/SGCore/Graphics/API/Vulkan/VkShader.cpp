//
// Created by stuka on 07.07.2023.
//

#include "VkShader.h"

SGCore::VkShader::~VkShader() noexcept
{
    destroy();
}

void SGCore::VkShader::compile(Ref<TextFileAsset>) noexcept
{

}

void SGCore::VkShader::bind() noexcept
{

}

void SGCore::VkShader::useUniformBuffer(const Ref<IUniformBuffer> &)
{

}

void SGCore::VkShader::destroy() noexcept
{

}

std::int32_t SGCore::VkShader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    return 0;
}
