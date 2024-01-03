//
// Created by stuka on 24.12.2023.
//

#include "ShaderTexturesBlock.h"
#include "API/ISubPassShader.h"

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

void SGCore::ShaderTexturesBlock::setParentShader(const SGCore::Ref<SGCore::ISubPassShader>& shader) noexcept
{
    m_parentShader = shader;
    shader->onTexturesCountChanged();
}

std::list<SGCore::Weak<SGCore::ITexture2D>>::const_iterator SGCore::ShaderTexturesBlock::eraseTexture(
        const std::list<SGCore::Weak<SGCore::ITexture2D>>::const_iterator& iter) noexcept
{
    return m_textures.erase(iter);
}
