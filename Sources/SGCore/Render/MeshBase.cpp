//
// Created by stuka on 19.01.2024.
//

#include "MeshBase.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "RenderPipelinesManager.h"

SGCore::MeshBase::MeshBase()
{
    m_meshData = Ref<IMeshData>(CoreMain::getRenderer()->createMeshData());
}

void SGCore::MeshBase::onMeshCreated(const std::string& shaderPath) noexcept
{
    m_shaderPath = shaderPath;

    auto renderPipeline = RenderPipelinesManager::getRenderPipeline();

    if(renderPipeline)
    {
        m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
                AssetManager::loadAsset<FileAsset>(
                        renderPipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization()));
    }

    RenderPipelinesManager::subscribeToRenderPipelineSetEvent(m_onRenderPipelineSet);
}

void SGCore::MeshBase::onRenderPipelineSet() noexcept
{
    m_meshData->m_material->getShader()->removeAllSubPassShadersByPath(m_shaderPath);

    auto renderPipeline = RenderPipelinesManager::getRenderPipeline();

    if(renderPipeline)
    {
        m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
                AssetManager::loadAsset<FileAsset>(
                        renderPipeline->m_shadersPaths.getByVirtualPath(m_shaderPath).getCurrentRealization()));
    }
}
