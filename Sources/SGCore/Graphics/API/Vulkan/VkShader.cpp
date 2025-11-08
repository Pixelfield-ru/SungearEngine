//
// Created by stuka on 07.07.2023.
//

#include "VkShader.h"

SGCore::VkShader::~VkShader() noexcept
{
    destroy();
}

void SGCore::VkShader::destroy() noexcept
{

}

void SGCore::VkShader::doCompile() noexcept
{

}

void SGCore::VkShader::bind() const noexcept
{

}

void SGCore::VkShader::useUniformBuffer(const Ref<IUniformBuffer> &)
{

}

std::int32_t SGCore::VkShader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    return 0;
}
