//
// Created by Ilya on 18.09.2023.
//

#include "ShaderAsset.h"
#include "SGCore/Main/CoreMain.h"

SGCore::Ref<SGCore::IAsset> SGCore::ShaderAsset::load(const std::string& path)
{
    m_shader = Ref<IShader>(CoreMain::getRenderer()
            .createShader(path))->addToGlobalStorage();

    return shared_from_this();
}

SGCore::Ref<SGCore::IShader> SGCore::ShaderAsset::getShader() const noexcept
{
    return m_shader;
}

