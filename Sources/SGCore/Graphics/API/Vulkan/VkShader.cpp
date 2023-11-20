//
// Created by stuka on 07.07.2023.
//

#include "VkShader.h"

SGCore::VkShader::~VkShader() noexcept
{
    destroy();
}

void SGCore::VkShader::compile(std::shared_ptr<FileAsset>) noexcept
{

}

void SGCore::VkShader::bind() noexcept
{

}

void SGCore::VkShader::useUniformBuffer(const std::shared_ptr<IUniformBuffer> &)
{

}

void SGCore::VkShader::destroy() noexcept
{

}

std::int32_t SGCore::VkShader::getShaderUniformLocation(const std::string& uniformName) const noexcept
{
    return 0;
}
