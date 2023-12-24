//
// Created by stuka on 24.12.2023.
//

#include "ShaderTexturesBlock.h"
#include "API/IShader.h"

bool SGCore::ShaderTexturesBlock::operator==(
        const SGCore::ShaderTexturesBlock& other) const noexcept
{
    return other.m_uniformName == m_uniformName;
}

bool SGCore::ShaderTexturesBlock::operator!=(
        const SGCore::ShaderTexturesBlock& other) const noexcept
{
    return !(*this == other);
}

void SGCore::ShaderTexturesBlock::setParentShader(const SGCore::Ref<SGCore::IShader>& shader) noexcept
{
    m_parentShader = shader;
    shader->onTexturesCountChanged();
}
