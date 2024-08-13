//
// Created by stuka on 07.07.2023.
//

#include "VkSubPassShader.h"

SGCore::VkSubPassShader::~VkSubPassShader() noexcept
{
    destroy();
}

void SGCore::VkSubPassShader::compile(const std::string& subPassName) noexcept
{

}

void SGCore::VkSubPassShader::bind() noexcept
{

}

void SGCore::VkSubPassShader::useUniformBuffer(const Ref<IUniformBuffer> &)
{

}

void SGCore::VkSubPassShader::destroy() noexcept
{

}

std::int32_t SGCore::VkSubPassShader::getShaderUniformLocation(const std::string& uniformName) noexcept
{
    return 0;
}
