//
// Created by stuka on 06.02.2025.
//

#include "Main.h"

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/ext.hpp>

#include <SGCore/Memory/AssetManager.h>
#include <SGCore/UI/CSS/CSSFile.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Render/RenderingBase.h>

#include <SGCore/Actions/KeyboardKeyDownAction.h>
#include <SGCore/Actions/KeyboardKeyReleasedAction.h>
#include <SGCore/Motion/MotionPlanner.h>
#include <SGCore/Motion/MotionPlannerConnection.h>
#include <SGCore/Audio/AudioListener.h>
#include <SGCore/Audio/AudioSource.h>
#include <SGCore/Coro/CoroScheduler.h>
#include <SGCore/Graphics/API/IFrameBuffer.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Input/PCInput.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Render/Alpha/TransparentEntityTag.h>
#include <SGCore/Render/Picking/Pickable.h>
#include <SGCore/Render/SpacePartitioning/IgnoreOctrees.h>
#include <SGCore/Serde/Components/NonSavable.h>
#include <SGCore/Transformations/Controllable3D.h>
#include <SGCore/Render/Mesh.h>
#include <SGCore/Render/Atmosphere/Atmosphere.h>
#include <SGCore/Memory/Assets/Materials/IMaterial.h>
#include <SGCore/Graphics/API/ICubemapTexture.h>
#include <SGCore/Memory/Assets/AnimationsFile.h>
#include <SGCore/Memory/Assets/AudioTrackAsset.h>
#include <SGCore/Coro/Task.h>

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
SGCore::AssetRef<SGCore::ITexture2D> someTexture;
SGCore::AssetRef<SGCore::AudioTrackAsset> copterSound;
SGCore::MeshRenderState quadMeshRenderState;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;

SGCore::Ref<SGCore::RenderingBase> cameraRenderingBase { };

SGCore::ECS::entity_t mainCamera;

SGCore::Ref<SGCore::MotionPlannerNode> testIdleNode;

SGCore::ECS::entity_t characterEntity;

void coreInit()
{
    auto mainAssetManager = SGCore::AssetManager::getInstance();

    auto& renderPipelinesManager = SGCore::RenderPipelinesManager::instance();

    // setting pipeline that will render our scene
    auto pbrrpPipeline = renderPipelinesManager.createRenderPipeline<SGCore::PBRRenderPipeline>();
    renderPipelinesManager.registerRenderPipeline(pbrrpPipeline);
    renderPipelinesManager.setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    // creating scene
    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    // setting this scene as current
    SGCore::Scene::setCurrentScene(scene);

    mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "cube_model",
        "${enginePath}/Resources/models/standard/cube.obj"
    );
    // TEST!!!
    someTexture = mainAssetManager->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/no_material.png");

    auto ecsRegistry = scene->getECSRegistry();

    // creating camera ===========================================

    // creating entity
    mainCamera = ecsRegistry->create();

    // creating components for entity
    auto cameraTransform = ecsRegistry->emplace<SGCore::Transform>(mainCamera, SGCore::MakeRef<SGCore::Transform>());
    ecsRegistry->emplace<SGCore::NonSavable>(mainCamera);
    ecsRegistry->emplace<SGCore::Camera3D>(mainCamera, SGCore::MakeRef<SGCore::Camera3D>());
    ecsRegistry->emplace<SGCore::RenderingBase>(mainCamera, SGCore::MakeRef<SGCore::RenderingBase>());
    ecsRegistry->emplace<SGCore::Controllable3D>(mainCamera);
    auto& cameraReceiver = ecsRegistry->emplace<SGCore::LayeredFrameReceiver>(mainCamera);

    cameraTransform->m_ownTransform.m_position.z = 1.5f;
    cameraTransform->m_ownTransform.m_position.y = 1.0f;

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
                    *renderPipelinesManager.getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
    standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;
    standardCubemapMaterial->addTexture2D(SGTextureSlot::SGTT_SKYBOX, standardCubemap);

    // ======

    SGCore::ECS::entity_t atmosphereEntity;

    std::vector<SGCore::ECS::entity_t> skyboxEntities;
    auto cubeModel =  SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("cube_model");
    cubeModel->m_rootNode->addOnScene(scene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities](const auto& entity) {
        skyboxEntities.push_back(entity);
        scene->getECSRegistry()->emplace<SGCore::IgnoreOctrees>(entity);
        scene->getECSRegistry()->remove<SGCore::Pickable>(entity);
        scene->getECSRegistry()->remove<SGCore::TransparentEntityTag>(entity);
    });

    atmosphereEntity = skyboxEntities[2];

    auto& skyboxMesh = scene->getECSRegistry()->get<SGCore::Mesh>(atmosphereEntity);
    auto& atmosphereScattering = scene->getECSRegistry()->emplace<SGCore::Atmosphere>(atmosphereEntity);
    skyboxMesh.m_base.setMaterial(SGCore::AssetManager::getInstance()->getAsset<SGCore::IMaterial, SGCore::AssetStorageType::BY_ALIAS>("standard_skybox_material0"));

    auto& skyboxTransform = scene->getECSRegistry()->get<SGCore::Transform>(atmosphereEntity);

    skyboxTransform->m_ownTransform.m_scale = { 1150, 1150, 1150 };

    // loading audio ================================================================================================
    copterSound = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AudioTrackAsset>("${enginePath}/Tests/ModelDraw/Resources/drone/copter.wav");
    copterSound->toMono();
    std::cout << copterSound->getSummary() << std::endl;

    // creating model ===============================================================================================

    // loading model asset
    /*auto modelAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/ModelDraw/Resources/Fast Run.fbx");
    auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/Fast Run.fbx/skeletons/mixamorig:Hips");*/

    auto modelAsset = SGCore::AssetManager::getInstance()->createAndAddAsset<SGCore::ModelAsset>();

    auto& modelAssetLoadSlot = modelAsset->onLazyLoadDone += [](SGCore::IAsset* thisAsset) {
        auto* modelAsset = static_cast<SGCore::ModelAsset*>(thisAsset);

        // auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/fsb_operator/scene.gltf/skeletons/GLTF_created_0_rootJoint");

        auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf/skeletons/GLTF_created_0_rootJoint");

        // auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/hu_tao_animated/scene.gltf/skeletons/_rootJoint");

        // auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/Fast Run.fbx/skeletons/mixamorig:Hips");

        /*auto modelAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/ModelDraw/Resources/tec/scene.gltf");
        auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/tec/scene.gltf/skeletons/GLTF_created_0_rootJoint");*/

        /*auto modelAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>("${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf");
        auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf/skeletons/GLTF_created_0_rootJoint");*/

        std::vector<SGCore::ECS::entity_t> entities;
        modelAsset->m_rootNode->addOnScene(SGCore::Scene::getCurrentScene(), SG_LAYER_OPAQUE_NAME,
                                           [&entities](const auto& entity) { entities.push_back(entity); });

        // adding animation
        {
            characterEntity = entities[0];

            /*auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/Walking.fbx");
            auto animations1 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/Idle.fbx");

            auto animations = SGCore::AssetManager::getInstance()->getAsset<SGCore::AnimationsFile, SGCore::AssetStorageType::BY_PATH>(
                    "${enginePath}/Tests/ModelDraw/Resources/Fast Run.fbx/animations"
            );*/

            /*auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>(
                "${enginePath}/Tests/ModelDraw/Resources/fsb_operator/scene.gltf/animations");*/

            // auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/tec/scene.gltf/animations");

            auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf/animations");
            // auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/hu_tao_animated/scene.gltf/animations");

            auto& motionPlanner = SGCore::Scene::getCurrentScene()->getECSRegistry()->emplace<SGCore::MotionPlanner>(
                entities[0]);
            motionPlanner.m_skeleton = modelSkeletonAsset;

            auto& copterAudioSource = SGCore::Scene::getCurrentScene()->getECSRegistry()->emplace<SGCore::AudioSource>(
                entities[0]);
            copterAudioSource.create();
            copterAudioSource.attachAudioTrack(copterSound);
            copterAudioSource.setRolloffFactor(0.5f);
            copterAudioSource.setIsLooping(true);
            copterAudioSource.setState(SGCore::AudioSourceState::PLAYING);
            copterAudioSource.setType(SGCore::AudioSourceType::POSITIONAL);

            // SGCore::Transform::reg_t& huTaoTransform = SGCore::Scene::getCurrentScene()->getECSRegistry()->get<SGCore::Transform>(entities[0]);

            // huTaoTransform->m_ownTransform.m_scale = { 0.01, 0.01, 0.01 };

            // sample skeleton
            /*auto idleNode = SGCore::MotionPlannerNode::createNode();
            idleNode->m_isRepeated = true;
            idleNode->m_animationSpeed = 1.0f;
            idleNode->m_skeletalAnimation = animations1->m_skeletalAnimations[0];
            auto idleConnection = SGCore::MakeRef<SGCore::MotionPlannerConnection>();
            idleConnection->m_blendTime = 2.0f;
            idleConnection->m_doNotInterruptWhenInactive = true;
            idleNode->m_anyState.m_toRootConnection = idleConnection;

            auto walkNode = SGCore::MotionPlannerNode::createNode();
            walkNode->m_isRepeated = true;
            walkNode->m_animationSpeed = 1.0f;
            walkNode->m_skeletalAnimation = animations0->m_skeletalAnimations[0];

            auto runNode = SGCore::MotionPlannerNode::createNode();
            runNode->m_isRepeated = true;
            runNode->m_animationSpeed = 0.1f;
            runNode->m_skeletalAnimation = animations->m_skeletalAnimations[0];

            auto walkConnection = SGCore::MakeRef<SGCore::MotionPlannerConnection>();
            walkConnection->m_previousNode = idleNode;
            walkConnection->m_nextNode = walkNode;
            walkConnection->m_blendTime = 0.2f;
            auto walkActivationAction = SGCore::MakeRef<SGCore::KeyboardKeyDownAction>();
            walkActivationAction->m_key = SGCore::KeyboardKey::KEY_W;
            walkConnection->m_activationAction = walkActivationAction;

            auto runConnection = SGCore::MakeRef<SGCore::MotionPlannerConnection>();
            runConnection->m_previousNode = walkNode;
            runConnection->m_nextNode = runNode;
            runConnection->m_blendTime = 2.0f;
            auto runActivationAction = SGCore::MakeRef<SGCore::KeyboardKeyDownAction>();
            runActivationAction->m_key = SGCore::KeyboardKey::KEY_LEFT_SHIFT;
            runConnection->m_activationAction = runActivationAction;

            idleNode->m_connections.push_back(walkConnection);
            walkNode->m_connections.push_back(runConnection);*/

            auto idleNode = SGCore::MotionPlannerNode::createNode();
            idleNode->m_animationSpeed = 1.0f;
            idleNode->m_isRepeated = true;
            idleNode->m_skeletalAnimation = animations0->m_skeletalAnimations[0];

            testIdleNode = idleNode;

            motionPlanner.m_rootNodes.push_back(idleNode);
        }
    };

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/fsb_operator/scene.gltf");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/Fast Run.fbx");

    SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/hu_tao_animated/scene.gltf");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Models/vss/scene.gltf");

    // creating quad model for drawing camera framebuffer attachment to screen ======================================
}

double g_dt = 0.0f;

SGCore::Coro::Task<> moveSmoothly(SGCore::ECS::entity_t entity, glm::vec3 to, float speed)
{
    using namespace std::chrono_literals;

    auto transform = scene->getECSRegistry()->get<SGCore::Transform>(entity);

    while(glm::distance(transform->m_ownTransform.m_position, to) > 0.5f)
    {
        co_await 1ms;

        std::cout << transform->m_ownTransform.m_position << std::endl;

        const auto dif = to - transform->m_ownTransform.m_position;
        transform->m_ownTransform.m_position += dif * speed;
    }
}

void onUpdate(const double& dt, const double& fixedDt)
{
    g_dt = dt;

    const auto currentScene = SGCore::Scene::getCurrentScene();

    if(currentScene)
    {
        auto& cameraTransform = currentScene->getECSRegistry()->get<SGCore::Transform>(mainCamera);
        auto& frameReceiver = currentScene->getECSRegistry()->get<SGCore::LayeredFrameReceiver>(mainCamera);

        SGCore::AudioListener::setPosition(cameraTransform->m_finalTransform.m_position);
        SGCore::AudioListener::setOrientation(cameraTransform->m_finalTransform.m_forward, cameraTransform->m_finalTransform.m_up);

        auto& characterTransform = SGCore::Scene::getCurrentScene()->getECSRegistry()->get<SGCore::Transform>(characterEntity);

        const float characterSpeed = 3.0f;

        if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_UP))
        {
            characterTransform->m_ownTransform.m_position += characterTransform->m_finalTransform.m_up * characterSpeed * dt;
        }
        if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_DOWN))
        {
            characterTransform->m_ownTransform.m_position -= characterTransform->m_finalTransform.m_up * characterSpeed * dt;
        }
        if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_LEFT))
        {
            characterTransform->m_ownTransform.m_position -= characterTransform->m_finalTransform.m_right * characterSpeed * dt;
        }
        if(SGCore::Input::PC::keyboardKeyDown(SGCore::Input::KeyboardKey::KEY_RIGHT))
        {
            characterTransform->m_ownTransform.m_position += characterTransform->m_finalTransform.m_right * characterSpeed * dt;
        }

        if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_M))
        {
            moveSmoothly(characterEntity, characterTransform->m_ownTransform.m_position + characterTransform->m_ownTransform.m_up * 10.0f, 0.01f);
        }

        currentScene->update(dt, fixedDt);

        SGCore::CoreMain::getRenderer()->renderTextureOnScreen(attachmentToDisplay.get(), false);
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_2))
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_3))
    {
        testIdleNode->m_isPaused = !testIdleNode->m_isPaused;
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_MINUS))
    {
        testIdleNode->m_animationSpeed -= 0.1f;
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_KP_ADD))
    {
        testIdleNode->m_animationSpeed += 0.1f;
    }
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
