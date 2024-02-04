//
// Created by stuka on 24.12.2023.
//

#include "TexturesBlock.h"
#include "API/ISubPassShader.h"

bool SGCore::TexturesBlock::operator==(
        const SGCore::TexturesBlock& other) const noexcept
{
    return other.m_uniformName == m_uniformName;
}

bool SGCore::TexturesBlock::operator!=(
        const SGCore::TexturesBlock& other) const noexcept
{
    return !(*this == other);
}

void SGCore::TexturesBlock::setParentShader(const SGCore::Ref<SGCore::ISubPassShader>& shader) noexcept
{
    m_parentShader = shader;
    shader->onTexturesCountChanged();
}

std::list<SGCore::Weak<SGCore::ITexture2D>>::const_iterator SGCore::TexturesBlock::eraseTexture(
        const std::list<SGCore::Weak<SGCore::ITexture2D>>::const_iterator& iter) noexcept
{
    return m_textures.erase(iter);
}
