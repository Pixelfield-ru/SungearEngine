//
// Created by stuka on 02.01.2024.
//
#include "Mesh.h"
#include "SGCore/Graphics/API/IShader.h"
#include "Pipelines/IRenderPipeline.h"

void SGCore::Mesh::registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept
{
    if(m_registeredPipelines.contains(pipeline.get())) return;

    m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
            AssetManager::loadAsset<FileAsset>(pipeline->m_shadersPaths["GeometryShader"].getCurrentRealization())
    );
    m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
            AssetManager::loadAsset<FileAsset>(pipeline->m_shadersPaths["ShadowsGenerationShader"].getCurrentRealization())
    );

    m_registeredPipelines.insert(pipeline.get());
}

