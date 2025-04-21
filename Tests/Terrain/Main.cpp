//
// Created by stuka on 19.04.2025.
//

#include "Main.h"

#include "SGCore/Main/CoreMain.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Serde/Components/NonSavable.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Terrain.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"

#ifdef PLATFORM_OS_WINDOWS
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
    __declspec(dllexport) DWORD NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif
#endif

SGCore::Ref<SGCore::Scene> scene;
SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::IMeshData> quadMeshData;
SGCore::AssetRef<SGCore::IMeshData> terrainMeshData;
SGCore::AssetRef<SGCore::ModelAsset> testModelAsset;
SGCore::MeshRenderState quadMeshRenderState;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;

SGCore::ECS::entity_t mainCamera;
SGCore::ECS::entity_t terrainEntity;
SGCore::ECS::entity_t atmosphereEntity;

void generateTerrain(const SGCore::AssetRef<SGCore::IMeshData>& terrainMesh, int patchesCountX, int patchesCountY, int patchSize) noexcept
{
    size_t currentIdx = 0;

    glm::vec2 curPos = { 0.0, 0.0 };

    const glm::vec2 patchHalfSize = glm::vec2(patchSize, patchSize) / 2.0f;

    for(int y = 0; y < patchesCountY; ++y)
    {
        for(int x = 0; x < patchesCountX; ++x)
        {
            terrainMesh->m_vertices.push_back({
                .m_position = glm::vec3 { -1, 0, -1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { x / (float) patchesCountX, y / (float) patchesCountY, 0.0 }
            });

            terrainMesh->m_vertices.push_back({
                .m_position = glm::vec3 { -1, 0, 1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { x / (float) patchesCountX, (y + 1) / (float) patchesCountY, 0.0f }
            });

            terrainMesh->m_vertices.push_back({
                .m_position = glm::vec3 { 1, 0, 1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { (x + 1) / (float) patchesCountX, (y + 1) / (float) patchesCountY, 0.0f }
            });

            terrainMesh->m_vertices.push_back({
                .m_position = glm::vec3 { 1, 0, -1.0f } * patchSize / 2.0f + glm::vec3(curPos.x, 0.0, curPos.y),
                .m_uv = { (x + 1) / (float) patchesCountX, y / (float) patchesCountY, 0.0f }
            });

            terrainMeshData->m_indices.push_back(currentIdx + 0);
            terrainMeshData->m_indices.push_back(currentIdx + 3);
            terrainMeshData->m_indices.push_back(currentIdx + 2);
            terrainMeshData->m_indices.push_back(currentIdx + 1);

            currentIdx += 4;

            curPos.x += patchSize;
        }

        curPos.x = 0;
        curPos.y += patchSize;
    }
}

void coreInit()
{
    SGCore::AssetRef<SGCore::ITexture2D> terrainDiffuseTex;
    SGCore::AssetRef<SGCore::ITexture2D> terrainDisplacementTex;
    SGCore::AssetRef<SGCore::ITexture2D> terrainNormalsTex;
    SGCore::AssetRef<SGCore::ITexture2D> terrainAORoughnessMetalTex;
    SGCore::AssetRef<SGCore::ITexture2D> terrainHeightmapTex;

    auto mainAssetManager = SGCore::AssetManager::getInstance();

    // setting pipeline that will render our scene
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    // creating scene
    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    // setting this scene as current
    SGCore::Scene::setCurrentScene(scene);

    auto ecsRegistry = scene->getECSRegistry();

    mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "cube_model",
        "${enginePath}/Resources/models/standard/cube.obj"
    );

    screenShader = mainAssetManager->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");

    terrainDiffuseTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test_terrain/diffuse.png");
    terrainDisplacementTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test_terrain/displacement.png");
    terrainNormalsTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test_terrain/normals.png");
    terrainAORoughnessMetalTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test_terrain/ao_roughness_metal.png");
    terrainHeightmapTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test_heightmap0.png");

    // creating camera entity
    mainCamera = ecsRegistry->create();

    // creating components for entity
    auto cameraTransform = ecsRegistry->emplace<SGCore::Transform>(mainCamera, SGCore::MakeRef<SGCore::Transform>());
    ecsRegistry->emplace<SGCore::NonSavable>(mainCamera);
    ecsRegistry->emplace<SGCore::Camera3D>(mainCamera, SGCore::MakeRef<SGCore::Camera3D>());
    ecsRegistry->emplace<SGCore::RenderingBase>(mainCamera, SGCore::MakeRef<SGCore::RenderingBase>());
    ecsRegistry->emplace<SGCore::Controllable3D>(mainCamera)/*.m_movementSpeed = 100.0f*/;
    auto& cameraReceiver = ecsRegistry->emplace<SGCore::LayeredFrameReceiver>(mainCamera);

    attachmentToDisplay = cameraReceiver.m_layersFXFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7);

    // creating skybox ==============================================================================================

    // creating cubemap and skybox material =====

    auto standardCubemap = mainAssetManager->getOrAddAssetByAlias<SGCore::ICubemapTexture>("standard_skybox0");

    standardCubemap->m_parts.push_back(mainAssetManager->loadAsset<SGCore::ITexture2D>(
            "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
    ));
    standardCubemap->m_parts.push_back(mainAssetManager->loadAsset<SGCore::ITexture2D>(
            "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
    ));

    standardCubemap->m_parts.push_back(mainAssetManager->loadAsset<SGCore::ITexture2D>(
            "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
    ));
    standardCubemap->m_parts.push_back(mainAssetManager->loadAsset<SGCore::ITexture2D>(
            "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
    ));

    standardCubemap->m_parts.push_back(mainAssetManager->loadAsset<SGCore::ITexture2D>(
            "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
    ));
    standardCubemap->m_parts.push_back(mainAssetManager->loadAsset<SGCore::ITexture2D>(
            "${enginePath}/Resources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
    ));

    standardCubemap->create();

    auto standardCubemapMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("standard_skybox_material0");
    standardCubemapMaterial->m_shader =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
    standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;
    standardCubemapMaterial->addTexture2D(SGTextureType::SGTT_SKYBOX, standardCubemap);

    // ======

    std::vector<SGCore::ECS::entity_t> skyboxEntities;
    auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("cube_model");
    cubeModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities](const auto& entity) {
        skyboxEntities.push_back(entity);
        scene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
        scene->getECSRegistry()->remove<SGCore::Pickable>(entity);
        scene->getECSRegistry()->remove<SGCore::TransparentEntityTag>(entity);
    });

    atmosphereEntity = skyboxEntities[2];

    auto& skyboxMesh = scene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
    auto& atmosphereScattering = scene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
    atmosphereScattering.m_sunRotation.x = 0.0;
    skyboxMesh.m_base.setMaterial(standardCubemapMaterial);

    auto& skyboxTransform = scene->getECSRegistry()->get<SGCore::Transform>(atmosphereEntity);

    skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };

    // =================================================================

    // creating terrain mesh ==================================

    const float terrainScale = 100.0f;

    terrainMeshData = mainAssetManager->createAsset<SGCore::IMeshData>();

    generateTerrain(terrainMeshData, 20, 20, 100);

    terrainMeshData->prepare();

    // creating terrain material ==============================

    auto standardTerrainMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("standard_terrain_material");
    standardTerrainMaterial->m_shader =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["StandardTerrainShader"]);
    standardTerrainMaterial->m_meshRenderState.m_useFacesCulling = false;
    standardTerrainMaterial->m_meshRenderState.m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
    //standardTerrainMaterial->m_meshRenderState.m_useIndices = false;
    standardTerrainMaterial->m_meshRenderState.m_drawMode = SGDrawMode::SGG_PATCHES;
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE, terrainDiffuseTex);
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_DISPLACEMENT, terrainDisplacementTex);
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_NORMALS, terrainNormalsTex);
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_LIGHTMAP, terrainAORoughnessMetalTex);
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE_ROUGHNESS, terrainAORoughnessMetalTex);
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_METALNESS, terrainAORoughnessMetalTex);
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_HEIGHT, terrainHeightmapTex);

    // creating terrain material ==============================

    auto testMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("test_material_0");
    testMaterial->m_shader =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["StandardMeshShader"]);
    testMaterial->m_meshRenderState.m_useFacesCulling = false;
    testMaterial->m_meshRenderState.m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
    //standardTerrainMaterial->m_meshRenderState.m_useIndices = false;
    testMaterial->m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    testMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE, terrainDiffuseTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_DISPLACEMENT, terrainDisplacementTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_NORMALS, terrainNormalsTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_LIGHTMAP, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE_ROUGHNESS, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_METALNESS, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_HEIGHT, terrainHeightmapTex);

    // creating terrain entity ================================

    terrainEntity = ecsRegistry->create();

    auto& terrainTransform = ecsRegistry->emplace<SGCore::Transform>(terrainEntity, SGCore::MakeRef<SGCore::Transform>());
    ecsRegistry->emplace<SGCore::NonSavable>(terrainEntity);
    auto& terrainMesh = ecsRegistry->emplace<SGCore::Mesh>(terrainEntity);
    ecsRegistry->emplace<SGCore::Terrain>(terrainEntity);

    // terrainTransform->m_ownTransform.m_scale = { 1.0f, 1.0f, 1.0f };

    terrainMesh.m_base.setMeshData(terrainMeshData);
    terrainMesh.m_base.setMaterial(standardTerrainMaterial);

    // =================================================================
    // ================================================================= test
    // =================================================================

    testModelAsset = mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "quad_test",
        "${enginePath}/Resources/models/standard/cube.obj"
    );

    std::vector<SGCore::ECS::entity_t> testQuadEntities;
    testModelAsset->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&testQuadEntities, &testMaterial](const auto& entity) {
        testQuadEntities.push_back(entity);
        scene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
        scene->getECSRegistry()->remove<SGCore::Pickable>(entity);
        scene->getECSRegistry()->remove<SGCore::TransparentEntityTag>(entity);

        auto* meshComponent = scene->getECSRegistry()->tryGet<SGCore::Mesh>(entity);
        if(meshComponent)
        {
            meshComponent->m_base.setMaterial(testMaterial);
        }
    });

    scene->getECSRegistry()->get<SGCore::Transform>(testQuadEntities[0])->m_ownTransform.m_position.y += 10.0f;

    // =================================================================
    // =================================================================
    // =================================================================

    quadMeshData = SGCore::Ref<SGCore::IMeshData>(SGCore::CoreMain::getRenderer()->createMeshData());

    quadMeshData->m_vertices.resize(4);

    quadMeshData->m_vertices[0] = {
        .m_position = glm::vec3 { -1, -1, 0.0f }
    };

    quadMeshData->m_vertices[1] = {
        .m_position = glm::vec3 { -1, 1, 0.0f }
    };

    quadMeshData->m_vertices[2] = {
        .m_position = glm::vec3 { 1, 1, 0.0f }
    };

    quadMeshData->m_vertices[3] = {
        .m_position = glm::vec3 { 1, -1, 0.0f }
    };

    quadMeshData->m_indices.resize(6);

    quadMeshData->m_indices[0] = 0;
    quadMeshData->m_indices[1] = 2;
    quadMeshData->m_indices[2] = 1;
    quadMeshData->m_indices[3] = 0;
    quadMeshData->m_indices[4] = 3;
    quadMeshData->m_indices[5] = 2;

    quadMeshData->prepare();
}

void onUpdate(const double& dt, const double& fixedDt)
{
    SGCore::CoreMain::getWindow().setTitle("Tesselation Test. FPS: " + std::to_string(SGCore::CoreMain::getFPS()));

    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT))
    {
        scene->getECSRegistry()->get<SGCore::Transform>(mainCamera)->m_ownTransform.m_yawPitchRoll.y -= 0.5f;
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_RIGHT))
    {
        scene->getECSRegistry()->get<SGCore::Transform>(mainCamera)->m_ownTransform.m_yawPitchRoll.y += 0.5f;
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_3))
    {
        auto terrainTransform = scene->getECSRegistry()->get<SGCore::Transform>(terrainEntity);
        // terrainTransform->m_ownTransform.m_rotation = glm::identity<glm::quat>();
        terrainTransform->m_ownTransform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)) * terrainTransform->m_ownTransform.m_rotation;

    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_9))
    {
        auto& atmosphereScattering = scene->getECSRegistry()->get<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.x += 10.0f * dt;
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_0))
    {
        auto& atmosphereScattering = scene->getECSRegistry()->get<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.x -= 10.0f * dt;
    }

    // rendering frame buffer attachment from camera to screen
    screenShader->bind();

    // use this to flip screen output
    screenShader->useInteger("u_flipOutput", false);

    // someTexture->bind(0);
    attachmentToDisplay->bind(0);
    screenShader->useTextureBlock("u_bufferToDisplay", 0);

    SGCore::CoreMain::getRenderer()->renderArray(
        quadMeshData->getVertexArray(),
        quadMeshRenderState,
        quadMeshData->m_vertices.size(),
        quadMeshData->m_indices.size()
    );

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_2))
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
    }
}

void onFixedUpdate(const double& dt, const double& fixedDt)
{
    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->fixedUpdate(dt, fixedDt);
    }
}

int main()
{
    SGCore::CoreMain::onInit.connect<&coreInit>();
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    SGCore::CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    SGCore::CoreMain::start();

    return 0;
}
