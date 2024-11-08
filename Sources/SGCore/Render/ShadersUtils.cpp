//
// Created by stuka on 02.02.2024.
//

#include "ShadersUtils.h"
#include "RenderPipelinesManager.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Scene/Scene.h"
#include "ShaderComponent.h"

void SGCore::ShadersUtils::loadShader(ShaderComponent& shaderComponent, const std::string& shaderPath) noexcept
{
    if(shaderComponent.m_isCustomShader) return;

    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();

    if(renderPipeline)
    {
        auto shaderFile =
                AssetManager::getInstance()->loadAsset<TextFileAsset>(renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization());

        // std::cout << "loaded shader: " << renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization() << std::endl;
        shaderComponent.m_shaderPath = shaderPath;
        shaderComponent.m_shader = AssetManager::getInstance()->getOrAddAssetByAlias<IShader>(shaderFile->getAlias());
        shaderComponent.m_shader->addSubPassShadersAndCompile(shaderFile);
    }
}

void SGCore::ShadersUtils::onRenderPipelineSet(ShaderComponent& shaderComponent) noexcept
{
    if(shaderComponent.m_isCustomShader) return;

    if(shaderComponent.m_shader)
    {
        shaderComponent.m_shader->removeAllSubPassShadersByDiskPath(shaderComponent.m_shaderPath);
    }

    auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();

    if(renderPipeline)
    {
        auto shaderFile =
                AssetManager::getInstance()->loadAsset<TextFileAsset>(renderPipeline->m_shadersPaths.getByVirtualPath(shaderComponent.m_shaderPath).getCurrentRealization());

        shaderComponent.m_shader = AssetManager::getInstance()->getOrAddAssetByAlias<IShader>(shaderFile->getAlias());
        shaderComponent.m_shader->addSubPassShadersAndCompile(shaderFile);
    }
}
