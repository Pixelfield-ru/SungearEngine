//
// Created by stuka on 02.10.2024.
//

#include "Main.h"

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Scene/EntityBaseInfo.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Transformations/Controllable3D.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Graphics/API/ICubemapTexture.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Render/Atmosphere/Atmosphere.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <SGCore/Physics/Rigidbody3D.h>
#include <SGCore/Physics/PhysicsWorld3D.h>
#include <SGCore/Input/InputManager.h>

#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Serde/Components/NonSavable.h"


#include <BulletCollision/CollisionShapes/btBvhTriangleMeshShape.h>
#include <BulletCollision/CollisionShapes/btStaticPlaneShape.h>

#include "SGCore/Coro/CoroScheduler.h"

using namespace SGCore;

// VARIABLES =================================================

SGCore::AssetRef<SGCore::IShader> screenShader;
AssetRef<ModelAsset> cubeModel;
AssetRef<ModelAsset> sphereModel;
AssetRef<ICubemapTexture> cubemapTexture;

Ref<Scene> testScene;

ECS::entity_t testCameraEntity;
ECS::entity_t testPlayerEntity;

ECS::entity_t floorEntity;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;
SGCore::Ref<SGCore::IMeshData> quadMeshData;
SGCore::MeshRenderState quadMeshRenderState;

// ===========================================================

void createBallAndApplyImpulse(const glm::vec3& spherePos,
    const glm::vec3& impulse) noexcept
{
    auto sphereModel = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("sphere_model");

    std::vector<SGCore::ECS::entity_t> sphereEntities;
    sphereModel->m_rootNode->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
        [&sphereEntities](const SGCore::ECS::entity_t& entity)
        {
            sphereEntities.push_back(entity);
        }
    );

    auto sphereRigidbody3D = testScene->getECSRegistry()->emplace<SGCore::Rigidbody3D>(sphereEntities[2],
        SGCore::MakeRef<SGCore::Rigidbody3D>(
            testScene->getSystem<SGCore::PhysicsWorld3D>()));

    SGCore::Ref<btSphereShape> sphereRigidbody3DShape = SGCore::MakeRef<btSphereShape>(1.0);
    btTransform shapeTransform;
    shapeTransform.setIdentity();
    sphereRigidbody3D->addShape(shapeTransform, sphereRigidbody3DShape);
    sphereRigidbody3D->setType(SGCore::PhysicalObjectType::OT_DYNAMIC);
    sphereRigidbody3D->m_body->setRestitution(0.9);
    btScalar mass = 100.0f;
    btVector3 inertia(0, 0, 0);
    sphereRigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
    sphereRigidbody3D->m_body->setMassProps(mass, inertia);
    sphereRigidbody3D->reAddToWorld();

    glm::vec3 finalImpulse = impulse;
    sphereRigidbody3D->m_body->applyCentralImpulse({ finalImpulse.x, finalImpulse.y, finalImpulse.z });

    SGCore::Ref<SGCore::Transform>& sphereTransform = testScene->getECSRegistry()->get<SGCore::Transform>(sphereEntities[0]);
    sphereTransform->m_ownTransform.m_position = spherePos;
}

void coreInit()
{
    auto mainAssetManager = SGCore::AssetManager::getInstance();

    // creating PBR render pipeline and setting as current
    auto pbrrpPipeline = RenderPipelinesManager::createRenderPipeline<PBRRenderPipeline>();
    RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    RenderPipelinesManager::setCurrentRenderPipeline<PBRRenderPipeline>();

    // creating scene and setting as current
    testScene = SGCore::MakeRef<SGCore::Scene>();
    testScene->m_metaInfo.m_sceneName = "TestScene";
    testScene->createDefaultSystems();

    Scene::setCurrentScene(testScene);

    auto ecsRegistry = testScene->getECSRegistry();

    // creating camera entity
    testCameraEntity = testScene->getECSRegistry()->create();


    ecsRegistry->emplace<SGCore::Transform>(testCameraEntity, SGCore::MakeRef<SGCore::Transform>());
    ecsRegistry->emplace<SGCore::NonSavable>(testCameraEntity);
    ecsRegistry->emplace<SGCore::Camera3D>(testCameraEntity, SGCore::MakeRef<SGCore::Camera3D>());
    ecsRegistry->emplace<SGCore::RenderingBase>(testCameraEntity, SGCore::MakeRef<SGCore::RenderingBase>());
    ecsRegistry->emplace<SGCore::Controllable3D>(testCameraEntity);
    auto& cameraReceiver = ecsRegistry->emplace<SGCore::LayeredFrameReceiver>(testCameraEntity);

    attachmentToDisplay = cameraReceiver.m_layersFXFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7);

    // loading assets =============================================

    cubeModel = AssetManager::getInstance()->loadAssetWithAlias<ModelAsset>(
        "cube_model",
        CoreMain::getSungearEngineRootPath() / "Resources/models/standard/cube.obj"
    );

    sphereModel = AssetManager::getInstance()->loadAssetWithAlias<ModelAsset>(
        "sphere_model",
        CoreMain::getSungearEngineRootPath() / "Resources/models/standard/sphere.obj"
    );

    screenShader = mainAssetManager->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");

    // =====

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

    // adding skybox with atmosphere
    {
        auto standardCubemapMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("standard_skybox_material0");
        standardCubemapMaterial->m_shaders["GeometryPass"] =
                mainAssetManager->loadAsset<SGCore::IShader>(
                        *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
        standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;
        standardCubemapMaterial->addTexture2D(SGTextureSlot::SGTT_SKYBOX, standardCubemap);SGCore::ECS::entity_t atmosphereEntity;

        std::vector<SGCore::ECS::entity_t> skyboxEntities;
        auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("cube_model");
        cubeModel->m_rootNode->addOnScene(testScene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities](const auto& entity) {
            skyboxEntities.push_back(entity);
            testScene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
            testScene->getECSRegistry()->remove<SGCore::Pickable>(entity);
            testScene->getECSRegistry()->remove<SGCore::TransparentEntityTag>(entity);
        });

        atmosphereEntity = skyboxEntities[2];

        auto& skyboxMesh = testScene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
        auto& atmosphereScattering = testScene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
        atmosphereScattering.m_sunRotation.z = 90.0;
        skyboxMesh.m_base.setMaterial(SGCore::AssetManager::getInstance()->getAsset<SGCore::IMaterial, SGCore::AssetStorageType::BY_ALIAS>("standard_skybox_material0"));

        auto& skyboxTransform = testScene->getECSRegistry()->get<SGCore::Transform>(atmosphereEntity);

        skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };


    }

    // creating player ===================
    std::vector<ECS::entity_t> playerEntities;
    cubeModel->m_rootNode->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                      [&playerEntities](const ECS::entity_t& entity)
                                      {
                                          playerEntities.push_back(entity);
                                          //testScene->getECSRegistry()->emplace<EntityBaseInfo>(meshEntity);
                                      }
    );

    testPlayerEntity = playerEntities[2];

    // cameraBaseInfo.m_parent = testPlayerEntity;

    auto playerTransform = testScene->getECSRegistry()->get<Transform>(playerEntities[0]);

    playerTransform->m_ownTransform.m_position = { 300, 10.0f, 0.0f };
    playerTransform->m_ownTransform.m_scale = { 1.0f, 1.8f, 1.0f };

    auto playerRigidbody3D = testScene->getECSRegistry()->emplace<Rigidbody3D>(testPlayerEntity,
                                                                                        MakeRef<Rigidbody3D>(testScene->getSystem<PhysicsWorld3D>()));

    // creating rigidbody and box shape for player
    /*auto playerRigidbody3D = testScene->getECSRegistry()->emplace<Rigidbody3D>(playerEntities[0],
                                                                                    MakeRef<Rigidbody3D>(testScene->getSystem<PhysicsWorld3D>()));
    SGCore::Ref<btBoxShape> playerRigidbody3DShape = SGCore::MakeRef<btBoxShape>(btVector3(1.0, 1.8, 1.0));
    btTransform playerShapeTransform;
    playerShapeTransform.setIdentity();
    playerRigidbody3D->addShape(playerShapeTransform, playerRigidbody3DShape);
    playerRigidbody3D->setType(SGCore::PhysicalObjectType::OT_DYNAMIC);
    playerRigidbody3D->m_body->setRestitution(0.1);
    btScalar mass = 70.0f;
    btVector3 inertia(0, 0, 0);
    playerRigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
    playerRigidbody3D->m_body->setMassProps(mass, inertia);*/
    // playerRigidbody3D->reAddToWorld();

    // ===================================

    // adding entities on scene ===================================

    std::vector<ECS::entity_t> floorEntities;
    cubeModel->m_rootNode->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
        [&floorEntities](const ECS::entity_t& entity)
        {
            floorEntities.push_back(entity);
            //testScene->getECSRegistry()->emplace<EntityBaseInfo>(meshEntity);
        }
    );

    floorEntity = floorEntities[0];

    auto floorTransform = testScene->getECSRegistry()->get<Transform>(floorEntities[0]);

    floorTransform->m_ownTransform.m_scale = { 250.0f, 1.0f, 250.0f };
    floorTransform->m_ownTransform.m_position = { 0, -50, 0 };

    // creating rigidbody and box shape for floor
    auto floorRigidbody3D = testScene->getECSRegistry()->emplace<Rigidbody3D>(floorEntities[0],
        MakeRef<Rigidbody3D>(testScene->getSystem<PhysicsWorld3D>()));

    // floorRigidbody3D->removeFromWorld();

    // floorRigidbody3D->reAddToWorld();

    // creating quad model for drawing camera framebuffer attachment to screen ======================================

    quadMeshData = SGCore::Ref<SGCore::IMeshData>(SGCore::CoreMain::getRenderer()->createMeshData());

    quadMeshData->m_vertices.resize(4);

    quadMeshData->m_vertices[0] = {
        .m_position = { -1, -1, 0.0f }
    };

    quadMeshData->m_vertices[1] = {
        .m_position = { -1, 1, 0.0f }
    };

    quadMeshData->m_vertices[2] = {
        .m_position = { 1, 1, 0.0f }
    };

    quadMeshData->m_vertices[3] = {
        .m_position = { 1, -1, 0.0f }
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

SGCore::Coro::Task<> addMass(SGCore::Rigidbody3D::reg_t rigidbody3D)
{
    using namespace std::chrono_literals;

    co_await 1s;

    btScalar mass = 70.0f;
    btVector3 inertia(0, 0, 0);
    rigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
    rigidbody3D->m_body->setMassProps(mass, inertia);
}

void onUpdate(const double& dt, const double& fixedDt)
{
    if(InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_5))
    {
        auto playerRigidbody3D = testScene->getECSRegistry()->get<Rigidbody3D>(testPlayerEntity);

        SGCore::Ref<btBoxShape> playerRigidbody3DShape = SGCore::MakeRef<btBoxShape>(btVector3(0.5, 0.5, 0.5));
        btTransform playerShapeTransform;
        playerShapeTransform.setIdentity();
        playerRigidbody3D->addShape(playerShapeTransform, playerRigidbody3DShape);

        // playerRigidbody3D->m_body->setRestitution(0.1);

        // addMass(playerRigidbody3D).run();
        static auto c = [playerRigidbody3D]() -> SGCore::Coro::Task<> {
            using namespace std::chrono_literals;

            co_await 4s;

            playerRigidbody3D->setType(SGCore::PhysicalObjectType::OT_DYNAMIC);

            btScalar mass = 1.0f;
            btVector3 inertia(0, 0, 0);
            playerRigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
            playerRigidbody3D->m_body->setMassProps(mass, inertia);

            playerRigidbody3D->m_body->activate();
        };

        c().run();

        auto floorRigidbody3D = testScene->getECSRegistry()->get<Rigidbody3D>(floorEntity);

        SGCore::Ref<btBoxShape> floorRigidbody3DShape = SGCore::MakeRef<btBoxShape>(btVector3(1.0, 1.0, 1.0));
        btTransform floorShapeTransform;
        floorShapeTransform.setIdentity();
        // floorShapeTransform.getOrigin().m_floats[0] = 100;
        floorRigidbody3D->addShape(floorShapeTransform, floorRigidbody3DShape);

        floorRigidbody3D->getShapeTransform(0).setOrigin(btVector3(100, 0, 0));
        floorRigidbody3DShape->setLocalScaling({ 250, 1, 250 });

        floorRigidbody3D->updateShapeTransform(0);

        // floorRigidbody3D->m_body->activate();
        floorRigidbody3D->stop();
        /*btScalar mass = 70.0f;
        btVector3 inertia(0, 0, 0);
        playerRigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
        playerRigidbody3D->m_body->setMassProps(mass, inertia);*/
    }

    if (Scene::getCurrentScene())
    {
        Scene::getCurrentScene()->update(dt, fixedDt);
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

    if (InputManager::getMainInputListener()->keyboardKeyReleased(KeyboardKey::KEY_M))
    {
        auto& physicsWorld3DDebug = testScene->getSystem<PhysicsWorld3D>()->getDebugDraw();
        if (physicsWorld3DDebug->getDebugMode() == btIDebugDraw::DBG_NoDebug)
        {
            physicsWorld3DDebug->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
        }
        else
        {
            physicsWorld3DDebug->setDebugMode(btIDebugDraw::DBG_NoDebug);
        }
    }

    if (SGCore::InputManager::getMainInputListener()->keyboardKeyDown(SGCore::KeyboardKey::KEY_4))
    {
        auto& cameraTransform = testScene->getECSRegistry()->get<SGCore::Transform>(testCameraEntity);
        createBallAndApplyImpulse(cameraTransform->m_ownTransform.m_position, cameraTransform->m_ownTransform.m_forward * 200000.0f / 10.0f);
    }
}

void onFixedUpdate(const double& dt, const double& fixedDt)
{
    if (Scene::getCurrentScene())
    {
        Scene::getCurrentScene()->fixedUpdate(dt, fixedDt);
    }

    /*if(InputManager::getMainInputListener()->keyboardKeyDown(KeyboardKey::KEY_S))
    {
        playerTransform->m_ownTransform.m_position.z += -playerSpeed * dt;
    }

    if(InputManager::getMainInputListener()->keyboardKeyDown(KeyboardKey::KEY_D))
    {
        playerTransform->m_ownTransform.m_position.x += -playerSpeed * dt;
    }

    if(InputManager::getMainInputListener()->keyboardKeyDown(KeyboardKey::KEY_A))
    {
        playerTransform->m_ownTransform.m_position.x += playerSpeed * dt;
    }*/
}

int main()
{
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    CoreMain::init();
    CoreMain::startCycle();

    return 0;
}