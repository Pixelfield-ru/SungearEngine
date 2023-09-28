#include <iostream>
#include <memory>
//#include <new>

#include "SGCore/ECS/Entity.h"
#include "SGCore/ECS/Scene.h"

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/SkyboxComponent.h"
#include "SGCore/Memory/Assets/CubemapAsset.h"
#include "SGCore/ECS/ECSWorld.h"

#include <BulletCollision/CollisionDispatch/btDefaultCollisionConfiguration.h>
#include <BulletCollision/CollisionDispatch/btCollisionDispatcher.h>
#include <BulletCollision/BroadphaseCollision/btBroadphaseInterface.h>
#include <BulletDynamics/ConstraintSolver/btSequentialImpulseConstraintSolver.h>
#include <BulletDynamics/Dynamics/btDiscreteDynamicsWorld.h>
#include <BulletCollision/BroadphaseCollision/btDbvtBroadphase.h>
#include "BulletCollision/CollisionShapes/btBoxShape.h"
#include "SGCore/ECS/Rendering/Primitives/LineComponent.h"
#include "SGCore/ECS/Rendering/Primitives/BoxComponent.h"
#include "SGCore/ECS/Rendering/Primitives/SphereComponent.h"

std::shared_ptr<Core::ECS::Entity> testCameraEntity;
std::shared_ptr<Core::ECS::Scene> testScene;

// physics ---------------------
btDefaultCollisionConfiguration*        collisionConfiguration;
btCollisionDispatcher*                  dispatcher;
btBroadphaseInterface*                  overlappingPairCache;
btSequentialImpulseConstraintSolver*    solver;
btDiscreteDynamicsWorld* 				dynamicsWorld;
btIDebugDraw*							debugDraw;

btCollisionObject* testCollisionObject;
btBoxShape* testBoxShape;

void physicsInit()
{
    collisionConfiguration = new btDefaultCollisionConfiguration;
    dispatcher = new btCollisionDispatcher(collisionConfiguration);

    testBoxShape = new btBoxShape({ 0.5, 0.5, 0.5 });
    testCollisionObject = new btCollisionObject;

    overlappingPairCache = new btDbvtBroadphase();

    solver = new btSequentialImpulseConstraintSolver;

    //debugDraw = new btIDebugDraw;

    dynamicsWorld = new btDiscreteDynamicsWorld(dispatcher, overlappingPairCache, solver, collisionConfiguration);

    //debugDraw->setDebugMode(btIDebugDraw::DBG_DrawWireframe);
    dynamicsWorld->setDebugDrawer(debugDraw);

    dynamicsWorld->setGravity(btVector3(0, -10, 0));
}

// example how to convert imported scene to Sungear ECS scene
void processLoadedNode(const std::shared_ptr<Core::ImportedScene::Node>& sgNode,
                       const glm::vec3& pos,
                       const glm::vec3& rot,
                       const glm::vec3& scale,
                       std::vector<std::shared_ptr<Core::ECS::Entity>>& outputEntities)
{
    std::shared_ptr<Core::ECS::Entity> nodeEntity = std::make_shared<Core::ECS::Entity>();
    nodeEntity->addComponent(std::make_shared<Core::ECS::TransformComponent>());
    nodeEntity->m_name = sgNode->m_name;

    outputEntities.push_back(nodeEntity);

    for(auto& mesh : sgNode->m_meshes)
    {
        std::shared_ptr<Core::ECS::TransformComponent> meshedEntityTransformComponent = std::make_shared<Core::ECS::TransformComponent>();
        meshedEntityTransformComponent->m_position = pos;
        meshedEntityTransformComponent->m_rotation = rot;
        meshedEntityTransformComponent->m_scale = scale;

        std::shared_ptr<Core::ECS::MeshComponent> meshComponent = std::make_shared<Core::ECS::MeshComponent>();
        meshComponent->m_mesh = mesh;

        std::shared_ptr<Core::ECS::Entity> meshedEntity = std::make_shared<Core::ECS::Entity>();
        meshedEntity->addComponent(meshedEntityTransformComponent);
        meshedEntity->addComponent(meshComponent);

        outputEntities.push_back(meshedEntity);
    }

    for(auto& node : sgNode->m_children)
    {
        processLoadedNode(node, pos, rot, scale, outputEntities);
    }
}

std::shared_ptr<Core::ECS::Entity> testShadowsCaster;

std::shared_ptr<Core::ECS::SphereComponent> sphereComponent;
std::shared_ptr<Core::ECS::BoxComponent> boxComponent;

void init()
{
    testScene = std::make_shared<Core::ECS::Scene>();
    Core::ECS::Scene::setCurrentScene(testScene);

    // objects ---------------------------------

    auto plane = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            "../SGResources/models/test/plane.obj"
    );

    auto cubeModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            "../SGResources/models/standard/cube.fbx"
    );

    auto skyboxModel = Core::Memory::AssetManager::loadAssetWithAlias<Core::Memory::Assets::ModelAsset>(
            "skyboxModel",
            "../SGResources/models/standard/cube.fbx"
    );

    std::vector<std::shared_ptr<Core::ECS::Entity>> planeEntities;

    for(auto& node : plane->m_nodes)
    {
        processLoadedNode(node, { 0, -3, -15 }, { }, { 1, 1, 2 }, planeEntities);
    }

    for(const auto& entity : planeEntities)
    {
        auto meshComponent = entity->getComponent<Core::ECS::MeshComponent>();
        auto transformComponent = entity->getComponent<Core::ECS::TransformComponent>();
        if(meshComponent) meshComponent->m_enableFacesCulling = false;
        if(transformComponent)
        {
            transformComponent->m_scale = { 400.0, 400.0, 400.0 };
        }
        testScene->m_entities.push_back(entity);
    }

    std::vector<std::shared_ptr<Core::ECS::Entity>> cubeEntities;

    for(int i = 0; i < 11; i++)
    {
        for (auto& node: cubeModel->m_nodes)
        {
            processLoadedNode(node, {0, 0, 7}, {0, 0, 0},
                              {0.1, 0.1, 0.1}, cubeEntities);
        }
    }

    std::vector<std::shared_ptr<Core::ECS::Entity>> skyboxEntities;

    for(auto& node : skyboxModel->m_nodes)
    {
        processLoadedNode(node, { 0, 0, 0 }, { 0, 0, 0 },
                          { 39, 39, 39 }, skyboxEntities);
    }

    float curY = 0.0;

    for (const auto& entity : cubeEntities)
    {
        auto transformComponent = entity->getComponent<Core::ECS::TransformComponent>();

        if (transformComponent)
        {
            //transformComponent->m_position.y = curY;
            transformComponent->m_position.z = -curY;
        }

        testScene->m_entities.push_back(entity);

        curY += 0.35;
    }

    for(const auto& entity : skyboxEntities)
    {
        entity->addComponent(std::make_shared<Core::ECS::SkyboxComponent>());

        auto meshComponent = entity->getComponent<Core::ECS::MeshComponent>();
        if(meshComponent)
        {
            meshComponent->m_enableFacesCulling = false;
            meshComponent->m_mesh->m_material->addTexture2D(
                    SGMaterialTextureType::SGTP_SKYBOX,
                    Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::CubemapAsset>(
                            "standard_skybox0",
                            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(
                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
                            ),
                            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(
                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
                            ),

                            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(
                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
                            ),
                            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(
                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
                            ),

                            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(
                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
                            ),
                            Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::Texture2DAsset>(
                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
                            )
                    )
            );

            meshComponent->m_mesh->m_material->setShader(
                    SGMAT_STANDARD_SHADER_NAME,
                    std::shared_ptr<Core::Graphics::IShader>(
                    Core::Main::CoreMain::getRenderer().createShader(
                            Core::Graphics::getShaderPath(Core::Graphics::StandardShaderType::SG_SKYBOX_SHADER)
                            )
            ));
        }
        testScene->m_entities.push_back(entity);
    }

    // camera -----------------------------------

    testCameraEntity = std::make_shared<Core::ECS::Entity>();
    auto cameraTransformComponent = std::make_shared<Core::ECS::TransformComponent>();
    /*cameraTransformComponent->m_position.y = -3;
    cameraTransformComponent->m_position.z = 2;
    cameraTransformComponent->m_rotation.x = -30;*/

    cameraTransformComponent->m_position.z = 4.0;
    //cameraTransformComponent->m_position.x = -5;
    testCameraEntity->addComponent(cameraTransformComponent);
    testCameraEntity->addComponent(std::make_shared<Core::ECS::CameraComponent>());

    testScene->m_entities.push_back(testCameraEntity);

    // shadows caster ---------------------------

    testShadowsCaster = std::make_shared<Core::ECS::Entity>();
    testScene->m_entities.push_back(testShadowsCaster);
    auto shadowsCasterTransform = std::make_shared<Core::ECS::TransformComponent>();
    shadowsCasterTransform->m_position.y = 25;
    shadowsCasterTransform->m_position.z = 20.0;
    shadowsCasterTransform->m_position.x = 0.0;
    shadowsCasterTransform->m_rotation.x = 50;
    //shadowsCasterTransform->m_rotation.y = -90;
    sphereComponent = std::make_shared<Core::ECS::SphereComponent>();
    sphereComponent->m_color.y = 1.0f;
    auto sphereComponent1 = std::make_shared<Core::ECS::SphereComponent>();
    boxComponent = std::make_shared<Core::ECS::BoxComponent>();

    boxComponent->m_offset.z = -7.5f;
    boxComponent->m_size.x = boxComponent->m_size.y = 1.0f;
    boxComponent->m_size.z = 5.0f;

    sphereComponent1->m_radius = 3.0f;
    sphereComponent1->m_offset.z = -(5.0f + 5.0f + 3.0);
    sphereComponent1->m_color.b = 1.0f;

    testShadowsCaster->addComponent(shadowsCasterTransform);
    testShadowsCaster->addComponent(std::make_shared<Core::ECS::ShadowsCasterComponent>());
    testShadowsCaster->addComponent(std::make_shared<Core::ECS::DirectionalLightComponent>());

    testShadowsCaster->addComponent(sphereComponent);
    testShadowsCaster->addComponent(sphereComponent1);
    testShadowsCaster->addComponent(sphereComponent);
    testShadowsCaster->addComponent(boxComponent);

    physicsInit();
}

int framesCnt = 0;

void FPSNotRelativeFixedUpdate()
{
    //boxComponent->m_size.z += sin(framesCnt / 75.0) / 10.0;
    //testShadowsCaster->getComponent<Core::ECS::TransformComponent>()->m_rotation.x += sin(framesCnt / 75.0) / 2.0;

    testShadowsCaster->getComponent<Core::ECS::TransformComponent>()->m_rotation.x += 0.3f;

    Core::ECS::ECSWorld::FPSNotRelativeFixedUpdate(Core::ECS::Scene::getCurrentScene());

    framesCnt++;
}

void FPSRelativeFixedUpdate()
{
    Core::ECS::ECSWorld::FPSRelativeFixedUpdate(Core::ECS::Scene::getCurrentScene());
}

// --------------------------------------------
void deltaUpdate(const double& deltaTime)
{

    //Core::ECS::ECSWorld::deltaUpdate(Core::ECS::Scene::getCurrentScene(), deltaTime);
}


int main()
{
    std::cout << "hello physics test!" << std::endl;

    //SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFPSNotRelativeFixedUpdateCallback(FPSNotRelativeFixedUpdate);
    sgSetFPSRelativeFixedUpdateCallback(FPSRelativeFixedUpdate);

    Core::Main::CoreMain::start();

    //SGConsole::Console::stop();

    return EXIT_SUCCESS;

    return 0;
}