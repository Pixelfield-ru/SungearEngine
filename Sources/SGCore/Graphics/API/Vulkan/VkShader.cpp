//
// Created by stuka on 07.07.2023.
//

#include "VkShader.h"

Core::Graphics::VkShader::~VkShader() noexcept
{
    destroy();
}

void Core::Graphics::VkShader::compile(std::shared_ptr<Memory::Assets::FileAsset>) noexcept
{

}

void Core::Graphics::VkShader::bind() noexcept
{

}

void Core::Graphics::VkShader::destroy() noexcept
{

}

std::int32_t Core::Graphics::VkShader::getShaderUniformLocation(const std::string& uniformName) const noexcept
{
    return 0;
}
