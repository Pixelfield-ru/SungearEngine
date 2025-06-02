//
// Created by stuka on 19.04.2025.
//

#include "Main.h"

#include "SGCore/ECS/Utils.h"
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
#include "SGCore/Physics/Rigidbody3D.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Terrain/Terrain.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Physics/PhysicsWorld3D.h"

#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btCompoundShape.h>

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
SGCore::ECS::entity_t terrainDecalEntity;

SGCore::AssetRef<SGCore::ITexture2D> terrainHeightmapTex;

SGCore::AssetRef<SGCore::ITexture2D> terrainDecalDiffuseTex0;
SGCore::AssetRef<SGCore::ITexture2D> terrainDecalDiffuseTex1;
SGCore::AssetRef<SGCore::ITexture2D> terrainDecalAOTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainDecalHeightTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainDecalMetallicTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainDecalNormalTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainDecalRoughnessTex;

SGCore::AssetRef<SGCore::IMaterial> terrainDecalMaterial;

SGCore::AssetRef<SGCore::ITexture2D> terrainDiffuseTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainDisplacementTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainNormalsTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainAORoughnessMetalTex;
SGCore::AssetRef<SGCore::ITexture2D> terrainTilingNoiseTex;

std::vector<float> terrainDisplacementData;

float terrainGrowSpeed = 0.001f;
float terrainLowerSpeed = 0.001f;

enum class TerrainOp
{
    TERRAIN_GROW,
    TERRAIN_LOWER
};

TerrainOp currentTerrainOp = TerrainOp::TERRAIN_GROW;

void createBallAndApplyImpulse(const glm::vec3& spherePos,
    const glm::vec3& impulse) noexcept
{
    auto sphereModel = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("sphere_model");

    std::vector<SGCore::ECS::entity_t> sphereEntities;
    sphereModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME,
        [&sphereEntities](const SGCore::ECS::entity_t& entity)
        {
            sphereEntities.push_back(entity);
        }
    );

    auto sphereRigidbody3D = scene->getECSRegistry()->emplace<SGCore::Rigidbody3D>(sphereEntities[2],
        SGCore::MakeRef<SGCore::Rigidbody3D>(
            scene->getSystem<SGCore::PhysicsWorld3D>()));

    SGCore::Ref<btSphereShape> sphereRigidbody3DShape = SGCore::MakeRef<btSphereShape>(1.0);
    btTransform sphereShapeTransform;
    sphereShapeTransform.setIdentity();
    sphereRigidbody3D->addShape(sphereShapeTransform, sphereRigidbody3DShape);
    sphereRigidbody3D->setType(SGCore::PhysicalObjectType::OT_DYNAMIC);
    sphereRigidbody3D->m_body->setRestitution(0.9);
    btScalar mass = 100.0f;
    btVector3 inertia(0, 0, 0);
    sphereRigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
    sphereRigidbody3D->m_body->setMassProps(mass, inertia);
    sphereRigidbody3D->reAddToWorld();

    glm::vec3 finalImpulse = impulse;
    sphereRigidbody3D->m_body->applyCentralImpulse({ finalImpulse.x, finalImpulse.y, finalImpulse.z });

    SGCore::Ref<SGCore::Transform>& sphereTransform = scene->getECSRegistry()->get<SGCore::Transform>(sphereEntities[0]);
    sphereTransform->m_ownTransform.m_position = spherePos;
}

void regenerateTerrainPhysicalMesh(SGCore::ECS::entity_t terrainEntity)
{
    auto& terrainComponent = scene->getECSRegistry()->get<SGCore::Terrain>(terrainEntity);
    auto& terrainMesh = scene->getECSRegistry()->get<SGCore::Mesh>(terrainEntity);
    auto& terrainRigidbody = scene->getECSRegistry()->get<SGCore::Rigidbody3D>(terrainEntity);

    terrainRigidbody->removeFromWorld();

    // generating terrain physical mesh
    SGCore::Terrain::generatePhysicalMesh(terrainComponent, terrainMesh, 5);

    SGCore::Ref<btBvhTriangleMeshShape> terrainRigidbodyShape = SGCore::MakeRef<btBvhTriangleMeshShape>(terrainMeshData->m_physicalMesh.get(), true);
    btTransform terrainShapeTransform;
    terrainShapeTransform.setIdentity();
    terrainRigidbody->removeAllShapes();
    terrainRigidbody->addShape(terrainShapeTransform, terrainRigidbodyShape);

    terrainRigidbody->reAddToWorld();
}

void coreInit()
{
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

    scene->getSystem<SGCore::PhysicsWorld3D>()->getDebugDraw()->setDebugMode(btIDebugDraw::DBG_NoDebug);

    auto ecsRegistry = scene->getECSRegistry();

    mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "cube_model",
        "${enginePath}/Resources/models/standard/cube.obj"
    );

    mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "sphere_model",
        "${enginePath}/Resources/models/standard/sphere.obj"
    );

    screenShader = mainAssetManager->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");

    // terrainDecalDiffuseTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/spotted_rust/spotted-rust_albedo.png");
    terrainDecalDiffuseTex0 = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/terrain_editor/grow.png");
    terrainDecalDiffuseTex1 = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/terrain_editor/lower.png");
    terrainDecalAOTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/spotted_rust/spotted-rust_ao.png");
    terrainDecalHeightTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/spotted_rust/spotted-rust_height.png");
    terrainDecalMetallicTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/spotted_rust/spotted-rust_metallic.png");
    terrainDecalNormalTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/spotted_rust/spotted-rust_normal-ogl.png");
    terrainDecalRoughnessTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/spotted_rust/spotted-rust_roughness.png");

    terrainDiffuseTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/test_terrain/diffuse.png");
    terrainHeightmapTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/test_terrain/displacement.png");
    terrainNormalsTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/test_terrain/normals.png");
    terrainAORoughnessMetalTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/test_terrain/ao_roughness_metal.png");
    terrainTilingNoiseTex = mainAssetManager->loadAsset<SGCore::ITexture2D>(SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Resources/textures/test_terrain/tiling_noise.png");
    // terrainDisplacementTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test_heightmap0.png");
    terrainDisplacementTex = mainAssetManager->getOrAddAssetByAlias<SGCore::ITexture2D>("test_heightmap");

    terrainDisplacementData.resize(1000 * 1000);

    terrainDisplacementTex->create(terrainDisplacementData.data(), 1000, 1000, 1, SGGColorInternalFormat::SGG_R32_FLOAT, SGGColorFormat::SGG_R);

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
    auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("sphere_model");
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

    skyboxTransform->m_ownTransform.m_scale = { 2000, 2000, 2000 };

    // =================================================================

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
    standardTerrainMaterial->addTexture2D(SGTextureType::SGTT_NOISE, terrainTilingNoiseTex);

    // creating terrain material ==============================

    auto testMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("test_material_0");
    testMaterial->m_shader =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["StandardMeshShader"]);
    testMaterial->m_meshRenderState.m_useFacesCulling = false;
    testMaterial->m_meshRenderState.m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
    //standardTerrainMaterial->m_meshRenderState.m_useIndices = false;
    testMaterial->m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    /*testMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE, terrainDiffuseTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_DISPLACEMENT, terrainDisplacementTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_NORMALS, terrainNormalsTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_LIGHTMAP, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE_ROUGHNESS, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_METALNESS, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureType::SGTT_HEIGHT, terrainHeightmapTex);*/

    // creating terrain entity ================================

    terrainEntity = ecsRegistry->create();

    auto& terrainTransform = ecsRegistry->emplace<SGCore::Transform>(terrainEntity, SGCore::MakeRef<SGCore::Transform>());
    // terrainTransform->m_ownTransform.m_scale = { 0.1, 0.1, 0.1 };
    ecsRegistry->emplace<SGCore::NonSavable>(terrainEntity);
    auto& terrainMesh = ecsRegistry->emplace<SGCore::Mesh>(terrainEntity);
    auto& terrainComponent = ecsRegistry->emplace<SGCore::Terrain>(terrainEntity);

    // creating terrain mesh ====
    terrainMeshData = mainAssetManager->createAsset<SGCore::IMeshData>();

    SGCore::Terrain::generate(terrainComponent, terrainMeshData, 40, 40, 100);

    terrainMesh.m_base.setMeshData(terrainMeshData);
    terrainMesh.m_base.setMaterial(standardTerrainMaterial);

    // creating rigidbody for terrain
    auto terrainRigidbody = scene->getECSRegistry()->emplace<SGCore::Rigidbody3D>(terrainEntity,
        SGCore::MakeRef<SGCore::Rigidbody3D>(scene->getSystem<SGCore::PhysicsWorld3D>()));

    // generating terrain physical mesh
    SGCore::Terrain::generatePhysicalMesh(terrainComponent, terrainMesh, 10);

    SGCore::Ref<btBvhTriangleMeshShape> terrainRigidbodyShape = SGCore::MakeRef<btBvhTriangleMeshShape>(terrainMeshData->m_physicalMesh.get(), true);
    btTransform terrainShapeTransform;
    terrainShapeTransform.setIdentity();
    terrainRigidbody->addShape(terrainShapeTransform, terrainRigidbodyShape);
    btScalar mass = 0.0f;
    btVector3 inertia(0, 0, 0);
    terrainRigidbody->m_body->setMassProps(mass, inertia);
    terrainRigidbody->reAddToWorld();

    // ==========================

    // terrainTransform->m_ownTransform.m_scale = { 1.0f, 1.0f, 1.0f };

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

    // creating decal !!! ==============================================

    terrainDecalEntity = SGCore::ECS::Utils::createDecal(*ecsRegistry.get());

    terrainDecalMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("terrain_decal_material_0");
    terrainDecalMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE, terrainDecalDiffuseTex0);
    /*terrainDecalMaterial->addTexture2D(SGTextureType::SGTT_LIGHTMAP, terrainDecalAOTex);
    terrainDecalMaterial->addTexture2D(SGTextureType::SGTT_HEIGHT, terrainDecalHeightTex);
    terrainDecalMaterial->addTexture2D(SGTextureType::SGTT_METALNESS, terrainDecalMetallicTex);
    terrainDecalMaterial->addTexture2D(SGTextureType::SGTT_NORMALS, terrainDecalNormalTex);
    terrainDecalMaterial->addTexture2D(SGTextureType::SGTT_DIFFUSE_ROUGHNESS, terrainDecalRoughnessTex);*/
    terrainDecalMaterial->setMetallicFactor(0.0f);
    terrainDecalMaterial->setShininess(0.0f);
    terrainDecalMaterial->setRoughnessFactor(1.0f);

    auto& decalMesh = ecsRegistry->get<SGCore::Mesh>(terrainDecalEntity);
    decalMesh.m_base.setMaterial(terrainDecalMaterial);

    auto& decalTransform = ecsRegistry->get<SGCore::Transform>(terrainDecalEntity);
    // decalTransform->m_ownTransform.m_scale.x *= 10.0;
    // decalTransform->m_ownTransform.m_scale.z *= 10.0;
    // decalTransform->m_ownTransform.m_scale.y = 5.0;

    // =================================================================

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

    const auto mainInputListener = SGCore::InputManager::getMainInputListener();

    /*if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT))
    {
        scene->getECSRegistry()->get<SGCore::Transform>(mainCamera)->m_ownTransform.m_yawPitchRoll.y -= 0.5f;
    }

    if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_RIGHT))
    {
        scene->getECSRegistry()->get<SGCore::Transform>(mainCamera)->m_ownTransform.m_yawPitchRoll.y += 0.5f;
    }*/

    auto& decalTransform = scene->getECSRegistry()->get<SGCore::Transform>(terrainDecalEntity);
    auto& terrainTransform = scene->getECSRegistry()->get<SGCore::Transform>(terrainEntity);
    auto& terrain = scene->getECSRegistry()->get<SGCore::Terrain>(terrainEntity);

    if(mainInputListener->keyboardKeyReleased(SGCore::KeyboardKey::KEY_3))
    {
        // terrainTransform->m_ownTransform.m_rotation = glm::identity<glm::quat>();
        terrainTransform->m_ownTransform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)) * terrainTransform->m_ownTransform.m_rotation;

    }

    if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_9))
    {
        auto& atmosphereScattering = scene->getECSRegistry()->get<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.x += 10.0f * dt;
    }

    if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_0))
    {
        auto& atmosphereScattering = scene->getECSRegistry()->get<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.x -= 10.0f * dt;
    }

    {
        auto& layeredFrameReceiver = scene->getECSRegistry()->get<SGCore::LayeredFrameReceiver>(mainCamera);

        int windowSizeX;
        int windowSizeY;
        SGCore::CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

        const glm::vec2 cursorPos {
            mainInputListener->getCursorPositionX(),
            windowSizeY - mainInputListener->getCursorPositionY()
        };

        const auto& attachment4 = layeredFrameReceiver.m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4);

        const glm::vec2 cursorRelativePos = {
            cursorPos.x / windowSizeX * (attachment4->getWidth()),
            cursorPos.y / windowSizeY * (attachment4->getHeight())
        };

        const auto worldPos = layeredFrameReceiver.m_layersFrameBuffer->readPixelsFromAttachment(cursorRelativePos, SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT4);
        const auto surfaceNormal = layeredFrameReceiver.m_layersFrameBuffer->readPixelsFromAttachment(cursorRelativePos, SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT6);

        /*std::cout << "pos: " << worldPos << ", cursor pos: " << cursorRelativePos <<
                ", attachment4->getWidth(): " << attachment4->getWidth() <<
                ", attachment4->getHeight(): " << attachment4->getHeight() <<
                ", surface normal: " << surfaceNormal <<
                std::endl;*/

        glm::vec3 zDir = glm::normalize(surfaceNormal);
        glm::vec3 up = glm::vec3(0.0f, 1.0f, 0.0f);

        // fixing normal
        if (glm::abs(glm::dot(zDir, up)) > 0.999f)
            up = glm::vec3(1.0f, 0.0f, 0.0f);

        glm::vec3 xDir = glm::normalize(glm::cross(up, zDir));
        glm::vec3 yDir = glm::cross(zDir, xDir);

        glm::mat3 rotationMatrix(xDir, yDir, zDir);
        glm::quat rotation = glm::quat_cast(rotationMatrix);

        decalTransform->m_ownTransform.m_position = worldPos;
        decalTransform->m_ownTransform.m_rotation = rotation * glm::angleAxis(glm::radians(-90.0f), glm::vec3 { 1.0f, 0.0f, 0.0f });

        if(mainInputListener->mouseButtonDown(SGCore::MouseButton::MOUSE_BUTTON_LEFT))
        {
            const glm::vec3 decalRelativePos = worldPos - terrainTransform->m_finalTransform.m_position + glm::vec3 { terrain.getPatchSize() / 2.0f, 0.0f, terrain.getPatchSize() / 2.0f };

            const glm::vec2 pixelSize {
                (terrain.getPatchSize() * terrain.getSize().x) / (float) terrainDisplacementTex->getWidth(),
                (terrain.getPatchSize() * terrain.getSize().y) / (float) terrainDisplacementTex->getHeight()
            };

            const glm::ivec2 indices { std::floor(decalRelativePos.x / pixelSize.x), std::floor(decalRelativePos.z / pixelSize.y) };

            const float paintRadius = (decalTransform->m_ownTransform.m_scale.x / pixelSize.x);
            const int growRegionHalfSizeX = (int) paintRadius;
            const int growRegionHalfSizeY = (int) paintRadius;

            switch(currentTerrainOp)
            {
                case TerrainOp::TERRAIN_GROW:
                {
                    for(int y = -growRegionHalfSizeY; y < growRegionHalfSizeY + 1; ++y)
                    {
                        for(int x = -growRegionHalfSizeX; x < growRegionHalfSizeX + 1; ++x)
                        {
                            if(glm::length(glm::vec2 { x, y }) < paintRadius)
                            {
                                const size_t finalIndex = indices.x + x + (indices.y + y) * terrainDisplacementTex->getWidth();
                                if(finalIndex > 0 && finalIndex < terrainDisplacementData.size())
                                {
                                    terrainDisplacementData[finalIndex] += terrainGrowSpeed;
                                }
                            }
                        }
                    }

                    break;
                }
                case TerrainOp::TERRAIN_LOWER:
                {
                    for(int y = -growRegionHalfSizeY; y < growRegionHalfSizeY + 1; ++y)
                    {
                        for(int x = -growRegionHalfSizeX; x < growRegionHalfSizeX + 1; ++x)
                        {
                            if(glm::length(glm::vec2 { x, y }) < paintRadius)
                            {
                                const size_t finalIndex = indices.x + x + (indices.y + y) * terrainDisplacementTex->getWidth();
                                if(finalIndex > 0 && finalIndex < terrainDisplacementData.size())
                                {
                                    terrainDisplacementData[finalIndex] -= terrainLowerSpeed;
                                }
                            }
                        }
                    }

                    break;
                }
            }

            terrainDisplacementTex->bind(0);
            terrainDisplacementTex->subTextureData(terrainDisplacementData.data(), terrainDisplacementTex->getWidth(), terrainDisplacementTex->getHeight(), 0, 0);
        }

        if(mainInputListener->keyboardKeyReleased(SGCore::KeyboardKey::KEY_F1))
        {
            currentTerrainOp = TerrainOp::TERRAIN_GROW;
            terrainDecalMaterial->replaceTexture(SGTextureType::SGTT_DIFFUSE, 0, terrainDecalDiffuseTex0);
        }
        else if(mainInputListener->keyboardKeyReleased(SGCore::KeyboardKey::KEY_F2))
        {
            currentTerrainOp = TerrainOp::TERRAIN_LOWER;
            terrainDecalMaterial->replaceTexture(SGTextureType::SGTT_DIFFUSE, 0, terrainDecalDiffuseTex1);
        }

        if(mainInputListener->keyboardKeyReleased(SGCore::KeyboardKey::KEY_PAGE_UP))
        {
            switch(currentTerrainOp)
            {
                case TerrainOp::TERRAIN_GROW:
                {
                    terrainGrowSpeed += 0.001f;
                    std::cout << "current terrain grow speed: " << terrainGrowSpeed << std::endl;
                    break;
                }
                case TerrainOp::TERRAIN_LOWER:
                {
                    terrainLowerSpeed += 0.001f;
                    std::cout << "current terrain lower speed: " << terrainLowerSpeed << std::endl;
                    break;
                }
            }
        }

        if(mainInputListener->keyboardKeyReleased(SGCore::KeyboardKey::KEY_PAGE_DOWN))
        {
            switch(currentTerrainOp)
            {
                case TerrainOp::TERRAIN_GROW:
                {
                    terrainGrowSpeed -= 0.001f;
                    std::cout << "current terrain grow speed: " << terrainGrowSpeed << std::endl;
                    break;
                }
                case TerrainOp::TERRAIN_LOWER:
                {
                    terrainLowerSpeed -= 0.001f;
                    std::cout << "current terrain lower speed: " << terrainLowerSpeed << std::endl;
                    break;
                }
            }
        }

        if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_EQUAL))
        {
            decalTransform->m_ownTransform.m_scale += 0.1f;
        }
        else if(mainInputListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_MINUS))
        {
            decalTransform->m_ownTransform.m_scale -= 0.1f;
        }
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

    if (SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_4))
    {
        auto& cameraTransform = scene->getECSRegistry()->get<SGCore::Transform>(mainCamera);
        createBallAndApplyImpulse(cameraTransform->m_ownTransform.m_position, cameraTransform->m_ownTransform.m_forward * 200000.0f / 10.0f);
    }

    if (SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_5))
    {
        regenerateTerrainPhysicalMesh(terrainEntity);
    }

    if (SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_6))
    {
        if(scene->getSystem<SGCore::PhysicsWorld3D>()->getDebugDraw()->getDebugMode() != btIDebugDraw::DBG_NoDebug)
        {
            scene->getSystem<SGCore::PhysicsWorld3D>()->getDebugDraw()->setDebugMode(btIDebugDraw::DBG_NoDebug);
        }
        else
        {
            scene->getSystem<SGCore::PhysicsWorld3D>()->getDebugDraw()->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
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
