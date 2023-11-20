//
// Created by Ilya on 18.09.2023.
//

#include "ShaderAsset.h"
#include "SGCore/Main/CoreMain.h"

std::shared_ptr<SGCore::IAsset> SGCore::ShaderAsset::load(const std::string& path)
{
    m_shader = std::shared_ptr<IShader>(CoreMain::getRenderer()
            .createShader());
    m_shader->compile(AssetManager::loadAsset<FileAsset>(path));

    return shared_from_this();
}

std::shared_ptr<SGCore::IShader> SGCore::ShaderAsset::getShader() const noexcept
{
    return m_shader;
}

