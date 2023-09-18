//
// Created by Ilya on 18.09.2023.
//

#include "ShaderAsset.h"
#include "SGCore/Main/CoreMain.h"

std::shared_ptr<Core::Memory::Assets::IAsset> Core::Memory::Assets::ShaderAsset::load(const std::string& path)
{
    m_shader = std::shared_ptr<Graphics::IShader>(Core::Main::CoreMain::getRenderer()
            .createShader());
    m_shader->compile(AssetManager::loadAsset<FileAsset>(path));

    return shared_from_this();
}

std::shared_ptr<Core::Graphics::IShader> Core::Memory::Assets::ShaderAsset::getShader() const noexcept
{
    return m_shader;
}

