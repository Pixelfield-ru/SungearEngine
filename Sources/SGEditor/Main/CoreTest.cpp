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
#include "SGCore/ECS/Rendering/Primitives/BoxComponent.h"
#include <imgui/imgui.h>

std::shared_ptr<Core::Memory::Assets::ModelAsset> testModel;

std::shared_ptr<Core::ECS::Entity> testCameraEntity;
std::shared_ptr<Core::ECS::Scene> testScene;

ImColor f;

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
        meshedEntityTransformComponent->m_position = sgNode->m_position + pos;
        meshedEntityTransformComponent->m_rotation = eulerAngles(sgNode->m_rotationQuaternion) + rot;
        meshedEntityTransformComponent->m_scale = sgNode->m_scale * scale;

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
            //"../SGResources/models/test/sponza_new/NewSponza_Main_glTF_002.gltf"
            //"../SGResources/models/test/gaz-66.obj"
            //"../SGResources/models/test/t62/scene.gltf"
            //"../SGResources/models/test/stalk_bagger/bagger.fbx"
            //"../SGResources/models/test/cnpp/scene.gltf"
            //"../SGResources/models/test/apb/scene.gltf"
            //"../SGResources/models/test/btr_80a2016/scene.gltf"
            //"../SGResources/models/test/btr_80/scene.gltf"
            //"../SGResources/models/test/train_ep20/scene.gltf"
            //"../SGResources/models/test/trees/NewSponza_CypressTree_glTF.gltf"
            //"../SGResources/models/test/stalker/stalk_e/fbx/stalker_1.fbx"
            //"../SGResources/models/test/hamada_gun/scene.gltf"
            //"../SGResources/models/test/ak74/scene.gltf"
            //"../SGResources/models/test/backpack/scene.gltf"
            //"../SGResources/models/test/stalk_bunk/bunker.fbx"
            //"../SGResources/models/test/Duty Exoskeleton/Duty Exoskeleton.obj"
            //"../SGResources/models/test/room/room.obj"
            //"../SGResources/models/test/sponza/sponza.obj"
            //"../SGResources/models/test/stalker/mercenary_exo/Mercenary Exoskeleton.obj"
            //"../SGResources/models/test/stalker/agroprom/agro_fbx.fbx"
            //"../SGResources/models/test/uaz/scene.gltf"
            "../SGResources/models/test/wooden_table/scene.gltf"
            //"../SGResources/models/test/svd/scene.gltf"
            //"../SGResources/models/test/yamato/scene.gltf"
            //"../SGResources/models/test/vss/scene.gltf"
            //"../SGResources/models/test/vsk94/scene.gltf"
            //"../SGResources/models/test/helicopter/scene.gltf"
            //"../SGResources/models/test/metal_door/scene.gltf"
            //"../SGResources/models/test/ak47/scene.gltf"
            //"../SGResources/models/test/pavlov/scene.gltf"
            //"../SGResources/models/test/kv2/scene.gltf"
            //"../SGResources/models/test/Putin/scene.gltf"
            //"../SGResources/models/test/Russia_flag/scene.gltf"
            //"../SGResources/models/test/old_building/scene.gltf"
            //"../SGResources/models/test/cathedral/scene.gltf"
            //"../SGResources/models/test/stierlitz/scene.gltf"
            //"../SGResources/models/test/panelka/scene.gltf"
            //"../SGResources/models/test/t55a/scene.gltf"
            //"../SGResources/models/test/rpg7/scene.gltf"
            //"../SGResources/models/test/zucchini/scene.gltf"
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
        testScene->addEntity(entity);
    }

    // -------

    std::vector<std::shared_ptr<Core::ECS::Entity>> btrEntities;

    for(auto& node : btrModel->m_nodes)
    {
        /*processLoadedNode(node, { 0, -1, -20 }, { 0, 0, 0 },
                          { 4, 4, 4 }, btrEntities);*/
        /*processLoadedNode(node, { 0, -1, -20 }, { 0, 0, 0 },
                          { 0.25, 0.25, 0.25 }, btrEntities);*/
        /*processLoadedNode(node, { 0, -1, -20 }, { 90, 0, 0 },
                          { 1, 1, 1 }, btrEntities);*/
        processLoadedNode(node, { 0, -1.65, -20 }, { 0, 90, 0 },
                          { 0.1, 0.1, 0.1 }, btrEntities);
        /*processLoadedNode(node, { 0, 0, -20 }, { 90, 0, 90 },
                          { 0.005, 0.005, 0.005 }, btrEntities);*/
        /*processLoadedNode(node, { 0, 0, -20 }, { 90, 0, 0 },
                          { 0.005, 0.005, 0.005 }, btrEntities);*/
        /*processLoadedNode(node, { 3, -3, -20 }, { 90, 0, 0 },
                          { 0.075, 0.075, 0.075 }, btrEntities);*/
        /*processLoadedNode(node, { 3, -1, -20 }, { 90, 0, 90 },
                          { 0.2, 0.2, 0.2 }, btrEntities);*/
        /*processLoadedNode(node, { 3, 1.5, -20 }, { 90, 0, 0 },
                          { 0.2, 0.2, 0.2 }, btrEntities);*/

        // for uaz
        /*processLoadedNode(node, { 3, -3, -20 }, { 90, 0, 0 },
                          { 0.0025, 0.0025, 0.0025 }, btrEntities);*/
        /*processLoadedNode(node, { 3, -3, -20 }, { 0, 0, 0 },
                          { 0.0025, 0.0025, 0.0025 }, btrEntities);*/
        /*processLoadedNode(node, { 0.0, -3.0, -20 }, { 90, 0, 0 },
                          { 0.01, 0.01, 0.01 }, btrEntities);*/
        /*processLoadedNode(node, { 0, -1, -20 }, { 90, 0, 90 },
                          { 0.025, 0.025, 0.025 }, btrEntities);*/
        /*processLoadedNode(node, { 0, -1, -20 }, { 0, -90, 0 },
                          { 20, 20, 20 }, btrEntities);*/
    }

    for(const auto& entity : btrEntities)
    {
        testScene->addEntity(entity);

        auto meshComponent = entity->getComponent<Core::ECS::MeshComponent>();
        if(meshComponent)
        {
            //meshComponent->m_enableFacesCulling = false;
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
        testScene->addEntity(entity);
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
        testScene->addEntity(entity);
    }

    testCameraEntity = std::make_shared<Core::ECS::Entity>();
    testCameraEntity->m_name = "SGMainCamera";
    auto cameraTransformComponent = std::make_shared<Core::ECS::TransformComponent>();
    cameraTransformComponent->m_position.y = -3;
    cameraTransformComponent->m_position.z = 2;
    cameraTransformComponent->m_rotation.x = -30;
    //cameraTransformComponent->m_position.x = -5;
    testCameraEntity->addComponent(cameraTransformComponent);
    testCameraEntity->addComponent(std::make_shared<Core::ECS::CameraComponent>());

    testScene->addEntity(testCameraEntity);

    testShadowsCaster = std::make_shared<Core::ECS::Entity>();
    testScene->addEntity(testShadowsCaster);
    auto shadowsCasterTransform = std::make_shared<Core::ECS::TransformComponent>();
    shadowsCasterTransform->m_position.y = 15;
    shadowsCasterTransform->m_position.z = 5.0;
    shadowsCasterTransform->m_position.x = -5.0;
    shadowsCasterTransform->m_rotation.x = 50;
    //shadowsCasterTransform->m_rotation.y = -90;
    auto shadowCasterComponent = std::make_shared<Core::ECS::ShadowsCasterComponent>();
    testShadowsCaster->addComponent(shadowsCasterTransform);
    testShadowsCaster->addComponent(shadowCasterComponent);
    auto directionalLight = std::make_shared<Core::ECS::DirectionalLightComponent>();
    directionalLight->m_color.r = 250.0f / 255.0f;
    directionalLight->m_color.g = 129.0f / 255.0f;
    directionalLight->m_color.b = 0.0f / 255.0f;
    //directionalLight->m_color.r = 255.0f / 255.0f * 2.0f;
    //directionalLight->m_color.g = 255.0f / 255.0f * 2.0f;
    //directionalLight->m_color.b = 255.0f / 255.0f * 2.0f;
    testShadowsCaster->addComponent(directionalLight);
    testShadowsCaster->addComponent(std::make_shared<Core::ECS::BoxComponent>());

    std::cout << "bam bam bam mi" << std::endl;

    auto testShadowsCaster1 = std::make_shared<Core::ECS::Entity>();
    testScene->addEntity(testShadowsCaster1);
    auto shadowsCasterTransform1 = std::make_shared<Core::ECS::TransformComponent>();
    shadowsCasterTransform1->m_position.x = -10;
    shadowsCasterTransform1->m_position.y = 10;
    shadowsCasterTransform1->m_position.z = -50.0;
    shadowsCasterTransform1->m_rotation.y = 180;
    //shadowsCasterTransform1->m_rotation.x = 40;
    //shadowsCasterTransform1->m_rotation.y = 30;
    auto shadowCasterComponent1 = std::make_shared<Core::ECS::ShadowsCasterComponent>();
    testShadowsCaster1->addComponent(shadowsCasterTransform1);
    testShadowsCaster1->addComponent(shadowCasterComponent1);
    auto directionalLight1 = std::make_shared<Core::ECS::DirectionalLightComponent>();
    directionalLight1->m_color.r = 139.0f / 255.0f;
    directionalLight1->m_color.g = 184.0f / 255.0f;
    directionalLight1->m_color.b = 241.0f / 255.0f;
    //directionalLight1->m_intensity = 10.0f;
    testShadowsCaster1->addComponent(directionalLight1);
    testShadowsCaster1->addComponent(std::make_shared<Core::ECS::BoxComponent>());
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------

int framesCnt = 0;

void fixedUpdate()
{
    //boxComponent->m_size.z += sin(framesCnt / 75.0) / 10.0;
    //testShadowsCaster->getComponent<Core::ECS::TransformComponent>()->m_rotation.x += sin(framesCnt / 75.0) / 2.0;

    testShadowsCaster->getComponent<Core::ECS::TransformComponent>()->m_position.y += sin(framesCnt / 75.0) / 10.0;

    Core::ECS::ECSWorld::fixedUpdate(Core::ECS::Scene::getCurrentScene());

    framesCnt++;
}

void update()
{
    Core::ECS::ECSWorld::update(Core::ECS::Scene::getCurrentScene());
}

// --------------------------------------------

int main()
{
    //SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFixedUpdateCallback(fixedUpdate);
    sgSetUpdateCallback(update);

    Core::Main::CoreMain::start();

    //SGConsole::Console::stop();

    return EXIT_SUCCESS;
}