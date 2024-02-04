//
// Created by stuka on 02.02.2024.
//

#include "MeshesUtils.h"
#include "RenderPipelinesManager.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Scene/Scene.h"
#include "MeshBase.h"

void SGCore::MeshesUtils::loadMeshShader(MeshBase& meshBase, const std::string& shaderPath) noexcept
{
    auto renderPipeline = RenderPipelinesManager::getRenderPipeline();

    if(renderPipeline)
    {
        std::cout << "loaded shader: " << renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization() << std::endl;
        meshBase.m_shaderPath = shaderPath;
        meshBase.m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
                AssetManager::loadAsset<FileAsset>(
                        renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization()));
    }
}

void SGCore::MeshesUtils::onRenderPipelineSet(MeshBase& meshBase) noexcept
{
    meshBase.m_meshData->m_material->getShader()->removeAllSubPassShadersByPath(meshBase.m_shaderPath);

    auto renderPipeline = RenderPipelinesManager::getRenderPipeline();

    if(renderPipeline)
    {
        meshBase.m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
                AssetManager::loadAsset<FileAsset>(
                        renderPipeline->m_shadersPaths.getByVirtualPath(meshBase.m_shaderPath
                        ).getCurrentRealization()));
    }
}
