//
// Created by stuka on 02.01.2024.
//
#include "Mesh.h"
#include "SGCore/Graphics/API/IShader.h"
#include "Pipelines/IRenderPipeline.h"

SGCore::Mesh::Mesh(const SGCore::Ref<SGCore::IRenderPipeline>& pipeline) noexcept
{
    loadShader(pipeline, "StandardMeshShader");
}

void SGCore::Mesh::loadShader(const SGCore::Ref<SGCore::IRenderPipeline>& pipeline, const std::string& shaderPath) noexcept
{
    m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
            AssetManager::loadAsset<FileAsset>(pipeline->m_shadersPaths.getByVirtualPath(shaderPath).getCurrentRealization()));
}
