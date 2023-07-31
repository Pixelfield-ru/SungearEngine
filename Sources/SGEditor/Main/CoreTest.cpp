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
#include "SGCore/ECS/Rendering/ShadowsCasterComponent.h"

std::shared_ptr<Core::Memory::Assets::ModelAsset> testModel;

std::shared_ptr<Core::ECS::Entity> testCameraEntity;
std::shared_ptr<Core::ECS::Scene> testScene;

// TODO: ALL THIS CODE WAS WRITTEN JUST FOR THE SAKE OF THE TEST. remove

// example how to convert imported scene to Sungear ECS scene
void processLoadedNode(const std::shared_ptr<Core::ImportedScene::Node>& sgNode,
                       const glm::vec3& pos,
                       const glm::vec3& rot,
                       const glm::vec3& scale)
{
    std::shared_ptr<Core::ECS::Entity> nodeEntity = std::make_shared<Core::ECS::Entity>();
    nodeEntity->addComponent(std::make_shared<Core::ECS::TransformComponent>());
    nodeEntity->m_name = sgNode->m_name;

    testScene->m_entities.push_back(nodeEntity);

    static float cur = 1;

    for(auto& mesh : sgNode->m_meshes)
    {
        std::shared_ptr<Core::ECS::TransformComponent> meshedEntityTransformComponent = std::make_shared<Core::ECS::TransformComponent>();
        // hardcoded transformations =)
        meshedEntityTransformComponent->m_position = pos;
        meshedEntityTransformComponent->m_rotation = rot;
        //transformComponent->m_scale = glm::vec3(0.1, 0.1, 0.1);
        meshedEntityTransformComponent->m_scale = scale;

        std::shared_ptr<Core::ECS::MeshComponent> meshComponent = std::make_shared<Core::ECS::MeshComponent>();
        meshComponent->m_mesh = mesh;

        std::shared_ptr<Core::ECS::Entity> meshedEntity = std::make_shared<Core::ECS::Entity>();
        meshedEntity->addComponent(meshedEntityTransformComponent);
        meshedEntity->addComponent(meshComponent);

        testScene->m_entities.push_back(meshedEntity);

        cur += 0.05;
    }

    for(auto& node : sgNode->m_children)
    {
        processLoadedNode(node, pos, rot, scale);
    }
}

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
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/lenin/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/ak74m/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/train_ep20/scene.gltf");
    testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            "../SGResources/models/test/plane.obj"
            );

    auto btrModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>(
            "../SGResources/models/test/btr_80a2016/scene.gltf"
            //"../SGResources/models/test/sponza/sponza.obj"
            //"../SGResources/models/test/stalker/mercenary_exo/Mercenary Exoskeleton.obj"
            //"../SGResources/models/test/lenin/scene.gltf"
    );

    for(auto& node : testModel->m_nodes)
    {
        processLoadedNode(node, { 0, -3, -15 }, { }, { 1, 1, 1 });
    }

    for(auto& node : btrModel->m_nodes)
    {
        processLoadedNode(node, { 0, 0, -7 }, { 0, 90, 0 }, { 2, 2, 2 });
        //processLoadedNode(node, { 0, -2, -4 }, { -90, 0, 0 }, { 0.005, 0.005, 0.005 });
    }

    testCameraEntity = std::make_shared<Core::ECS::Entity>();
    //testCameraEntity->addComponent(std::make_shared<Core::ECS::TransformComponent>());
    //testCameraEntity->addComponent(std::make_shared<Core::ECS::ShadowsCasterComponent>());
    auto cameraTransformComponent = std::make_shared<Core::ECS::TransformComponent>();
    cameraTransformComponent->m_position.z = 0;
    testCameraEntity->addComponent(cameraTransformComponent);
    testCameraEntity->addComponent(std::make_shared<Core::ECS::CameraComponent>());

    testScene->m_entities.push_back(testCameraEntity);

    auto testShadowsCaster = std::make_shared<Core::ECS::Entity>();
    testScene->m_entities.push_back(testShadowsCaster);
    auto shadowsCasterTransform = std::make_shared<Core::ECS::TransformComponent>();
    shadowsCasterTransform->m_position.y = -5;
    shadowsCasterTransform->m_position.z = 0.0;
    shadowsCasterTransform->m_rotation.x = 30;
    auto shadowCasterComponent = std::make_shared<Core::ECS::ShadowsCasterComponent>();
    /*shadowCasterComponent->m_projectionMatrix = glm::ortho(-10.0f, 10.0f,
                                                           -10.0f, 10.0f,
                                                           shadowCasterComponent->m_zNear,
                                                           shadowCasterComponent->m_zFar);*/
    testShadowsCaster->addComponent(shadowsCasterTransform);
    testShadowsCaster->addComponent(shadowCasterComponent);
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------


void framePostRender()
{
    Core::ECS::ECSWorld::update(Core::ECS::Scene::getCurrentScene());
}

// --------------------------------------------

void deltaUpdate(const double& deltaTime)
{
    Core::ECS::ECSWorld::deltaUpdate(Core::ECS::Scene::getCurrentScene(), deltaTime);
}

int main()
{
    std::cout << "sdfsdfsdf" << std::endl;

    //SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);
    sgSetDeltaUpdateCallback(deltaUpdate);

    Core::Main::CoreMain::start();

    //SGConsole::Console::stop();

    return EXIT_SUCCESS;
}