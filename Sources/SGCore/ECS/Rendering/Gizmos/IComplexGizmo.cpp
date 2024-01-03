//
// Created by stuka on 02.01.2024.
//
#include "IComplexGizmo.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/ECS/Rendering/Pipelines/IRenderPipeline.h"

SGCore::IComplexGizmo::IComplexGizmo() noexcept
{
    auto& shadersPaths = *SGSingleton::getSharedPtrInstance<ShadersPaths>();
    m_meshData->m_material->getShader()->addSubPassShadersAndCompile(AssetManager::loadAsset<FileAsset>(shadersPaths["Gizmos"]["ComplexGizmosShader"].getCurrentRealization()));
}

void SGCore::IComplexGizmo::registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept
{

}

