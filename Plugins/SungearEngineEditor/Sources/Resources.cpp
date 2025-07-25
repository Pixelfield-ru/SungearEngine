//
// Created by ilya on 28.04.24.
//

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Graphics/API/IRenderer.h>
#include <SGCore/Graphics/API/ICubemapTexture.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Render/RenderPipelinesManager.h>

#include "Resources.h"

void SGE::Resources::loadStandardResources(SGCore::AssetManager& toManager) noexcept
{
    // auto standardCubemap = SGCore::Ref<SGCore::ICubemapTexture>(SGCore::CoreMain::getRenderer()->createCubemapTexture());
    auto standardCubemap = toManager.getOrAddAssetByAlias<SGCore::ICubemapTexture>("standard_skybox0");

    {
        standardCubemap->m_parts.push_back(toManager.loadAsset<SGCore::ITexture2D>(
                "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
        ));
        standardCubemap->m_parts.push_back(toManager.loadAsset<SGCore::ITexture2D>(
                "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
        ));

        standardCubemap->m_parts.push_back(toManager.loadAsset<SGCore::ITexture2D>(
                "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
        ));
        standardCubemap->m_parts.push_back(toManager.loadAsset<SGCore::ITexture2D>(
                "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
        ));

        standardCubemap->m_parts.push_back(toManager.loadAsset<SGCore::ITexture2D>(
                "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
        ));
        standardCubemap->m_parts.push_back(toManager.loadAsset<SGCore::ITexture2D>(
                "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
        ));

        standardCubemap->create();

        auto standardCubemapMaterial = toManager.getOrAddAssetByAlias<SGCore::IMaterial>("standard_skybox_material0");
        standardCubemapMaterial->m_shaders["GeometryPass"] =
                toManager.loadAsset<SGCore::IShader>(
                        *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
        standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;

        auto standardGridMaterial = toManager.getOrAddAssetByAlias<SGCore::IMaterial>("standard_grid_material");
        standardGridMaterial->m_shaders["GeometryPass"] =
                toManager.loadAsset<SGCore::IShader>(*SGCore::Paths::getDefaultPaths()["Shaders/InfiniteGrid"]);
        standardGridMaterial->m_meshRenderState.m_useFacesCulling = false;

        standardCubemapMaterial->addTexture2D(SGTextureType::SGTT_SKYBOX, standardCubemap);
    }

    toManager.loadAssetWithAlias<SGCore::ModelAsset>(
            "cube_model",
            "${enginePath}/Resources/models/standard/cube.obj"
    );

    toManager.loadAssetWithAlias<SGCore::ModelAsset>(
            "plane_model",
            "${enginePath}/Resources/models/standard/plane.fbx"
    );
}

void SGE::Resources::unloadStandardResources(SGCore::AssetManager& fromManager) noexcept
{
    // fromManager.
}

SGCore::Ref<SGCore::AssetManager> SGE::Resources::getEditorOnlyAssetManager() noexcept
{
    return m_editorOnlyAssetManager;
}
