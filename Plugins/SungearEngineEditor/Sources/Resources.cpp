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
    auto standardCubemap = toManager.loadAsset<SGCore::ICubemapTexture>("${enginePath}/Resources/skyboxes/standard_skybox0.sgcm");

    {
        auto standardCubemapMaterial = toManager.loadAsset<SGCore::IMaterial>("${enginePath}/Resources/materials/standard_skybox_material0.sgmat");

        auto standardGridMaterial = toManager.getOrAddAssetByAlias<SGCore::IMaterial>("standard_grid_material");
        standardGridMaterial->m_shaders["GeometryPass"] =
                toManager.loadAsset<SGCore::IShader>(*SGCore::Paths::getDefaultPaths()["Shaders/InfiniteGrid"]);
        standardGridMaterial->m_meshRenderState.m_useFacesCulling = false;
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
