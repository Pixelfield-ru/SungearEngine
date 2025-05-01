//
// Created by stuka on 01.05.2025.
//

#include "PBRRPDecalsPass.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Render/DisableMeshGeometryPass.h"
#include "SGCore/Render/IRenderPipeline.h"
#include "SGCore/Render/Decals/Decal.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/RenderingBase.h"

void SGCore::PBRRPDecalsPass::create(const Ref<IRenderPipeline>& parentRenderPipeline)
{
    auto shaderFile = AssetManager::getInstance()->loadAsset<TextFileAsset>(
                *parentRenderPipeline->m_shadersPaths["StandardDecalShader"]);

    m_shader = AssetManager::getInstance()->loadAsset<IShader>(shaderFile->getPath());
}

void SGCore::PBRRPDecalsPass::render(const Ref<Scene>& scene, const Ref<IRenderPipeline>& renderPipeline)
{
    auto registry = scene->getECSRegistry();

    auto camerasView = registry->view<EntityBaseInfo, RenderingBase, Transform>();
    auto decalsView = registry->view<EntityBaseInfo, Transform, Decal, Mesh>(ECS::ExcludeTypes<DisableMeshGeometryPass>{});

    camerasView.each([decalsView, this](const ECS::entity_t& cameraEntity,
                                        const EntityBaseInfo::reg_t& camera3DBaseInfo,
                                        RenderingBase::reg_t& cameraRenderingBase, Transform::reg_t& cameraTransform) {
        decalsView.each([this](const EntityBaseInfo::reg_t& decalInfo,
                               const Transform::reg_t& decalTransform,
                               const Decal::reg_t& decal,
                               const Mesh::reg_t& decalMesh) {

        });
    });
}
