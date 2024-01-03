//
// Created by stuka on 02.01.2024.
//
#include "Skybox.h"
#include "SGCore/Graphics/API/IShader.h"
#include "Pipelines/IRenderPipeline.h"

void SGCore::Skybox::registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept
{
    m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
            AssetManager::loadAsset<FileAsset>(pipeline->m_shadersPaths["GeometryShader"].getCurrentRealization())
    );
}

