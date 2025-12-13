//
// Created by stuka on 19.04.2025.
//

#include "Main.h"

#include <stb_image_write.h>
#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/string_cast.hpp>

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
#include "SGCore/Input/PCInput.h"
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

#include "SGCore/Navigation/NavGrid3D.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"
#include "SGCore/Navigation/NavMesh/Steps/InputFilteringStep.h"
#include "SGCore/Navigation/NavMesh/Steps/VoxelizationStep.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/Terrain/TerrainUtils.h"

#if SG_PLATFORM_OS_WINDOWS
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

std::vector<SGCore::Vertex> terrainDisplacedVertices;
std::vector<std::uint32_t> terrainDisplacedIndices;

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
    terrainComponent.generatePhysicalMesh(terrainMesh, 5);

    SGCore::Ref<btBvhTriangleMeshShape> terrainRigidbodyShape = SGCore::MakeRef<btBvhTriangleMeshShape>(terrainMeshData->m_physicalMesh.get(), true);
    btTransform terrainShapeTransform;
    terrainShapeTransform.setIdentity();
    terrainRigidbody->removeAllShapes();
    terrainRigidbody->addShape(terrainShapeTransform, terrainRigidbodyShape);

    terrainRigidbody->reAddToWorld();
}

void regenerateTerrainNavGrid(SGCore::ECS::entity_t terrainEntity)
{
    terrainDisplacedVertices.clear();
    terrainDisplacedIndices.clear();

    const auto ecsRegistry = scene->getECSRegistry();

    auto& terrainComponent = ecsRegistry->get<SGCore::Terrain>(terrainEntity);
    // auto& terrainNavGrid = ecsRegistry->get<SGCore::Navigation::NavGrid3D>(terrainEntity);
    auto& terrainNavMesh = ecsRegistry->get<SGCore::Navigation::NavMesh>(terrainEntity);

    SGCore::TerrainUtils::calculateVerticesUsingDisplacementMap(terrainComponent, terrainDisplacementTex.get(), 5, terrainDisplacedVertices, terrainDisplacedIndices);

    std::vector<SGCore::MathPrimitivesUtils::Triangle<>> inputTriangles;

    for(size_t i = 0; i < terrainDisplacedVertices.size(); i += 4)
    {
        const auto& v0 = terrainDisplacedVertices[terrainDisplacedIndices[i] + 0];
        const auto& v1 = terrainDisplacedVertices[terrainDisplacedIndices[i] + 1];
        const auto& v2 = terrainDisplacedVertices[terrainDisplacedIndices[i] + 2];
        const auto& v3 = terrainDisplacedVertices[terrainDisplacedIndices[i] + 3];

        SGCore::MathPrimitivesUtils::Triangle<> tri0 {
            .m_vertices = { v0.m_position, v1.m_position, v2.m_position }
        };

        SGCore::MathPrimitivesUtils::Triangle<> tri1 {
            .m_vertices = { v1.m_position, v2.m_position, v3.m_position }
        };

        tri0.calculateNormal();
        tri1.calculateNormal();

        inputTriangles.push_back(tri0);
        inputTriangles.push_back(tri1);
    }

    terrainNavMesh.useStandardSteps();
    terrainNavMesh.build(inputTriangles);


    // terrainNavGrid.build(terrainDisplacementTex.get(), terrainComponent.m_heightScale, terrainMeshData->m_aabb, *ecsRegistry);
    // terrainNavGrid.build(terrainDisplacedVertices, terrainDisplacedIndices, 4, terrainMeshData->m_aabb, *ecsRegistry);

    // std::cout << "terrain nav grid nodes count: " << terrainNavGrid.m_nodes.size() << std::endl;
}

void coreInit()
{
    auto mainAssetManager = SGCore::AssetManager::getInstance();

    auto& pipelinesManager = SGCore::RenderPipelinesManager::instance();

    // setting pipeline that will render our scene
    auto pbrrpPipeline = pipelinesManager.createRenderPipeline<SGCore::PBRRenderPipeline>();
    pipelinesManager.registerRenderPipeline(pbrrpPipeline);
    pipelinesManager.setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

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

    if(std::filesystem::exists("terrain_displacement.bin"))
    {
        size_t texByteSize = 0;
        const auto* texBinContent = SGCore::FileUtils::readBytes("terrain_displacement.bin", texByteSize);

        terrainDisplacementData.resize(1000 * 1000);

        memcpy(terrainDisplacementData.data(), texBinContent, texByteSize);

        delete texBinContent;

        /*terrainDisplacementTex = mainAssetManager->loadAsset<SGCore::ITexture2D>("terrain_displacement.hdr");

        terrainDisplacementData.resize(1000 * 1000);

        memcpy(terrainDisplacementData.data(), terrainDisplacementTex->getData(), terrainDisplacementTex->getWidth() * terrainDisplacementTex->getHeight() * sizeof(float));*/
        // terrainDisplacementTex->getData()
    }
    else
    {
        terrainDisplacementData.resize(1000 * 1000);
    }

    terrainDisplacementTex->m_dataType = SGGDataType::SGG_FLOAT;
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
    standardCubemapMaterial->m_shaders["GeometryPass"] =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *pipelinesManager.getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
    standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;
    standardCubemapMaterial->addTexture2D(SGTextureSlot::SGTT_SKYBOX, standardCubemap);

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
    standardTerrainMaterial->m_shaders["GeometryPass"] =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *pipelinesManager.getCurrentRenderPipeline()->m_shadersPaths["StandardTerrainShader"]);
    standardTerrainMaterial->m_meshRenderState.m_useFacesCulling = false;
    standardTerrainMaterial->m_meshRenderState.m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
    //standardTerrainMaterial->m_meshRenderState.m_useIndices = false;
    standardTerrainMaterial->m_meshRenderState.m_drawMode = SGDrawMode::SGG_PATCHES;
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE, terrainDiffuseTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_DISPLACEMENT, terrainDisplacementTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_NORMALS, terrainNormalsTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_LIGHTMAP, terrainAORoughnessMetalTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS, terrainAORoughnessMetalTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_METALNESS, terrainAORoughnessMetalTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_HEIGHT, terrainHeightmapTex);
    standardTerrainMaterial->addTexture2D(SGTextureSlot::SGTT_NOISE, terrainTilingNoiseTex);

    // creating terrain material ==============================

    auto testMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("test_material_0");
    testMaterial->m_shaders["GeometryPass"] =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *pipelinesManager.getCurrentRenderPipeline()->m_shadersPaths["StandardMeshShader"]);
    testMaterial->m_meshRenderState.m_useFacesCulling = false;
    testMaterial->m_meshRenderState.m_facesCullingPolygonsOrder = SGPolygonsOrder::SGG_CCW;
    //standardTerrainMaterial->m_meshRenderState.m_useIndices = false;
    testMaterial->m_meshRenderState.m_drawMode = SGDrawMode::SGG_TRIANGLES;
    /*testMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE, terrainDiffuseTex);
    testMaterial->addTexture2D(SGTextureSlot::SGTT_DISPLACEMENT, terrainDisplacementTex);
    testMaterial->addTexture2D(SGTextureSlot::SGTT_NORMALS, terrainNormalsTex);
    testMaterial->addTexture2D(SGTextureSlot::SGTT_LIGHTMAP, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureSlot::SGTT_METALNESS, terrainAORoughnessMetalTex);
    testMaterial->addTexture2D(SGTextureSlot::SGTT_HEIGHT, terrainHeightmapTex);*/

    // creating terrain entity ================================

    terrainEntity = ecsRegistry->create();

    auto& terrainTransform = ecsRegistry->emplace<SGCore::Transform>(terrainEntity, SGCore::MakeRef<SGCore::Transform>());
    // terrainTransform->m_ownTransform.m_scale = { 0.1, 0.1, 0.1 };
    ecsRegistry->emplace<SGCore::NonSavable>(terrainEntity);
    auto& terrainMesh = ecsRegistry->emplace<SGCore::Mesh>(terrainEntity);
    auto& terrainComponent = ecsRegistry->emplace<SGCore::Terrain>(terrainEntity);
    // auto& terrainNavGrid = ecsRegistry->emplace<SGCore::Navigation::NavGrid3D>(terrainEntity);
    auto& terrainNavMesh = ecsRegistry->emplace<SGCore::Navigation::NavMesh>(terrainEntity);
    terrainNavMesh.m_config.m_cellHeight = 5.0f;
    terrainNavMesh.m_config.m_cellSize = 5.0f;

    // creating terrain mesh ====
    terrainMeshData = mainAssetManager->createAndAddAsset<SGCore::IMeshData>();

    terrainComponent.generate(terrainMeshData, 40, 40, 100);

    terrainMesh.m_base.setMeshData(terrainMeshData);
    terrainMesh.m_base.setMaterial(standardTerrainMaterial);

    std::cout << "terrain aabb min: " << glm::to_string(terrainMeshData->m_aabb.m_min) << ", terrain aabb max: " << glm::to_string(terrainMeshData->m_aabb.m_max) << std::endl;

    // terrainNavGrid.m_cellSize = 10.0f;

    regenerateTerrainNavGrid(terrainEntity);

    // creating rigidbody for terrain
    auto terrainRigidbody = scene->getECSRegistry()->emplace<SGCore::Rigidbody3D>(terrainEntity,
        SGCore::MakeRef<SGCore::Rigidbody3D>(scene->getSystem<SGCore::PhysicsWorld3D>()));

    // generating terrain physical mesh
    terrainComponent.generatePhysicalMesh(terrainMesh, 10);

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
    terrainDecalMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE, terrainDecalDiffuseTex0);
    /*terrainDecalMaterial->addTexture2D(SGTextureSlot::SGTT_LIGHTMAP, terrainDecalAOTex);
    terrainDecalMaterial->addTexture2D(SGTextureSlot::SGTT_HEIGHT, terrainDecalHeightTex);
    terrainDecalMaterial->addTexture2D(SGTextureSlot::SGTT_METALNESS, terrainDecalMetallicTex);
    terrainDecalMaterial->addTexture2D(SGTextureSlot::SGTT_NORMALS, terrainDecalNormalTex);
    terrainDecalMaterial->addTexture2D(SGTextureSlot::SGTT_DIFFUSE_ROUGHNESS, terrainDecalRoughnessTex);*/
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

void saveTerrainDisplacementMap() noexcept
{
    const int displacementTexWidth = terrainDisplacementTex->getWidth();
    const int displacementTexHeight = terrainDisplacementTex->getHeight();

    SGCore::FileUtils::writeBytes("terrain_displacement.bin", 0, reinterpret_cast<char*>(terrainDisplacementTex->getData()), displacementTexWidth * displacementTexHeight * sizeof(float) ,false);

    /*std::vector<float> rgbData;
    rgbData.resize(displacementTexWidth * displacementTexHeight * 1);

    for(int i = 0; i < displacementTexWidth * displacementTexHeight; ++i)
    {
        rgbData[i] = terrainDisplacementData[i];
    }

    int result = stbi_write_hdr("terrain_displacement.hdr", displacementTexWidth, displacementTexHeight, 1, rgbData.data());
    std::cout << "result of saving displacement: " << result << std::endl;*/
}

void onUpdate(const double& dt, const double& fixedDt)
{
    SGCore::CoreMain::getWindow().setTitle("Tesselation Test. FPS: " + std::to_string(SGCore::CoreMain::getFPS()));

    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);

        SGCore::CoreMain::getRenderer()->renderTextureOnScreen(attachmentToDisplay.get());
    }

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
    // auto& terrainNavGrid = scene->getECSRegistry()->get<SGCore::Navigation::NavGrid3D>(terrainEntity);
    auto& terrainNavMesh = scene->getECSRegistry()->get<SGCore::Navigation::NavMesh>(terrainEntity);

    const auto debugDraw = SGCore::RenderPipelinesManager::instance().getCurrentRenderPipeline()->getRenderPass<SGCore::DebugDraw>();
    const auto& physicsDebugDraw = scene->getSystem<SGCore::PhysicsWorld3D>()->getDebugDraw();

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_3))
    {
        // terrainTransform->m_ownTransform.m_rotation = glm::identity<glm::quat>();
        terrainTransform->m_ownTransform.m_rotation = glm::angleAxis(glm::radians(90.0f), glm::vec3(0, 0, 1)) * terrainTransform->m_ownTransform.m_rotation;

    }

    if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_9))
    {
        auto& atmosphereScattering = scene->getECSRegistry()->get<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.x += 10.0f * dt;
    }

    if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_0))
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
            SGCore::Input::PC::getCursorPositionX(),
            windowSizeY - SGCore::Input::PC::getCursorPositionY()
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

        if(SGCore::Input::PC::mouseButtonDown(SGCore::Input::MouseButton::MOUSE_BUTTON_LEFT))
        {
            const glm::vec3 decalRelativePos = worldPos - terrainTransform->m_finalTransform.m_position + glm::vec3 { terrain.getPatchSize() / 2.0f, 0.0f, terrain.getPatchSize() / 2.0f };

            const glm::vec2 pixelSize {
                float(terrain.getPatchSize() * terrain.getSize().x) / (float) terrainDisplacementTex->getWidth(),
                float(terrain.getPatchSize() * terrain.getSize().y) / (float) terrainDisplacementTex->getHeight()
            };

            const glm::ivec2 indices { std::floor(decalRelativePos.x / pixelSize.x), std::floor(decalRelativePos.z / pixelSize.y) };

            std::cout << "indices: " << indices.x << ", " << indices.y << std::endl;

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
                                // const size_t finalIndex = indices.x + (indices.y) * terrainDisplacementTex->getWidth();
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
            terrainDisplacementTex->subTextureData(reinterpret_cast<const std::uint8_t*>(terrainDisplacementData.data()), terrainDisplacementTex->getWidth(), terrainDisplacementTex->getHeight(), 0, 0);
        }

        if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_F1))
        {
            currentTerrainOp = TerrainOp::TERRAIN_GROW;
            terrainDecalMaterial->replaceTexture(SGTextureSlot::SGTT_DIFFUSE, 0, terrainDecalDiffuseTex0);
        }
        else if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_F2))
        {
            currentTerrainOp = TerrainOp::TERRAIN_LOWER;
            terrainDecalMaterial->replaceTexture(SGTextureSlot::SGTT_DIFFUSE, 0, terrainDecalDiffuseTex1);
        }

        if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_PAGE_UP))
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

        if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_PAGE_DOWN))
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

        if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_EQUAL))
        {
            decalTransform->m_ownTransform.m_scale += 0.1f;
        }
        else if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_MINUS))
        {
            decalTransform->m_ownTransform.m_scale -= 0.1f;
        }
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_2))
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
    }

    if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_4))
    {
        auto& cameraTransform = scene->getECSRegistry()->get<SGCore::Transform>(mainCamera);
        createBallAndApplyImpulse(cameraTransform->m_ownTransform.m_position, cameraTransform->m_ownTransform.m_forward * 200000.0f / 10.0f);
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_5))
    {
        regenerateTerrainPhysicalMesh(terrainEntity);
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_6))
    {
        if(physicsDebugDraw->getDebugMode() != btIDebugDraw::DBG_NoDebug)
        {
            physicsDebugDraw->setDebugMode(btIDebugDraw::DBG_NoDebug);
        }
        else
        {
            physicsDebugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        }
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_7))
    {
        saveTerrainDisplacementMap();
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_F3))
    {
        // terrainNavGrid.applyModelMatrix(terrainTransform->m_finalTransform.m_animatedModelMatrix);
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_F4))
    {
        if(debugDraw->m_mode != SGCore::DebugDrawMode::NO_DEBUG)
        {
            debugDraw->m_mode = SGCore::DebugDrawMode::NO_DEBUG;
        }
        else
        {
            debugDraw->m_mode = SGCore::DebugDrawMode::WIREFRAME;
        }
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_F5))
    {
        regenerateTerrainNavGrid(terrainEntity);
    }

    if(debugDraw->m_mode != SGCore::DebugDrawMode::NO_DEBUG)
    {
        /*for(size_t i = 0; i < terrainDisplacedIndices.size(); i += 4)
        {
            const auto v0 = terrainDisplacedVertices[terrainDisplacedIndices[i]].m_position;
            const auto v1 = terrainDisplacedVertices[terrainDisplacedIndices[i + 1]].m_position;
            const auto v2 = terrainDisplacedVertices[terrainDisplacedIndices[i + 2]].m_position;
            const auto v3 = terrainDisplacedVertices[terrainDisplacedIndices[i + 3]].m_position;

            debugDraw->drawLine(v0, v1, { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(v1, v2,  { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(v2, v3,  { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(v3, v0,  { 0.47, 0.87, 0.78, 1.0 });
        }*/
        /*for(const auto& node : terrainNavGrid.m_nodes)
        {
            const auto leftBottom = node.m_position - glm::vec3 { node.m_size / 2.0f, 0, node.m_size / 2.0f };
            const auto leftTop = node.m_position + glm::vec3 { -node.m_size / 2.0f, 0.0f, node.m_size / 2.0f };
            const auto rightTop = node.m_position + glm::vec3 { node.m_size / 2.0f, 0, node.m_size / 2.0f };
            const auto rightBottom = node.m_position + glm::vec3 { node.m_size / 2.0f, 0.0f, -node.m_size / 2.0f };

            debugDraw->drawLine(leftBottom, leftTop, { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(leftTop, rightTop,  { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(rightTop, rightBottom,  { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(rightBottom, leftBottom,  { 0.47, 0.87, 0.78, 1.0 });

            debugDraw->drawLine(node.m_position, node.m_position + glm::vec3 { 0.0f, 5.0f, 0.0f },  { 0.91, 0.40, 0.42, 1.0 });
        }*/

        const auto inputFilteringStep = terrainNavMesh.getStep<SGCore::Navigation::InputFilteringStep>();
        const auto voxelizationStep = terrainNavMesh.getStep<SGCore::Navigation::VoxelizationStep>();

        const auto& navMeshConfig = terrainNavMesh.m_config;

        /*for(const auto& tri : inputFilteringStep->m_walkableTriangles)
        {
            debugDraw->drawLine(tri.m_vertices[0], tri.m_vertices[1], { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(tri.m_vertices[1], tri.m_vertices[2], { 0.47, 0.87, 0.78, 1.0 });
            debugDraw->drawLine(tri.m_vertices[2], tri.m_vertices[0], { 0.47, 0.87, 0.78, 1.0 });
        }*/

        for(const auto& voxel : voxelizationStep->m_voxels)
        {
            /*const glm::vec3 min = voxelizationStep->voxelToWorld(
                                                                      voxel.m_position, navMeshConfig.m_cellSize,
                                                                      navMeshConfig.m_cellHeight) - glm::vec3(
                                                                      navMeshConfig.m_cellSize * 0.5f,
                                                                      navMeshConfig.m_cellHeight * 0.5f,
                                                                      navMeshConfig.m_cellSize * 0.5f);*/

            const glm::vec3 min = voxelizationStep->voxelToWorld(
                                      voxel.m_position, navMeshConfig.m_cellSize,
                                      navMeshConfig.m_cellHeight) - glm::vec3(
                                      navMeshConfig.m_cellSize * 0.5f,
                                      navMeshConfig.m_cellHeight * 0.5f,
                                      navMeshConfig.m_cellSize * 0.5f);

            const glm::vec3 max = min + glm::vec3(navMeshConfig.m_cellSize,
                                                  navMeshConfig.m_cellHeight,
                                                  navMeshConfig.m_cellSize);


            // debugDraw->drawAABB(min, max, { 0.47, 0.87, 0.78, 1.0 });
            if(voxel.m_isWalkable)
            {
                debugDraw->drawAABB(min, max, { 0.47, 0.87, 0.78, 1.0 });
            }
            else
            {
                debugDraw->drawAABB(min, max, { 1.0, 0.0, 0.0, 1.0 });
            }
        }

        /*for(std::int32_t z = 0; z < voxelizationStep->m_voxelGridDepth; ++z)
        {
            for(std::int32_t x = 0; x < voxelizationStep->m_voxelGridWidth; ++x)
            {
                for(std::int32_t y = 0; y < voxelizationStep->m_voxelGridHeight; ++y)
                {
                    auto& voxel = voxelizationStep->getVoxel(x, y, z);

                    const glm::vec3 min = voxelizationStep->voxelToWorld(
                                                                      { x, y, z }, navMeshConfig.m_cellSize,
                                                                      navMeshConfig.m_cellHeight) - glm::vec3(
                                                                      navMeshConfig.m_cellSize * 0.5f,
                                                                      navMeshConfig.m_cellHeight * 0.5f,
                                                                      navMeshConfig.m_cellSize * 0.5f);

                    const glm::vec3 max = min + glm::vec3(navMeshConfig.m_cellSize,
                                                          navMeshConfig.m_cellHeight,
                                                          navMeshConfig.m_cellSize);

                    if(voxel.m_isWalkable)
                    {
                        debugDraw->drawAABB(min, max, { 0.47, 0.87, 0.78, 1.0 });
                    }
                    else
                    {
                        debugDraw->drawAABB(min, max, { 1.0, 0.0, 0.0, 1.0 });
                    }
                }
            }
        }*/
    }

    // debugDraw->drawLine({ 0, 0, 0 }, { 0, 10, 0 },  { 0.91, 0.40, 0.42, 1.0 });
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

    SGCore::CoreMain::init();
    SGCore::CoreMain::startCycle();

    return 0;
}
