//
// Created by stuka on 14.11.2023.
//
#include "IGizmo.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/ECS/Rendering/Pipelines/IRenderPipeline.h"

SGCore::IGizmo::IGizmo() noexcept
{
    m_meshDataRenderInfo.m_enableFacesCulling = false;
    m_meshDataRenderInfo.m_drawMode = SGDrawMode::SGG_LINES;

    auto& shadersPaths = *SGSingleton::getSharedPtrInstance<ShadersPaths>();
    m_meshData->m_material->getShader()->addSubPassShadersAndCompile(
            AssetManager::loadAsset<FileAsset>(shadersPaths["Gizmos"]["LinesGizmosShader"].getCurrentRealization())
    );
}

void SGCore::IGizmo::registerRenderPipelineIfNotRegistered(const Ref <IRenderPipeline>& pipeline) noexcept
{

}

