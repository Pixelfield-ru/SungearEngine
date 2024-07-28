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
        // std::cout << "loaded shader: " << renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization() << std::endl;
        shaderComponent.m_shaderPath = shaderPath;
        shaderComponent.m_shader->addSubPassShadersAndCompile(
                AssetManager::getInstance()->loadAsset<TextFileAsset>(
                        renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization()));
    }
}

void SGCore::ShadersUtils::onRenderPipelineSet(ShaderComponent& shaderComponent) noexcept
{
    if(shaderComponent.m_isCustomShader) return;

    if(shaderComponent.m_shader)
    {
        shaderComponent.m_shader->removeAllSubPassShadersByDiskPath(shaderComponent.m_shaderPath);
        
        auto renderPipeline = RenderPipelinesManager::getCurrentRenderPipeline();
        
        if(renderPipeline)
        {
            shaderComponent.m_shader->addSubPassShadersAndCompile(
                    AssetManager::getInstance()->loadAsset<TextFileAsset>(
                            renderPipeline->m_shadersPaths.getByVirtualPath(shaderComponent.m_shaderPath
                            ).getCurrentRealization()));
        }
    }
}
