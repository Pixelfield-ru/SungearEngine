//#define SUNGEAR_DEBUG

#include "CoreTest.h"

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
#include "SGCore/Main/Callbacks.h"
#include "glm/gtx/euler_angles.hpp"
#include "glm/gtx/quaternion.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/ECSWorld.h"

std::shared_ptr<Core::Memory::Assets::ModelAsset> testModel;

std::shared_ptr<Core::ECS::Entity> testCameraEntity;
std::shared_ptr<Core::ECS::Scene> testScene;

// TODO: ALL THIS CODE WAS WRITTEN JUST FOR THE SAKE OF THE TEST. remove

// example how to convert imported scene to Sungear ECS scene
void processLoadedNode(const std::shared_ptr<Core::ImportedScene::Node>& sgNode)
{
    std::shared_ptr<Core::ECS::Entity> nodeEntity = std::make_shared<Core::ECS::Entity>();
    nodeEntity->m_name = sgNode->m_name;

    testScene->m_entities.push_back(nodeEntity);

    for(auto& mesh : sgNode->m_meshes)
    {
        std::shared_ptr<Core::ECS::TransformComponent> transformComponent = std::make_shared<Core::ECS::TransformComponent>();
        // hardcoded transformations =)
        transformComponent->m_position = glm::vec3(0, 0, -10);
        //transformComponent->m_scale = glm::vec3(0.1, 0.1, 0.1);
        transformComponent->m_scale = glm::vec3(2, 2, 2);

        std::shared_ptr<Core::ECS::MeshComponent> meshComponent = std::make_shared<Core::ECS::MeshComponent>();
        meshComponent->m_mesh = mesh;

        std::shared_ptr<Core::ECS::Entity> meshedEntity = std::make_shared<Core::ECS::Entity>();
        meshedEntity->addComponent(transformComponent);
        meshedEntity->addComponent(meshComponent);

        testScene->m_entities.push_back(meshedEntity);
    }

    for(auto& node : sgNode->m_children)
    {
        processLoadedNode(node);
    }
}

void init()
{
    testCameraEntity = std::make_shared<Core::ECS::Entity>();
    testCameraEntity->addComponent(std::make_shared<Core::ECS::TransformComponent>());
    testCameraEntity->addComponent(std::make_shared<Core::ECS::CameraComponent>());

    // найс это работает. TODO: убрать! просто ради теста ---------------------
    int windowWidth;
    int windowHeight;

    Core::Main::Core::getWindow().getSize(windowWidth, windowHeight);
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/sponza_new/NewSponza_Main_glTF_002.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/sponza/sponza.obj");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/trees/NewSponza_CypressTree_glTF.gltf");
    testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/btr_80a2016/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/stalker/mercenary_exo/Mercenary Exoskeleton.obj");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/lenin/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/ak74m/scene.gltf");
    //testModel = Core::Memory::AssetManager::loadAsset<Core::Memory::Assets::ModelAsset>("../SGResources/models/test/train_ep20/scene.gltf");

    testScene = std::make_shared<Core::ECS::Scene>();
    Core::ECS::Scene::setCurrentScene(testScene);

    for(auto& node : testModel->m_nodes)
    {
        processLoadedNode(node);
    }

    testScene->m_entities.push_back(testCameraEntity);
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
    SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFramePostRenderCallback(framePostRender);
    sgSetDeltaUpdateCallback(deltaUpdate);

    Core::Main::Core::start();

    SGConsole::Console::stop();

    return 0;
}