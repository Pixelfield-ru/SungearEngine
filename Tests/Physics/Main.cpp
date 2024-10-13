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
#include <SGCore/Render/ShaderComponent.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Render/Atmosphere/Atmosphere.h>
#include <BulletCollision/CollisionShapes/btBoxShape.h>
#include <SGCore/Physics/Rigidbody3D.h>
#include <SGCore/Physics/PhysicsWorld3D.h>
#include <SGCore/Input/InputManager.h>

using namespace SGCore;

// VARIABLES =================================================

Ref<ModelAsset> cubeModel;
Ref<ModelAsset> sphereModel;
Ref<ICubemapTexture> cubemapTexture;

Ref<Scene> testScene;

entity_t testCameraEntity;

entity_t planeEntity;

// ===========================================================

void createBallAndApplyImpulse(const glm::vec3& spherePos,
    const glm::vec3& impulse) noexcept
{
    auto sphereModel = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("sphere_model");

    std::vector<SGCore::entity_t> sphereEntities;
    sphereModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
        [&sphereEntities](const SGCore::entity_t& entity)
        {
            sphereEntities.push_back(entity);
        }
    );

    auto sphereRigidbody3D = testScene->getECSRegistry()->emplace<SGCore::Ref<SGCore::Rigidbody3D>>(sphereEntities[2],
        SGCore::MakeRef<SGCore::Rigidbody3D>(
            testScene->getSystem<SGCore::PhysicsWorld3D>()));
    SGCore::Ref<btSphereShape> sphereRigidbody3DShape = SGCore::MakeRef<btSphereShape>(1.0);
    sphereRigidbody3D->setShape(sphereRigidbody3DShape);
    sphereRigidbody3D->m_bodyFlags.removeFlag(btCollisionObject::CF_STATIC_OBJECT);
    sphereRigidbody3D->m_bodyFlags.addFlag(btCollisionObject::CF_DYNAMIC_OBJECT);
    sphereRigidbody3D->m_body->setRestitution(0.9);
    btScalar mass = 100.0f;
    btVector3 inertia(0, 0, 0);
    sphereRigidbody3D->m_body->getCollisionShape()->calculateLocalInertia(mass, inertia);
    sphereRigidbody3D->m_body->setMassProps(mass, inertia);
    sphereRigidbody3D->updateFlags();
    sphereRigidbody3D->reAddToWorld();

    glm::vec3 finalImpulse = impulse;
    sphereRigidbody3D->m_body->applyCentralImpulse({ finalImpulse.x, finalImpulse.y, finalImpulse.z });

    SGCore::Ref<SGCore::Transform>& sphereTransform = testScene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(sphereEntities[0]);
    sphereTransform->m_ownTransform.m_position = spherePos;
}

void coreInit()
{
    // creating PBR render pipeline and setting as current
    auto pbrrpPipeline = RenderPipelinesManager::createRenderPipeline<PBRRenderPipeline>();
    RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    RenderPipelinesManager::setCurrentRenderPipeline<PBRRenderPipeline>();

    // creating scene and setting as current
    testScene = SGCore::MakeRef<SGCore::Scene>();
    testScene->m_name = "TestScene";
    testScene->createDefaultSystems();
    Scene::addScene(testScene);
    Scene::setCurrentScene("TestScene");

    // creating camera entity
    testCameraEntity = testScene->getECSRegistry()->create();
    // adding base info to entity
    EntityBaseInfo& cameraBaseInfo = testScene->getECSRegistry()->emplace<EntityBaseInfo>(testCameraEntity);
    cameraBaseInfo.setRawName("SGMainCamera");

    auto& cameraTransform = testScene->getECSRegistry()->emplace<Ref<Transform>>(testCameraEntity, MakeRef<Transform>());

    auto& cameraEntityCamera3D = testScene->getECSRegistry()->emplace<Ref<Camera3D>>(testCameraEntity, MakeRef<Camera3D>());
    auto& cameraEntityControllable = testScene->getECSRegistry()->emplace<Controllable3D>(testCameraEntity);
    auto& cameraRenderingBase = testScene->getECSRegistry()->emplace<Ref<RenderingBase>>(testCameraEntity, MakeRef<RenderingBase>());

    // loading assets =============================================

    cubeModel = AssetManager::getInstance()->loadAssetWithAlias<ModelAsset>(
        "cube_model",
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/models/standard/cube.obj"
    );

    sphereModel = AssetManager::getInstance()->loadAssetWithAlias<ModelAsset>(
        "sphere_model",
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/models/standard/sphere.obj"
    );

    // =====

    cubemapTexture = Ref<ICubemapTexture>(CoreMain::getRenderer()->createCubemapTexture());

    cubemapTexture->m_parts.push_back(AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
    ));
    cubemapTexture->m_parts.push_back(AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
    ));

    cubemapTexture->m_parts.push_back(AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
    ));
    cubemapTexture->m_parts.push_back(AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
    ));

    cubemapTexture->m_parts.push_back(AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
    ));
    cubemapTexture->m_parts.push_back(AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(
        CoreMain::getSungearEngineRootPath() / "Resources/SGResources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
    ));

    cubemapTexture->setRawName("standard_skybox0");
    cubemapTexture->create();
    AssetManager::getInstance()->addAsset("standard_skybox0", cubemapTexture);

    auto standardCubemapMaterial = SGCore::MakeRef<SGCore::IMaterial>();
    standardCubemapMaterial->addTexture2D(SGTextureType::SGTT_SKYBOX, cubemapTexture);
    AssetManager::getInstance()->addAsset("standard_skybox_material0", standardCubemapMaterial);

    // adding skybox with atmosphere
    {
        std::vector<entity_t> skyboxEntities;
        cubeModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities](const auto& entity) {
            skyboxEntities.push_back(entity);
            });

        const entity_t& skyboxMeshEntity = skyboxEntities[2];

        Mesh& skyboxMesh = testScene->getECSRegistry()->get<Mesh>(skyboxMeshEntity);
        ShaderComponent& skyboxShaderComponent = testScene->getECSRegistry()->emplace<ShaderComponent>(skyboxMeshEntity);
        Atmosphere& atmosphereScattering = testScene->getECSRegistry()->emplace<Atmosphere>(skyboxMeshEntity);

        // setting material
        skyboxMesh.m_base.setMaterial(AssetManager::getInstance()->loadAsset<IMaterial>("standard_skybox_material0"));

        ShadersUtils::loadShader(skyboxShaderComponent, "SkyboxShader");
        skyboxMesh.m_base.m_meshDataRenderInfo.m_enableFacesCulling = false;

        auto& skyboxTransform = testScene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(skyboxMeshEntity);

        skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };
    }

    // adding entities on scene ===================================

    std::vector<entity_t> floorEntities;
    cubeModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
        [&floorEntities](const entity_t& entity)
        {
            floorEntities.push_back(entity);
            //testScene->getECSRegistry()->emplace<EntityBaseInfo>(meshEntity);
        }
    );

    auto transform = testScene->getECSRegistry()->get<Ref<Transform>>(floorEntities[0]);

    transform->m_ownTransform.m_scale = { 1000.0f, 1.0f, 1000.0f };
    transform->m_ownTransform.m_position = { 0, -50, 0 };

    // creating rigidbody and box shape for floor
    auto floorRigidbody3D = testScene->getECSRegistry()->emplace<Ref<Rigidbody3D>>(floorEntities[0],
        MakeRef<Rigidbody3D>(testScene->getSystem<PhysicsWorld3D>()));
    SGCore::Ref<btBoxShape> floorRigidbody3DShape = SGCore::MakeRef<btBoxShape>(btVector3(250, 1, 250.0));
    floorRigidbody3D->setShape(floorRigidbody3DShape);
    floorRigidbody3D->m_body->setMassProps(100000000.0, btVector3(0, 0, 0));
    floorRigidbody3D->m_body->setRestitution(0.9);
    floorRigidbody3D->reAddToWorld();
}

void onUpdate(const double& dt, const double& fixedDt)
{
    if (Scene::getCurrentScene())
    {
        Scene::getCurrentScene()->update(dt, fixedDt);
    }

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
        auto& cameraTransform = testScene->getECSRegistry()->get<SGCore::Ref<SGCore::Transform>>(testCameraEntity);
        createBallAndApplyImpulse(cameraTransform->m_ownTransform.m_position, cameraTransform->m_ownTransform.m_forward * 200000.0f / 10.0f);
    }
}

void onFixedUpdate(const double& dt, const double& fixedDt)
{
    if (Scene::getCurrentScene())
    {
        Scene::getCurrentScene()->fixedUpdate(dt, fixedDt);
    }
}

int main()
{
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    CoreMain::start();

    return 0;
}