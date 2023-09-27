//#define SUNGEAR_DEBUG

#include <cstdlib>

#include "CoreTest.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Graphics/API/IShaderUniform.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Memory/Assets/ModelAsset.h"

#include "SGConsole/API/Console.h"

#include "SGCore/ECS/Transformations/TransformationsSystem.h"

#include <glm/glm.hpp>
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"

#include "SGCore/Main/Callbacks.h"

#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/ECSWorld.h"

#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "SGCore/ECS/Rendering/CameraComponent.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCasterComponent.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLightComponent.h"
#include "SGCore/ECS/Rendering/SkyboxComponent.h"
#include "SGCore/Memory/Assets/CubemapAsset.h"

std::shared_ptr<Core::Memory::Assets::ModelAsset> testModel;

std::shared_ptr<Core::ECS::Entity> testCameraEntity;
std::shared_ptr<Core::ECS::Scene> testScene;

// TODO: ALL THIS CODE WAS WRITTEN JUST FOR THE SAKE OF THE TEST. remove

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

void init()
{
    testScene = std::make_shared<Core::ECS::Scene>();
    Core::ECS::Scene::setCurrentScene(testScene);

    // найс это работает. TODO: убрать! просто ради теста ---------------------
    int windowWidth;
    int windowHeight;

    Core::Main::CoreMain::getWindow().getSize(windowWidth, windowHeight);
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/sponza_new/NewSponza_Main_glTF_002.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/sponza/sponza.obj");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/trees/NewSponza_CypressTree_glTF.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/btr_80a2016/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/stalker/mercenary_exo/Mercenary Exoskeleton.obj");
    /*testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
     "../SGResources/models/test/lenin/scene.gltf"
     );*/
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/ak74m/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/train_ep20/scene.gltf");
    testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            "../SGResources/models/test/plane.obj"
            );

    auto btrModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            //"../SGResources/models/test/gaz-66.obj"
            //"../SGResources/models/test/btr_80a2016/scene.gltf"
            //"../SGResources/models/test/backpack/scene.gltf"
            "../SGResources/models/test/stalk_bunk/bunker.fbx"
            //"../SGResources/models/test/Duty Exoskeleton/Duty Exoskeleton.obj"
            //"../SGResources/models/test/room/room.obj"
            //"../SGResources/models/test/sponza/sponza.obj"
            //"../SGResources/models/test/stalker/mercenary_exo/Mercenary Exoskeleton.obj"
            //"../SGResources/models/test/lenin/scene.gltf"
    );

    auto cubeModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            "../SGResources/models/standard/cube.fbx"
    );

    auto cubeModel1 = Core::Memory::AssetManager::loadAssetWithAlias<Core::Memory::Assets::ModelAsset>(
            "cube1",
            "../SGResources/models/standard/cube.fbx"
    );

    std::vector<std::shared_ptr<Core::ECS::Entity>> planeEntities;

    for(auto& node : testModel->m_nodes)
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

    // -------

    std::vector<std::shared_ptr<Core::ECS::Entity>> btrEntities;

    for(auto& node : btrModel->m_nodes)
    {
        /*processLoadedNode(node, { 0, -1, -20 }, { 0, -90, 0 },
                          { 4, 4, 4 }, btrEntities);*/
        processLoadedNode(node, { 100.0, -0.5f, 0 }, { 0, 0, 0 },
                          { 0.5, 0.5, 0.5 }, btrEntities);
    }

    for(const auto& entity : btrEntities)
    {
        testScene->m_entities.push_back(entity);

        auto meshComponent = entity->getComponent<Core::ECS::MeshComponent>();
        if(meshComponent)
        {
            meshComponent->m_enableFacesCulling = false;
        }
    }

    std::vector<std::shared_ptr<Core::ECS::Entity>> cubeEntities;

    for(auto& node : cubeModel->m_nodes)
    {
        processLoadedNode(node, { 0, 0, 0 }, { 0, 0, 0 },
                          { 39, 39, 39 }, cubeEntities);
    }

    std::vector<std::shared_ptr<Core::ECS::Entity>> cube1Entities;

    for(auto& node : cubeModel1->m_nodes)
    {
        processLoadedNode(node, { -5, 3, -20 }, { 0, 0, 0 },
                          { 0.1, 0.4, 0.1 }, cube1Entities);
    }

    for (const auto& entity: cube1Entities)
    {
        testScene->m_entities.push_back(entity);
    }

    for(const auto& entity : cubeEntities)
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

    testCameraEntity = std::make_shared<Core::ECS::Entity>();
    auto cameraTransformComponent = std::make_shared<Core::ECS::TransformComponent>();
    cameraTransformComponent->m_position.y = -3;
    cameraTransformComponent->m_position.z = 2;
    cameraTransformComponent->m_rotation.x = -30;
    //cameraTransformComponent->m_position.x = -5;
    testCameraEntity->addComponent(cameraTransformComponent);
    testCameraEntity->addComponent(std::make_shared<Core::ECS::CameraComponent>());

    testScene->m_entities.push_back(testCameraEntity);

    testShadowsCaster = std::make_shared<Core::ECS::Entity>();
    testScene->m_entities.push_back(testShadowsCaster);
    auto shadowsCasterTransform = std::make_shared<Core::ECS::TransformComponent>();
    shadowsCasterTransform->m_position.y = 15;
    shadowsCasterTransform->m_position.z = 5.0;
    shadowsCasterTransform->m_position.x = -5.0;
    shadowsCasterTransform->m_rotation.x = 50;
    //shadowsCasterTransform->m_rotation.y = -90;
    auto shadowCasterComponent = std::make_shared<Core::ECS::ShadowsCasterComponent>();
    testShadowsCaster->addComponent(shadowsCasterTransform);
    testShadowsCaster->addComponent(shadowCasterComponent);
    testShadowsCaster->addComponent(std::make_shared<Core::ECS::DirectionalLightComponent>());

    /*auto testShadowsCaster1 = std::make_shared<Core::ECS::Entity>();
    testScene->m_entities.push_back(testShadowsCaster1);
    auto shadowsCasterTransform1 = std::make_shared<Core::ECS::TransformComponent>();
    shadowsCasterTransform1->m_position.x = 0;
    shadowsCasterTransform1->m_position.y = -3;
    shadowsCasterTransform1->m_position.z = 2.0;
    shadowsCasterTransform1->m_rotation.x = -30;
    //shadowsCasterTransform1->m_rotation.y = 30;
    auto shadowCasterComponent1 = std::make_shared<Core::ECS::ShadowsCasterComponent>();
    testShadowsCaster1->addComponent(shadowsCasterTransform1);
    testShadowsCaster1->addComponent(shadowCasterComponent1);
    testShadowsCaster1->addComponent(std::make_shared<Core::ECS::DirectionalLightComponent>());*/
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------


int framesCnt = 0;

void framePostRender()
{
    testShadowsCaster->getComponent<Core::ECS::TransformComponent>()->m_position.y += sin(framesCnt / 75.0) / 10.0;

    Core::ECS::ECSWorld::update(Core::ECS::Scene::getCurrentScene());

    framesCnt++;
}

// --------------------------------------------

void deltaUpdate(const double& deltaTime)
{
    Core::ECS::ECSWorld::deltaUpdate(Core::ECS::Scene::getCurrentScene(), deltaTime);
}

int main()
{
    //SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);
    sgSetDeltaUpdateCallback(deltaUpdate);

    Core::Main::CoreMain::start();

    //SGConsole::Console::stop();

    return EXIT_SUCCESS;
}