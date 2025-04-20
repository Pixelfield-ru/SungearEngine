//
// Created by stuka on 06.02.2025.
//

#include "Main.h"

#include <SGCore/Memory/AssetManager.h>
#include <SGCore/UI/CSS/CSSFile.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>
#include <SGCore/Render/LayeredFrameReceiver.h>
#include <SGCore/Render/RenderingBase.h>

#include "SGCore/Actions/KeyboardKeyDownAction.h"
#include "SGCore/Actions/KeyboardKeyReleasedAction.h"
#include "SGCore/Animation/MotionPlanner.h"
#include "SGCore/Animation/MotionPlannerConnection.h"
#include "SGCore/Audio/AudioListener.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Input/InputManager.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Render/SpacePartitioning/IgnoreOctrees.h"
#include "SGCore/Serde/Components/NonSavable.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"
#include "SGCore/Graphics/API/ICubemapTexture.h"
#include "SGCore/Memory/Assets/AnimationsFile.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"

#ifdef PLATFORM_OS_WINDOWS
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

SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::Scene> scene;
SGCore::Ref<SGCore::IMeshData> quadMeshData;
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

    // setting pipeline that will render our scene
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    // creating scene
    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    // setting this scene as current
    SGCore::Scene::setCurrentScene(scene);

    mainAssetManager->loadAssetWithAlias<SGCore::ModelAsset>(
        "cube_model",
        "${enginePath}/Resources/models/standard/cube.obj"
    );
    screenShader = mainAssetManager->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");
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
    standardCubemapMaterial->m_shader =
            mainAssetManager->loadAsset<SGCore::IShader>(
                    *SGCore::RenderPipelinesManager::getCurrentRenderPipeline()->m_shadersPaths["SkyboxShader"]);
    standardCubemapMaterial->m_meshRenderState.m_useFacesCulling = false;
    standardCubemapMaterial->addTexture2D(SGTextureType::SGTT_SKYBOX, standardCubemap);

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
    atmosphereScattering.m_sunRotation.z = 90.0;
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

    auto modelAsset = SGCore::AssetManager::getInstance()->createAsset<SGCore::ModelAsset>();

    auto& modelAssetLoadSlot = modelAsset->onLazyLoadDone += [](SGCore::IAsset* thisAsset) {
        auto* modelAsset = static_cast<SGCore::ModelAsset*>(thisAsset);

        auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/fsb_operator/scene.gltf/skeletons/GLTF_created_0_rootJoint");

        // auto modelSkeletonAsset = SGCore::AssetManager::getInstance()->loadAsset<SGCore::Skeleton>("${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf/skeletons/GLTF_created_0_rootJoint");

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

            auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>(
                "${enginePath}/Tests/ModelDraw/Resources/fsb_operator/scene.gltf/animations");

            // auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/tec/scene.gltf/animations");

            // auto animations0 = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AnimationsFile>("${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf/animations");
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
            copterAudioSource.setState(SGCore::AudioSourceState::STOPPED);
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

    SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/fsb_operator/scene.gltf");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/Fast Run.fbx");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/drone/scene.gltf");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Tests/ModelDraw/Resources/hu_tao_animated/scene.gltf");

    // SGCore::AssetManager::getInstance()->loadAsset<SGCore::ModelAsset>(modelAsset, SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD, "${enginePath}/Models/vss/scene.gltf");

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

void onUpdate(const double& dt, const double& fixedDt)
{
    const auto& mainListener = SGCore::InputManager::getMainInputListener();

    if(SGCore::Scene::getCurrentScene())
    {
        auto& cameraTransform = SGCore::Scene::getCurrentScene()->getECSRegistry()->get<SGCore::Transform>(mainCamera);

        SGCore::AudioListener::setPosition(cameraTransform->m_finalTransform.m_position);
        SGCore::AudioListener::setOrientation(cameraTransform->m_finalTransform.m_forward, cameraTransform->m_finalTransform.m_up);

        auto& characterTransform = SGCore::Scene::getCurrentScene()->getECSRegistry()->get<SGCore::Transform>(characterEntity);

        const float characterSpeed = 3.0f;

        if(mainListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_UP))
        {
            characterTransform->m_ownTransform.m_position += characterTransform->m_finalTransform.m_up * characterSpeed * dt;
        }
        if(mainListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_DOWN))
        {
            characterTransform->m_ownTransform.m_position -= characterTransform->m_finalTransform.m_up * characterSpeed * dt;
        }
        if(mainListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_LEFT))
        {
            characterTransform->m_ownTransform.m_position -= characterTransform->m_finalTransform.m_right * characterSpeed * dt;
        }
        if(mainListener->keyboardKeyDown(SGCore::KeyboardKey::KEY_RIGHT))
        {
            characterTransform->m_ownTransform.m_position += characterTransform->m_finalTransform.m_right * characterSpeed * dt;
        }

        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
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

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_2))
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_3))
    {
        testIdleNode->m_isPaused = !testIdleNode->m_isPaused;
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_MINUS))
    {
        testIdleNode->m_animationSpeed -= 0.1f;
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_KP_ADD))
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

    SGCore::CoreMain::start();

    return 0;
}
