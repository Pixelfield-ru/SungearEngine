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

#include "SGCore/ECS/Transformations/TransformationsUpdater.h"

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

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Lighting/ShadowsCaster.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLight.h"
#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/Memory/Assets/CubemapAsset.h"
#include "SGCore/ECS/Rendering/Gizmos/BoxGizmo.h"

#include "SGCore/Utils/ImGuiLayer.h"

SGCore::Ref<SGCore::ModelAsset> testModel;

SGCore::Ref<SGCore::Entity> testCameraEntity;
SGCore::Ref<SGCore::Scene> testScene;

// TODO: ALL THIS CODE WAS WRITTEN JUST FOR THE SAKE OF THE TEST. remove

// example how to convert imported scene to Sungear ECS scene
void processLoadedNode(const SGCore::Ref<SGCore::Node>& sgNode,
                       const glm::vec3& pos,
                       const glm::vec3& rot,
                       const glm::vec3& scale,
                       std::vector<SGCore::Ref<SGCore::Entity>>& outputEntities)
{
    SGCore::Ref<SGCore::Entity> nodeEntity = SGCore::MakeRef<SGCore::Entity>();
    nodeEntity->addComponent(SGCore::MakeRef<SGCore::Transform>());
    nodeEntity->m_name = sgNode->m_name;

    outputEntities.push_back(nodeEntity);

    for(auto& mesh : sgNode->m_meshesData)
    {
        SGCore::Ref<SGCore::Transform> meshedEntityTransformComponent = SGCore::MakeRef<SGCore::Transform>();
        meshedEntityTransformComponent->m_position = sgNode->m_position + pos;
        meshedEntityTransformComponent->m_rotation = eulerAngles(sgNode->m_rotationQuaternion) + rot;
        meshedEntityTransformComponent->m_scale = sgNode->m_scale * scale;

        SGCore::Ref<SGCore::Mesh> meshComponent = SGCore::MakeRef<SGCore::Mesh>();
        meshComponent->m_meshData = mesh;

        SGCore::Ref<SGCore::Entity> meshedEntity = SGCore::MakeRef<SGCore::Entity>();
        meshedEntity->addComponent(meshedEntityTransformComponent);
        meshedEntity->addComponent(meshComponent);

        outputEntities.push_back(meshedEntity);
    }

    for(auto& node : sgNode->m_children)
    {
        processLoadedNode(node, pos, rot, scale, outputEntities);
    }
}

SGCore::Ref<SGCore::Entity> testShadowsCaster;

void init()
{
    testScene = SGCore::MakeRef<SGCore::Scene>();
    SGCore::Scene::setCurrentScene(testScene);

    // найс это работает. TODO: убрать! просто ради теста ---------------------
    int windowWidth;
    int windowHeight;

    SGCore::CoreMain::getWindow().getSize(windowWidth, windowHeight);
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
    testModel = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
            "../SGResources/models/test/plane.obj"
    );

    auto btrModel = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
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
            "../SGResources/models/test/uaz/scene.gltf"
            //"../SGResources/models/test/zis_sport/scene.gltf"
            //"../SGResources/models/test/vodka/scene.gltf"
            //"../SGResources/models/test/mgu/scene.gltf"
            //"../SGResources/models/test/realistic_tree/scene.gltf"
            //"../SGResources/models/test/wooden_table/scene.gltf"
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

    const std::string cubePath = "../SGResources/models/standard/cube.obj";

    auto cubeModel = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
            cubePath
    );

    auto cubeModel1 = SGCore::AssetManager::loadAssetWithAlias<SGCore::ModelAsset>(
            "cube1",
            cubePath
    );

    std::vector<SGCore::Ref<SGCore::Entity>> planeEntities;

    for(auto& node : testModel->m_nodes)
    {
        processLoadedNode(node, { 0, -3, -15 }, { }, { 1, 1, 2 }, planeEntities);
    }

    for(const auto& entity : planeEntities)
    {
        auto meshComponent = entity->getComponent<SGCore::Mesh>();
        auto transformComponent = entity->getComponent<SGCore::Transform>();
        if(meshComponent) meshComponent->m_meshDataRenderInfo.m_enableFacesCulling = false;
        if(transformComponent)
        {
            transformComponent->m_scale = { 400.0, 400.0, 400.0 };
        }
        testScene->addEntity(entity);
    }

    // -------

    std::vector<SGCore::Ref<SGCore::Entity>> btrEntities;

    for(auto& node : btrModel->m_nodes)
    {
        /*processLoadedNode(node, { 0, -1, -20 }, { 0, 0, 0 },
                          { 4, 4, 4 }, btrEntities);*/
        /*processLoadedNode(node, { 0, -1, -20 }, { 0, 0, 0 },
                          { 0.25, 0.25, 0.25 }, btrEntities);*/
        /*processLoadedNode(node, { 5, -3, -20 }, { 0, 0, 0 },
                          { 1, 1, 1 }, btrEntities);*/
        /*processLoadedNode(node, { 5, -1, -20 }, { 0, 0, 0 },
                          { 0.01, 0.01, 0.01 }, btrEntities);*/
        /*processLoadedNode(node, { 0, -1.65, -20 }, { 0, 90, 0 },
                          { 0.1, 0.1, 0.1 }, btrEntities);*/
        /*processLoadedNode(node, { 0, 0, -20 }, { 90, 0, 0 },
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
        processLoadedNode(node, { 3, -3, -20 }, { 90, 0, 0 },
                          { 0.0025, 0.0025, 0.0025 }, btrEntities);
        /*processLoadedNode(node, { 3, -1.7, -20 }, { 0, 0, 0 },
                          { 0.0025, 0.0025, 0.0025 }, btrEntities);*/
        /*processLoadedNode(node, { -0, -3, -20 }, { 90, 0, 0 },
                          { 0.00025 * 2, 0.00025 * 2, 0.00025 * 2 }, btrEntities);*/
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
        testScene->addEntity(entity, SG_LAYER_OPAQUE_NAME);

        auto meshComponent = entity->getComponent<SGCore::Mesh>();
        if(meshComponent)
        {
            // meshComponent->m_meshDataRenderInfo.m_enableFacesCulling = false;
        }
    }

    std::vector<SGCore::Ref<SGCore::Entity>> cubeEntities;

    for(auto& node : cubeModel->m_nodes)
    {
        processLoadedNode(node, { 0, 0, 0 }, { 0, 0, 0 },
                          { 1000, 1000, 1000 }, cubeEntities);
    }

    std::vector<SGCore::Ref<SGCore::Entity>> cube1Entities;

    for(auto& node : cubeModel1->m_nodes)
    {
        processLoadedNode(node, { -5, 3, -30 }, { 0, 0, 0 },
                          { 0.1 * 10.0, 0.4 * 10.0, 0.1 * 10.0 }, cube1Entities);
    }

    auto geniusJPG = SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
            "../SGResources/textures/genius.jpg"
    );

    for (const auto& entity: cube1Entities)
    {
        testScene->addEntity(entity, SG_LAYER_TRANSPARENT_NAME);
        //testScene->addEntity(entity, SG_LAYER_OPAQUE_NAME);

        auto meshComponent = entity->getComponent<SGCore::Mesh>();

        if(meshComponent)
        {
            meshComponent->m_meshData->m_material->m_textures.emplace_back(
                    SGTextureType::SGTP_DIFFUSE,
                    geniusJPG->m_texture2D
            );
        }
    }

    auto standardCubemap = SGCore::AssetManager::loadAsset<SGCore::CubemapAsset>(
            "standard_skybox0",
            SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
            ),
            SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
            ),

            SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
            ),
            SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
            ),

            SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
            ),
            SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
                    "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
            )
    );

    for(const auto& entity : cubeEntities)
    {
        entity->addComponent(SGCore::MakeRef<SGCore::Skybox>());

        auto meshComponent = entity->getComponent<SGCore::Mesh>();
        if(meshComponent)
        {
            meshComponent->m_meshDataRenderInfo.m_enableFacesCulling = false;
            meshComponent->m_meshData->m_material->m_textures.emplace_back(
                    SGTextureType::SGTP_SKYBOX,
                    standardCubemap->getTexture2D()
            );
        }
        testScene->addEntity(entity);
    }

    testCameraEntity = SGCore::MakeRef<SGCore::Entity>();
    testCameraEntity->m_name = "SGMainCamera";
    auto cameraTransformComponent = SGCore::MakeRef<SGCore::Transform>();
    cameraTransformComponent->m_position.y = -3;
    cameraTransformComponent->m_position.z = 2;
    cameraTransformComponent->m_rotation.x = -30;
    //cameraTransformComponent->m_position.x = -5;
    testCameraEntity->addComponent(cameraTransformComponent);
    auto camera = SGCore::MakeRef<SGCore::Camera>();
    testCameraEntity->addComponent(camera);

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    SGCore::Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    camera->addPostProcessLayer("blurPPLayer",
                                testScene->getLayers().find(SG_LAYER_TRANSPARENT_NAME)->second,
                                primaryMonitorWidth,
                                primaryMonitorHeight
    );

    camera->setPostProcessLayerShader(testScene->getLayers().find(SG_LAYER_TRANSPARENT_NAME)->second,
                                      SGCore::Ref<SGCore::IShader>(
                                              SGCore::CoreMain::getRenderer().createShader("../SGResources/shaders/glsl4/postprocessing/test_pp_layer.glsl")
            ));

    testScene->addEntity(testCameraEntity); /// PASSED

    /// THIS CODE CLEARS CACHED COMPONENTS WTF
    testShadowsCaster = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster);
    auto shadowsCasterTransform = SGCore::MakeRef<SGCore::Transform>();
    shadowsCasterTransform->m_position.y = 15;
    shadowsCasterTransform->m_position.z = 5.0;
    shadowsCasterTransform->m_position.x = -5.0;
    shadowsCasterTransform->m_rotation.x = 50;
    //shadowsCasterTransform->m_rotation.y = -90;
    auto shadowCasterComponent = SGCore::MakeRef<SGCore::ShadowsCaster>();
    testShadowsCaster->addComponent(shadowsCasterTransform);
    testShadowsCaster->addComponent(shadowCasterComponent);
    auto directionalLight = SGCore::MakeRef<SGCore::DirectionalLight>();
    directionalLight->m_color.r = 250.0f / 255.0f;
    directionalLight->m_color.g = 129.0f / 255.0f;
    directionalLight->m_color.b = 0.0f / 255.0f;
    //directionalLight->m_color.r = 255.0f / 255.0f * 2.0f;
    //directionalLight->m_color.g = 255.0f / 255.0f * 2.0f;
    //directionalLight->m_color.b = 255.0f / 255.0f * 2.0f;
    testShadowsCaster->addComponent(directionalLight);
    testShadowsCaster->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    std::cout << "bam bam bam mi" << std::endl;

    auto testShadowsCaster1 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster1);
    auto shadowsCasterTransform1 = SGCore::MakeRef<SGCore::Transform>();
    shadowsCasterTransform1->m_position.x = -10;
    shadowsCasterTransform1->m_position.y = 10;
    shadowsCasterTransform1->m_position.z = -50.0;
    shadowsCasterTransform1->m_rotation.y = 180;
    //shadowsCasterTransform1->m_rotation.x = 40;
    //shadowsCasterTransform1->m_rotation.y = 30;
    auto shadowCasterComponent1 = SGCore::MakeRef<SGCore::ShadowsCaster>();
    testShadowsCaster1->addComponent(shadowsCasterTransform1);
    testShadowsCaster1->addComponent(shadowCasterComponent1);
    auto directionalLight1 = SGCore::MakeRef<SGCore::DirectionalLight>();
    directionalLight1->m_color.r = 139.0f / 255.0f;
    directionalLight1->m_color.g = 184.0f / 255.0f;
    directionalLight1->m_color.b = 241.0f / 255.0f;
    //directionalLight1->m_intensity = 10.0f;
    testShadowsCaster1->addComponent(directionalLight1);
    testShadowsCaster1->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    /// -----------------------------------------

    // IMGUI DEBUG -----------------------------------------------------------

    SGCore::ImGuiLayer::initImGui();

    // -----------------------------------------------------------------------

    auto& sys = SGCore::ECSWorld::getSystems();
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------

int framesCnt = 0;

void fixedUpdate()
{
    //boxComponent->m_size.z += sin(framesCnt / 75.0) / 10.0;
    //testShadowsCaster->getComponent<Core::ECS::TransformComponent>()->m_rotation.x += sin(framesCnt / 75.0) / 2.0;

    testShadowsCaster->getComponent<SGCore::Transform>()->m_position.y += sin(framesCnt / 75.0) / 10.0;

    SGCore::ECSWorld::fixedUpdate(SGCore::Scene::getCurrentScene());

    framesCnt++;
}

void update()
{
    SGCore::ImGuiLayer::beginFrame();

    ImGui::Begin("ECS Systems Stats");
    {
        if(ImGui::BeginTable("SystemsStats", 3))
        {
            ImGui::TableNextColumn();
            ImGui::Text("System");
            ImGui::TableNextColumn();
            ImGui::Text("update");
            ImGui::TableNextColumn();
            ImGui::Text("fixedUpdate");
            ImGui::TableNextColumn();

            for(const auto& system : SGCore::ECSWorld::getSystems())
            {
                std::string systemName = std::string(typeid(*(system)).name());
                ImGui::Text(systemName.c_str());

                ImGui::TableNextColumn();

                ImGui::Text((std::to_string(system->getUpdateFunctionExecutionTime()) + " ms").c_str());

                ImGui::TableNextColumn();

                ImGui::Text((std::to_string(system->getFixedUpdateFunctionExecutionTime()) + " ms").c_str());

                ImGui::TableNextColumn();
            }

            ImGui::EndTable();
        }
    }
    ImGui::End();

    //ImGui::ShowDemoWindow();

    SGCore::ECSWorld::update(SGCore::Scene::getCurrentScene());

    SGCore::ImGuiLayer::endFrame();
}

// --------------------------------------------

int main()
{
    //SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFixedUpdateCallback(fixedUpdate);
    sgSetUpdateCallback(update);

    SGCore::CoreMain::start();

    //SGConsole::Console::stop();

    return EXIT_SUCCESS;
}