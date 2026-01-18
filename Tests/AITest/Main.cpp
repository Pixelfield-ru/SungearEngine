//
// Created by stuka on 13.12.2025.
//

#define GLM_ENABLE_EXPERIMENTAL

#include "Main.h"

#include <glm/gtx/string_cast.hpp>

#include "SGCore/AI/GOAP/Goal.h"
#include "SGCore/AI/GOAP/Plan.h"
#include "SGCore/AI/GOAP/Solver.h"
#include "SGCore/AI/GOAP/StandardActions/Goto.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Serde/Components/NonSavable.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/LayeredFrameReceiver.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Input/PCInput.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Navigation/NavMesh/NavMesh.h"
#include "SGCore/Navigation/NavMesh/Steps/InputFilteringStep.h"
#include "SGCore/Navigation/NavMesh/Steps/RegionsPartitionStep.h"
#include "SGCore/Navigation/NavMesh/Steps/VoxelizationStep.h"
#include "SGCore/Render/DebugDraw.h"
#include "SGCore/Render/Lighting/SpotLight.h"
#include "Sources/States.h"
#include "Sources/Actions/FindCart.h"
#include "Sources/Actions/PickupCart.h"
#include "Sources/Components/Cart.h"

SGCore::Ref<SGCore::Scene> scene;
SGCore::ECS::entity_t mainCamera{};
SGCore::ECS::entity_t atmosphereEntity{};
SGCore::ECS::entity_t navMeshEntity{};
SGCore::ECS::entity_t lightEntity{};
SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;

std::vector<SGCore::ECS::entity_t> meshesEntities;

SGCore::AssetRef<SGCore::ModelAsset> cartModel;
SGCore::AssetRef<SGCore::ModelAsset> vegetableModel;
SGCore::AssetRef<SGCore::ModelAsset> npcModel;

std::vector<SGCore::ECS::entity_t> vegetables;

SGCore::ECS::entity_t cartEntity = entt::null;
SGCore::ECS::entity_t npcEntity = entt::null;

SGCore::GOAP::Solver goapSolver;
std::optional<SGCore::GOAP::Plan> pickCartPlan;

void regenerateNavMesh()
{
    const auto ecsRegistry = scene->getECSRegistry();

    auto& navMesh = scene->getECSRegistry()->get<SGCore::Navigation::NavMesh>(navMeshEntity);

    std::vector<SGCore::MathPrimitivesUtils::Triangle<>> navMeshTriangles;

    for(auto e : meshesEntities)
    {
        const auto transform = ecsRegistry->get<SGCore::Transform>(e);
        const auto& mesh = ecsRegistry->tryGet<SGCore::Mesh>(e);

        navMeshTriangles.reserve(navMeshTriangles.size() + mesh->m_base.getMeshData()->m_indices.size() / 3);

        for(size_t i = 0; i < mesh->m_base.getMeshData()->m_indices.size(); i += 3)
        {
            const auto& v0 = mesh->m_base.getMeshData()->m_vertices[mesh->m_base.getMeshData()->m_indices[i + 0]];
            const auto& v1 = mesh->m_base.getMeshData()->m_vertices[mesh->m_base.getMeshData()->m_indices[i + 1]];
            const auto& v2 = mesh->m_base.getMeshData()->m_vertices[mesh->m_base.getMeshData()->m_indices[i + 2]];

            SGCore::MathPrimitivesUtils::Triangle<> tri;
            tri.m_vertices[0] = transform->m_finalTransform.m_animatedModelMatrix * glm::vec4(v0.m_position, 1.0f);
            tri.m_vertices[1] = transform->m_finalTransform.m_animatedModelMatrix * glm::vec4(v1.m_position, 1.0f);
            tri.m_vertices[2] = transform->m_finalTransform.m_animatedModelMatrix * glm::vec4(v2.m_position, 1.0f);

            tri.calculateNormal();

            navMeshTriangles.push_back(tri);
        }
    }

    navMesh.build(std::move(navMeshTriangles));
}

void coreInit()
{
    // ================================================================
    // preparing project

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

    mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "sphere_model",
        "${enginePath}/Resources/models/standard/sphere.obj"
    );

    auto ecsRegistry = scene->getECSRegistry();

    const auto debugDraw = SGCore::RenderPipelinesManager::instance().getCurrentRenderPipeline()->getRenderPass<SGCore::DebugDraw>();
    // debugDraw->setMaxLinesCount(100'000'000);

    // ================================================================

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

    // ================================================================
    // creating skybox

    auto standardCubemapMaterial = mainAssetManager->getOrAddAssetByAlias<SGCore::IMaterial>("standard_skybox_material0");
    standardCubemapMaterial->m_shaders["GeometryPass"] =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *pipelinesManager.getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
    standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;
    // standardCubemapMaterial->addTexture2D(SGTextureSlot::SGTT_SKYBOX, standardCubemap);

    std::vector<SGCore::ECS::entity_t> skyboxEntities;
    auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("sphere_model");
    cubeModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities](auto entity) {
        skyboxEntities.push_back(entity);
        scene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
        scene->getECSRegistry()->remove<SGCore::Pickable>(entity);
        scene->getECSRegistry()->remove<SGCore::TransparentEntityTag>(entity);
    });

    atmosphereEntity = skyboxEntities[2];

    auto& skyboxMesh = scene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
    auto& atmosphereScattering = scene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
    skyboxMesh.m_base.setMaterial(standardCubemapMaterial);

    auto& skyboxTransform = scene->getECSRegistry()->get<SGCore::Transform>(atmosphereEntity);

    skyboxTransform->m_ownTransform.m_scale = { 2000, 2000, 2000 };

    // ================================================================
    // creating navmesh

    auto& navMesh = ecsRegistry->emplace<SGCore::Navigation::NavMesh>(navMeshEntity);
    navMesh.useStandardSteps();
    /*navMesh.m_config.m_cellHeight = 5.0f;
    navMesh.m_config.m_cellSize = 5.0f;*/
    navMesh.m_config.m_cellHeight = 1.0f;
    navMesh.m_config.m_cellSize = 1.0f;
    navMesh.m_config.m_agentRadius = 10.0f;
    // navMesh.m_config.m_agentHeight = 30.0f;
    navMesh.m_config.m_agentHeight = 30.0f;
    navMesh.m_config.m_agentMaxClimb = 10.0f;
    // navMesh.m_config.m_agentMaxClimb = 2.0f;

    // ================================================================
    // loading models

    std::vector<SGCore::ECS::entity_t> locationEntities;

    // auto locationModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/AITest/Resources/location_0/scene.gltf");
    auto locationModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/AITest/Resources/location_1/ai_test.gltf");

    // =================================== npc setup
    npcModel = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/ModelDraw/Resources/Idle.fbx");
    npcModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&](auto entity) {
        if(npcEntity == entt::null)
        {
            npcEntity = entity;
        }
    });

    auto& npcState = ecsRegistry->emplace<SGCore::GOAP::EntityState>(npcEntity);
    auto npcTransform = ecsRegistry->get<SGCore::Transform>(npcEntity);
    npcTransform->m_ownTransform.m_scale = { 0.008, 0.008, 0.008 };
    npcTransform->m_ownTransform.m_position = { 0, 0, 20 };

    // ====================================

    // ==================================== goap setup

    goapSolver.registerActionType<SGCore::GOAP::Goto>();
    goapSolver.registerActionType<FindCart>();
    goapSolver.registerActionType<PickupCart>();

    SGCore::GOAP::Goal pickCartGoal;
    pickCartGoal.addFinalState(HAS_CART);

    pickCartPlan = goapSolver.resolveGoal(*ecsRegistry, npcEntity, pickCartGoal).getBestPlan();
    if(!pickCartPlan)
    {
        std::cout << "cannot find plan to pick cart!" << std::endl;
    }

    // ====================================

    vegetableModel = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/AITest/Resources/vegetable_0/scene.gltf");

    cartModel = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/AITest/Resources/cart/scene.gltf");
    cartModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&](auto entity) {
        if(cartEntity == entt::null)
        {
            cartEntity = entity;
        }
    });

    ecsRegistry->emplace<Cart>(cartEntity);

    using namespace std::chrono_literals;
    startSpawnVegetables(*ecsRegistry, { 10, 0, 0 }, 10.0f, { 5000ms, 15000ms });
}

SGCore::ECS::entity_t spawnVegetable(SGCore::ECS::registry_t& registry, const glm::vec3& position, const glm::vec3& scale)
{
    SGCore::ECS::entity_t vegetableEntity = entt::null;
    vegetableModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&](auto entity) {
        if(vegetableEntity == entt::null)
        {
            vegetableEntity = entity;
        }
    });

    auto& transform = registry.get<SGCore::Transform>(vegetableEntity);
    transform->m_ownTransform.m_position = position;
    transform->m_ownTransform.m_scale = scale;

    return vegetableEntity;
}

SGCore::Coro::Task<> startSpawnVegetables(SGCore::ECS::registry_t& registry, glm::vec3 areaPosition, float radius, std::pair<std::chrono::milliseconds, std::chrono::milliseconds> interval)
{
    static std::random_device rd;
    static std::mt19937 gen(rd());

    volatile constexpr bool running = true;
    while(running)
    {
        glm::vec3 vegetableDirection{};

        {
            std::uniform_real_distribution<float> dis(-1, 1);
            vegetableDirection.x = dis(gen);
        }

        {
            std::uniform_real_distribution<float> dis(-1, 1);
            vegetableDirection.z = dis(gen);
        }

        {
            std::uniform_real_distribution<float> dis(0, radius);
            vegetableDirection *= dis(gen);
        }

        spawnVegetable(registry, areaPosition + vegetableDirection, { 0.1, 0.1, 0.1 });

        co_await interval.first;
    }
}

void onUpdate(const double& dt, const double& fixedDt)
{
    const auto currentScene = SGCore::Scene::getCurrentScene();

    if(!currentScene) return;

    currentScene->update(dt, fixedDt);

    SGCore::CoreMain::getRenderer()->renderTextureOnScreen(attachmentToDisplay.get(), false);

    const auto debugDraw = SGCore::RenderPipelinesManager::instance().getCurrentRenderPipeline()->getRenderPass<SGCore::DebugDraw>();

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_0))
    {
        if(debugDraw->m_mode == SGCore::DebugDrawMode::NO_DEBUG)
        {
            debugDraw->m_mode = SGCore::DebugDrawMode::WIREFRAME;
        }
        else
        {
            debugDraw->m_mode = SGCore::DebugDrawMode::NO_DEBUG;
        }
    }

    /*if(debugDraw->m_mode != SGCore::DebugDrawMode::NO_DEBUG)
    {
        auto& navMesh = scene->getECSRegistry()->get<SGCore::Navigation::NavMesh>(navMeshEntity);

        const auto inputFilteringStep = navMesh.getStep<SGCore::Navigation::InputFilteringStep>();
        const auto voxelizationStep = navMesh.getStep<SGCore::Navigation::VoxelizationStep>();
        const auto regionsPartitionStep = navMesh.getStep<SGCore::Navigation::RegionsPartitionStep>();

        const auto& navMeshConfig = navMesh.m_config;

        for(const auto& region : regionsPartitionStep->m_regions)
        {
            for(auto idx : region.m_contourVoxelsIndices)
            {
                const auto& voxel = voxelizationStep->m_voxels[idx];

                const glm::vec3 p = voxelizationStep->voxelToWorld(
                    voxel.m_position,
                    navMeshConfig.m_cellSize,
                    navMeshConfig.m_cellHeight);

                debugDraw->drawLine(p, p + glm::vec3 { 0.0f, 1.0f, 0.0f }, { 0, 0, 1, 1.0 });
            }
        }

        for(const auto& voxel : voxelizationStep->m_voxels)
        {
            const glm::vec3 min = voxelizationStep->voxelToWorld(
                                      voxel.m_position, navMeshConfig.m_cellSize,
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
    }*/

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_1))
    {
        regenerateNavMesh();
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_2))
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_3) && !pickCartPlan->isExecuting())
    {
        pickCartPlan->execute(*currentScene->getECSRegistry(), npcEntity);
    }
}

void onFixedUpdate(const double& dt, const double& fixedDt)
{
    const auto currentScene = SGCore::Scene::getCurrentScene();

    if(currentScene)
    {
        currentScene->fixedUpdate(dt, fixedDt);
    }
}

int main()
{
    SGCore::CoreMain::getRenderTimer().onUpdate += onUpdate;
    SGCore::CoreMain::getFixedTimer().onUpdate += onFixedUpdate;
    SGCore::CoreMain::onInit += coreInit;

    SGCore::CoreMain::init();
    SGCore::CoreMain::startCycle();

    return 0;
}