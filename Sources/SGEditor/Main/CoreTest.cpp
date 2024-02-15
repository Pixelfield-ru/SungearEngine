//#define SUNGEAR_DEBUG

#include <cstdlib>

#include "CoreTest.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/API/IShaderUniform.h"
#include "SGCore/Memory/Assets/ModelAsset.h"
#include "SGCore/Memory/Assets/Materials/IMaterial.h"

#include "SGCore/Main/Callbacks.h"

#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "EditorMain.h"
#include "SGCore/ImGuiWrap/Views/Base/Window.h"
#include "SGCore/ImGuiWrap/Views/Base/CollapsingHeader.h"
#include "SGCore/ImGuiWrap/ViewsInjector.h"

#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Graphics/API/ICubemapTexture.h"

#include "SGCore/Input/InputManager.h"

#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/Camera.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Render/Gizmos/BoxGizmo.h"

SGCore::Ref<SGCore::ModelAsset> testModel;

entt::entity testCameraEntity = entt::null;
SGCore::Ref<SGCore::Scene> testScene;

SGCore::Atmosphere* _atmosphereScattering = nullptr;

// TODO: ALL THIS CODE WAS WRITTEN JUST FOR THE SAKE OF THE TEST. remove

void init()
{
    testScene = SGCore::MakeRef<SGCore::Scene>();
    testScene->createDefaultSystems();
    SGCore::Scene::setCurrentScene(testScene);

    // найс это работает. TODO: убрать! просто ради теста ---------------------
    int windowWidth;
    int windowHeight;

    SGCore::CoreMain::getWindow().getSize(windowWidth, windowHeight);

    testModel = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
            "../SGResources/models/test/plane.obj"
    );

    auto model0 = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
            // "../SGResources/models/test/sponza_new/NewSponza_Main_glTF_002.gltf"
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
            // "../SGResources/models/test/sponza/sponza.obj"
            //"../SGResources/models/test/stalker/mercenary_exo/Mercenary Exoskeleton.obj"
            //"../SGResources/models/test/stalker/agroprom/agro_fbx.fbx"
            //"../SGResources/models/test/zis_sport/scene.gltf"
            //"../SGResources/models/test/vodka/scene.gltf"
            //"../SGResources/models/test/mgu/scene.gltf"
            //"../SGResources/models/test/realistic_tree/scene.gltf"
            //"../SGResources/models/test/wooden_table/scene.gltf"
            //"../SGResource
            //"../SGResources/models/test/uaz/scene.gltf"s/models/test/svd/scene.gltf"
            //"../SGResources/models/test/yamato/scene.gltf"
            "../SGResources/models/test/vss/scene.gltf"
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

    auto model1 = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
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
            //"../SGResources/models/test/zis_sport/scene.gltf"
            //"../SGResources/models/test/vodka/scene.gltf"
            //"../SGResources/models/test/mgu/scene.gltf"
            //"../SGResources/models/test/realistic_tree/scene.gltf"
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

    const std::string cubePath = "../SGResources/models/standard/cube.obj";

    auto cubeModel = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
            cubePath
    );

    auto cubeModel1 = SGCore::AssetManager::loadAssetWithAlias<SGCore::ModelAsset>(
            "cube1",
            cubePath
    );

    auto sphereModel = SGCore::AssetManager::loadAsset<SGCore::ModelAsset>(
            "../SGResources/models/standard/sphere.obj"
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    sphereModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_TRANSPARENT_NAME,
                                      [](const entt::entity& entity)
                                      {
                                          auto mesh = testScene->getECSRegistry().try_get<SGCore::Mesh>(entity);
                                          auto transform = testScene->getECSRegistry().try_get<SGCore::Transform>(entity);
                                          if(transform)
                                          {
                                              transform->m_ownTransform.m_position = { 0, 6.0, -20 };
                                              transform->m_ownTransform.m_rotation = { 0, 0, 0 };
                                              transform->m_ownTransform.m_scale = { 1.0, 1.0, 1.0 };
                                          }
                                          if(mesh)
                                          {
                                              mesh->m_base.m_meshData->m_material->setMetallicFactor(1);
                                              mesh->m_base.m_meshData->m_material->setRoughnessFactor(1);

                                              mesh->m_base.m_meshData->m_material->findAndAddTexture2D(
                                                      SGTextureType::SGTT_DIFFUSE,
                                                      "../SGResources/textures/spotted_rust/spotted-rust_albedo.png"
                                              );

                                              mesh->m_base.m_meshData->m_material->findAndAddTexture2D(
                                                      SGTextureType::SGTT_LIGHTMAP,
                                                      "../SGResources/textures/spotted_rust/spotted-rust_ao.png"
                                              );

                                              mesh->m_base.m_meshData->m_material->findAndAddTexture2D(
                                                      SGTextureType::SGTT_METALNESS,
                                                      "../SGResources/textures/spotted_rust/spotted-rust_metallic.png"
                                              );

                                              mesh->m_base.m_meshData->m_material->findAndAddTexture2D(
                                                      SGTextureType::SGTT_NORMALS,
                                                      "../SGResources/textures/spotted_rust/spotted-rust_normal-ogl.png"
                                              );

                                              mesh->m_base.m_meshData->m_material->findAndAddTexture2D(
                                                      SGTextureType::SGTT_DIFFUSE_ROUGHNESS,
                                                      "../SGResources/textures/spotted_rust/spotted-rust_roughness.png"
                                              );
                                          }
                                      }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    testModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                      [](const entt::entity& entity)
                                      {
                                          auto mesh = testScene->getECSRegistry().try_get<SGCore::Mesh>(entity);
                                          auto transform = testScene->getECSRegistry().try_get<SGCore::Transform>(entity);
                                          if(mesh)
                                          {
                                              mesh->m_base.m_meshDataRenderInfo.m_enableFacesCulling = false;
                                              mesh->m_base.m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTT_DIFFUSE, "../SGResources/textures/chess.jpg");
                                              mesh->m_base.m_meshData->setVertexUV(0, 200, 0, 0);
                                              mesh->m_base.m_meshData->setVertexUV(1, 0, 200, 0);
                                              mesh->m_base.m_meshData->setVertexUV(2, 200, 200, 0);
                                              mesh->m_base.m_meshData->prepare();
                                          }
                                          if(transform)
                                          {
                                              transform->m_ownTransform.m_scale = { 10.0, 10.0, 10.0 };
                                          }
                                      }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    model0->m_nodes[0]->addOnScene(testScene, SG_LAYER_TRANSPARENT_NAME,
                                   [](const entt::entity& entity)
    {
        auto transform = testScene->getECSRegistry().try_get<SGCore::Transform>(entity);
        if(transform)
        {
            /*transformComponent->m_position = { -8, 0.1, -20 };
            transformComponent->m_rotation = { 90, 0, 0 };
            transformComponent->m_scale = { 0.002, 0.002, 0.002 };*/

            // svd
            /*transformComponent->m_position = { 3, -1, -20 };
            transformComponent->m_rotation = { 0, 90, 0 };
            transformComponent->m_scale = { 0.2, 0.2, 0.2 };*/

            // vsk94
            /*transformComponent->m_position = { 3, 4, -20 };
            transformComponent->m_rotation = { 90, 0, 0 };
            transformComponent->m_scale = { 0.002, 0.002, 0.002 };*/

            // vss
            transform->m_ownTransform.m_position = { 0, 2.5, -5 };
            transform->m_ownTransform.m_rotation = { 0, 0, 0 };
            transform->m_ownTransform.m_scale = { 1.3, 1.3, 1.3 };

            // RPG
            /*transform->m_ownTransform.m_position = { 1, 20.0, -20 };
            transform->m_ownTransform.m_rotation = { 90, 0, 90 };
            transform->m_ownTransform.m_scale = { 0.8, 0.8, 0.8 };*/

            // sponza old model
            /*transformComponent->m_position = { 3, 2.91, -20 };
            transformComponent->m_rotation = { 0, 0, 0 };
            transformComponent->m_scale = { 0.07, 0.07, 0.07 };*/

            // old building
            /*transformComponent->m_position = { 3, 1, -20 };
            transformComponent->m_rotation = { 90, 0, 0 };
            transformComponent->m_scale = { 0.007, 0.007, 0.007 };*/
        }
    });

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    model1->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                      [](const entt::entity& entity)
                                      {
                                          auto transform = testScene->getECSRegistry().try_get<SGCore::Transform>(entity);
                                          if(transform)
                                          {
                                              // wooden table
                                              transform->m_ownTransform.m_position = { 0, 1.30, -20 };
                                              transform->m_ownTransform.m_rotation = { 0, 90, 0 };
                                              transform->m_ownTransform.m_scale = { 0.8, 0.8, 0.8 };
                                          }
                                      }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    auto standardCubemap = SGCore::Ref<SGCore::ICubemapTexture>(SGCore::CoreMain::getRenderer()->createCubemapTexture());

    standardCubemap->m_parts.push_back(SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/skyboxes/skybox0/standard_skybox0_xleft.png"
    ));
    standardCubemap->m_parts.push_back(SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/skyboxes/skybox0/standard_skybox0_xright.png"
    ));

    standardCubemap->m_parts.push_back(SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/skyboxes/skybox0/standard_skybox0_ytop.png"
    ));
    standardCubemap->m_parts.push_back(SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/skyboxes/skybox0/standard_skybox0_ybottom.png"
    ));

    standardCubemap->m_parts.push_back(SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zfront.png"
    ));
    standardCubemap->m_parts.push_back(SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/skyboxes/skybox0/standard_skybox0_zback.png"
    ));

    standardCubemap->setRawName("standard_skybox0");

    standardCubemap->create();

    SGCore::AssetManager::addAsset("standard_skybox0", standardCubemap);

    auto geniusJPG = SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(
            "../SGResources/textures/genius.jpg"
    );

    geniusJPG->setRawName("GeniusTexture");

    geniusJPG->create();

    // adding skybox
    {
        std::vector<entt::entity> skyboxEntities;
        cubeModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME, [&skyboxEntities](const auto& entity) {
            skyboxEntities.push_back(entity);
        });

        SGCore::Mesh& skyboxMesh = testScene->getECSRegistry().get<SGCore::Mesh>(skyboxEntities[2]);
        SGCore::Atmosphere& atmosphereScattering = testScene->getECSRegistry().emplace<SGCore::Atmosphere>(skyboxEntities[2]);
        _atmosphereScattering = &atmosphereScattering;
        // atmosphereScattering.m_sunRotation.z = 90.0;
        skyboxMesh.m_base.m_meshData->m_material->addTexture2D(SGTextureType::SGTT_SKYBOX,
                standardCubemap
        );
        // это топ пж
        skyboxMesh.m_base.m_meshData->m_material->getShader()->removeSubPass("GeometryPass");
        SGCore::MeshesUtils::loadMeshShader(skyboxMesh.m_base, "SkyboxShader");
        skyboxMesh.m_base.m_meshDataRenderInfo.m_enableFacesCulling = false;

        SGCore::Transform& skyboxTransform = testScene->getECSRegistry().get<SGCore::Transform>(skyboxEntities[2]);
        // auto transformComponent = skyboxEntities[2]->getComponent<SGCore::Transform>();

        skyboxTransform.m_ownTransform.m_scale = { 1150, 1150, 1150 };
    }

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    cubeModel1->m_nodes[0]->addOnScene(testScene, SG_LAYER_TRANSPARENT_NAME,
                                       [geniusJPG](const entt::entity& entity)
                                       {
                                           auto mesh = testScene->getECSRegistry().try_get<SGCore::Mesh>(entity);
                                           auto transform = testScene->getECSRegistry().try_get<SGCore::Transform>(entity);

                                           if(transform)
                                           {
                                               transform->m_ownTransform.m_position = { -5, 3, -30 };
                                               transform->m_ownTransform.m_rotation = { 0, 0, 0 };
                                               transform->m_ownTransform.m_scale = {0.1 * 10.0, 0.2 * 10.0, 0.1 * 10.0 };
                                           }

                                           if(mesh)
                                           {
                                               mesh->m_base.m_meshData->m_material->addTexture2D(SGTextureType::SGTT_DIFFUSE, geniusJPG);
                                               /*mesh->m_base.m_meshData->m_material->m_textures[SGTextureType::SGTT_DIFFUSE].push_back(
                                                       geniusJPG // нупачиму
                                               );*/
                                           }
                                       }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    // --- camera ---------------

    testCameraEntity = testScene->getECSRegistry().create();
    SGCore::EntityBaseInfo& cameraBaseInfo = testScene->getECSRegistry().emplace<SGCore::EntityBaseInfo>(testCameraEntity);
    cameraBaseInfo.setRawName("SGMainCamera");

    SGCore::Transform& cameraTransform = testScene->getECSRegistry().emplace<SGCore::Transform>(testCameraEntity);
    cameraTransform.m_ownTransform.m_position.y = -3;
    cameraTransform.m_ownTransform.m_position.z = 2;
    cameraTransform.m_ownTransform.m_rotation.x = -30;

    SGCore::Camera& cameraEntityCamera = testScene->getECSRegistry().emplace<SGCore::Camera>(testCameraEntity);
    SGCore::Controllable3D& cameraEntityControllable = testScene->getECSRegistry().emplace<SGCore::Controllable3D>(testCameraEntity);
    SGCore::RenderingBase& cameraRenderingBase = testScene->getECSRegistry().emplace<SGCore::RenderingBase>(testCameraEntity);

    // ===============================

    int primaryMonitorWidth;
    int primaryMonitorHeight;

    SGCore::Window::getPrimaryMonitorSize(primaryMonitorWidth, primaryMonitorHeight);

    // =================================================================================
    // ================ POST PROCESS SETUP =============================================
    // =================================================================================

    /*auto& ppLayer = camera->addPostProcessLayer("blurPPLayer",
                                                testScene->getLayers().find(SG_LAYER_TRANSPARENT_NAME)->second,
                                                primaryMonitorWidth,
                                                primaryMonitorHeight
    );

    ppLayer.m_frameBuffer
            ->bind()
            ->setSize(primaryMonitorWidth, primaryMonitorHeight)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->unbind();

    SGCore::PostProcessFXSubPass depthSubPass;
    depthSubPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    depthSubPass.m_index = 0;

    ppLayer.m_subPasses.push_back(depthSubPass);

    ppLayer.m_attachmentsForCombining[SGG_COLOR_ATTACHMENT0] = SGG_COLOR_ATTACHMENT7;

    camera->setPostProcessLayerShader(testScene->getLayers().find(SG_LAYER_TRANSPARENT_NAME)->second,
                                      SGCore::Ref<SGCore::ISubPassShader>(
                                              SGCore::CoreMain::getRenderer().createShader("../SGResources/shaders/glsl4/postprocessing/distortion_fx_layer.glsl")
                                      )->addToGlobalStorage());*/


    // =========== BLOOM SETUP =======================================
    /*ppLayer.m_frameBuffer
            ->bind()
            ->setSize(primaryMonitorWidth, primaryMonitorHeight)
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT6,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->addAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7,
                            SGGColorFormat::SGG_RGB,
                            SGGColorInternalFormat::SGG_RGB8,
                            0,
                            0
            )
            ->unbind();

    SGCore::PostProcessFXSubPass blurLayerDirectionalSubPass;
    blurLayerDirectionalSubPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT6;
    blurLayerDirectionalSubPass.m_index = 0;

    SGCore::PostProcessFXSubPass blurLayerTemporalSubPass;
    blurLayerTemporalSubPass.m_attachmentRenderTo = SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7;
    blurLayerTemporalSubPass.m_index = 1;

    blurLayerDirectionalSubPass.m_prepareFunction = [](const SGCore::Ref<SGCore::IShader>& ppLayerShader)
    {
        ppLayerShader->useInteger("isFirstBlurPass", true);
        ppLayerShader->useInteger("isHorizontalPass", true);
    };
    // first horizontal pass (blur of color attachment 0)
    ppLayer.m_subPasses.push_back(blurLayerDirectionalSubPass);
    blurLayerDirectionalSubPass.m_prepareFunction = [](const SGCore::Ref<SGCore::IShader>& ppLayerShader)
    {
        ppLayerShader->useInteger("isFirstBlurPass", false);
    };
    ppLayer.m_subPasses.push_back(blurLayerTemporalSubPass);
    ppLayer.m_subPasses.push_back(blurLayerDirectionalSubPass);
    ppLayer.m_subPasses.push_back(blurLayerTemporalSubPass);
    ppLayer.m_subPasses.push_back(blurLayerDirectionalSubPass);

    blurLayerTemporalSubPass.m_prepareFunction = [](const SGCore::Ref<SGCore::IShader>& ppLayerShader)
    {
        ppLayerShader->useInteger("isHorizontalPass", false);
    };
    // first vertical pass (blur of horizontal
    ppLayer.m_subPasses.push_back(blurLayerTemporalSubPass);
    ppLayer.m_subPasses.push_back(blurLayerDirectionalSubPass);
    ppLayer.m_subPasses.push_back(blurLayerTemporalSubPass);
    ppLayer.m_subPasses.push_back(blurLayerDirectionalSubPass);
    blurLayerTemporalSubPass.m_prepareFunction = [](const SGCore::Ref<SGCore::IShader>& ppLayerShader)
    {
        ppLayerShader->useInteger("isFinalPass", true);
    };
    ppLayer.m_subPasses.push_back(blurLayerTemporalSubPass);

    ppLayer.m_attachmentsForCombining[SGG_COLOR_ATTACHMENT0] = SGG_COLOR_ATTACHMENT7;

    camera->setPostProcessLayerShader(testScene->getLayers().find(SG_LAYER_TRANSPARENT_NAME)->second,
                                      SGCore::Ref<SGCore::IShader>(
                                              SGCore::CoreMain::getRenderer().createShader("../SGResources/shaders/glsl4/postprocessing/bloom_fx_layer.glsl")
            ));*/

    // =================================================================================
    // =================================================================================
    // =================================================================================

    /*testScene->addEntity(testCameraEntity); /// PASSED

    /// THIS CODE CLEARS CACHED COMPONENTS WTF
    testShadowsCaster = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster);
    auto shadowsCasterTransform = SGCore::MakeRef<SGCore::TransformBase>();
    shadowsCasterTransform->m_position.y = 10;
    shadowsCasterTransform->m_position.z = 15.0;
    shadowsCasterTransform->m_position.x = -5.0;
    //shadowsCasterTransform->m_rotation.x = 50;
    testShadowsCaster->addComponent(shadowsCasterTransform);
    auto directionalLight = SGCore::MakeRef<SGCore::PBRFRPDirectionalLight>();
    // directionalLight->m_color.r = 10.0f / 255.0f;
    // directionalLight->m_color.g = 129.0f / 255.0f;
    // directionalLight->m_color.b = 100.0f / 255.0f;
    directionalLight->m_intensity = 1000.0;
    testShadowsCaster->addComponent(directionalLight);
    testShadowsCaster->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    std::cout << "bam bam bam mi" << std::endl;

    auto testShadowsCaster1 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster1);
    auto shadowsCasterTransform1 = SGCore::MakeRef<SGCore::TransformBase>();
    shadowsCasterTransform1->m_position.x = -10;
    shadowsCasterTransform1->m_position.y = 10;
    shadowsCasterTransform1->m_position.z = -50.0;
    shadowsCasterTransform1->m_rotation.y = 180;
    testShadowsCaster1->addComponent(shadowsCasterTransform1);
    auto directionalLight1 = SGCore::MakeRef<SGCore::PBRFRPDirectionalLight>();
    directionalLight1->m_color.r = 139.0f / 255.0f;
    directionalLight1->m_color.g = 184.0f / 255.0f;
    directionalLight1->m_color.b = 241.0f / 255.0f;
    directionalLight1->m_intensity = 1000.0;
    testShadowsCaster1->addComponent(directionalLight1);
    testShadowsCaster1->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto testShadowsCaster2 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster2);
    auto shadowsCasterTransform2 = SGCore::MakeRef<SGCore::TransformBase>();
    shadowsCasterTransform2->m_position.x = 10;
    shadowsCasterTransform2->m_position.y = 10;
    shadowsCasterTransform2->m_position.z = -50.0;
    shadowsCasterTransform2->m_rotation.y = 180;
    testShadowsCaster2->addComponent(shadowsCasterTransform2);
    auto directionalLight2 = SGCore::MakeRef<SGCore::PBRFRPDirectionalLight>();
    directionalLight2->m_color.r = 20.0f / 255.0f;
    directionalLight2->m_color.g = 184.0f / 255.0f;
    directionalLight2->m_color.b = 241.0f / 255.0f;
    directionalLight2->m_intensity = 500.0;
    testShadowsCaster2->addComponent(directionalLight2);
    testShadowsCaster2->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto testShadowsCaster3 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster3);
    auto shadowsCasterTransform3 = SGCore::MakeRef<SGCore::TransformBase>();
    shadowsCasterTransform3->m_position.x = -20;
    shadowsCasterTransform3->m_position.y = 5;
    shadowsCasterTransform3->m_position.z = -20.0;
    shadowsCasterTransform3->m_rotation.y = 90;
    testShadowsCaster3->addComponent(shadowsCasterTransform3);
    auto directionalLight3 = SGCore::MakeRef<SGCore::PBRFRPDirectionalLight>();
    directionalLight3->m_color.r = 139.0f / 255.0f;
    directionalLight3->m_color.g = 15.0f / 255.0f;
    directionalLight3->m_color.b = 5.0f / 255.0f;
    testShadowsCaster3->addComponent(directionalLight3);
    directionalLight3->m_intensity = 100.0;
    testShadowsCaster3->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto testShadowsCaster4 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster4);
    auto shadowsCasterTransform4 = SGCore::MakeRef<SGCore::TransformBase>();
    shadowsCasterTransform4->m_position.x = 20;
    shadowsCasterTransform4->m_position.y = 5;
    shadowsCasterTransform4->m_position.z = -20.0;
    shadowsCasterTransform4->m_rotation.y = -90;
    testShadowsCaster4->addComponent(shadowsCasterTransform4);
    auto directionalLight4 = SGCore::MakeRef<SGCore::PBRFRPDirectionalLight>();
    directionalLight4->m_color.r = 139.0f / 255.0f;
    directionalLight4->m_color.g = 50.0f / 255.0f;
    directionalLight4->m_color.b = 241.0f / 255.0f;
    directionalLight4->m_intensity = 200.0;
    testShadowsCaster4->addComponent(directionalLight4);
    testShadowsCaster4->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto xLineGizmo = SGCore::MakeRef<SGCore::LineGizmo>();
    xLineGizmo->m_meshData->setVertexPosition(1, 10, 0, 0);
    xLineGizmo->m_color = { 1.0, 0.0, 0.0, 1.0 };

    auto yLineGizmo = SGCore::MakeRef<SGCore::LineGizmo>();
    yLineGizmo->m_meshData->setVertexPosition(1, 0, 10, 0);
    yLineGizmo->m_color = { 0.0, 1.0, 0.0, 1.0 };

    auto zLineGizmo = SGCore::MakeRef<SGCore::LineGizmo>();
    zLineGizmo->m_meshData->setVertexPosition(1, 0, 0, 10);
    zLineGizmo->m_color = { 0.0, 0.0, 1.0, 1.0 };*/

    /*testShadowsCaster1->addComponent(xLineGizmo);
    testShadowsCaster1->addComponent(yLineGizmo);
    testShadowsCaster1->addComponent(zLineGizmo);*/
    
    /*{
        entt::entity testShadowsCaster = testScene->getECSRegistry().create();
        SGCore::Transform& testShadowsCasterTransform = testScene->getECSRegistry().emplace<SGCore::Transform>(
                testShadowsCaster);
        SGCore::DirectionalLight& testShadowsCasterDirLight = testScene->getECSRegistry().emplace<SGCore::DirectionalLight>(
                testShadowsCaster);
        testShadowsCasterDirLight.m_base.m_color = { 1.0f, 1.0f, 1.0f, 1.0f };
        testShadowsCasterDirLight.m_base.m_intensity = 1200.0f;
        testShadowsCasterTransform.m_ownTransform.m_position = { 0.0, 20.0, -20.0 };
        SGCore::RenderingBase& testShadowsCasterRenderingBase = testScene->getECSRegistry().emplace<SGCore::RenderingBase>(
                testShadowsCaster);
        SGCore::EntityBaseInfo& testShadowsCasterBaseInfo = testScene->getECSRegistry().emplace<SGCore::EntityBaseInfo>(
                testShadowsCaster);
    }
    
    {
        entt::entity testShadowsCaster = testScene->getECSRegistry().create();
        SGCore::Transform& testShadowsCasterTransform = testScene->getECSRegistry().emplace<SGCore::Transform>(
                testShadowsCaster);
        SGCore::DirectionalLight& testShadowsCasterDirLight = testScene->getECSRegistry().emplace<SGCore::DirectionalLight>(
                testShadowsCaster);
        testShadowsCasterDirLight.m_base.m_color = { 1.0f, 0.0f, 0.0f, 1.0f };
        testShadowsCasterDirLight.m_base.m_intensity = 1200.0f;
        testShadowsCasterTransform.m_ownTransform.m_position = { -20.0, 20.0, -20.0 };
        SGCore::RenderingBase& testShadowsCasterRenderingBase = testScene->getECSRegistry().emplace<SGCore::RenderingBase>(
                testShadowsCaster);
        SGCore::EntityBaseInfo& testShadowsCasterBaseInfo = testScene->getECSRegistry().emplace<SGCore::EntityBaseInfo>(
                testShadowsCaster);
    }
    
    {
        entt::entity testShadowsCaster = testScene->getECSRegistry().create();
        SGCore::Transform& testShadowsCasterTransform = testScene->getECSRegistry().emplace<SGCore::Transform>(
                testShadowsCaster);
        SGCore::DirectionalLight& testShadowsCasterDirLight = testScene->getECSRegistry().emplace<SGCore::DirectionalLight>(
                testShadowsCaster);
        SGCore::BoxGizmo& testShadowsCasterBoxGizmo = testScene->getECSRegistry().emplace<SGCore::BoxGizmo>(
                testShadowsCaster);
        testShadowsCasterBoxGizmo.m_size = { 50.0, 50.0, 50.0 };
        testShadowsCasterBoxGizmo.m_base.m_color = { 1.0, 1.0, 1.0, 1.0 };
        testShadowsCasterDirLight.m_base.m_color = { 0.0f, 1.0f, 0.0f, 1.0f };
        testShadowsCasterDirLight.m_base.m_intensity = 1200.0f;
        testShadowsCasterTransform.m_ownTransform.m_position = { -20.0, 20.0, 20.0 };
        SGCore::RenderingBase& testShadowsCasterRenderingBase = testScene->getECSRegistry().emplace<SGCore::RenderingBase>(
                testShadowsCaster);
        SGCore::EntityBaseInfo& testShadowsCasterBaseInfo = testScene->getECSRegistry().emplace<SGCore::EntityBaseInfo>(
                testShadowsCaster);
    }
    
    {
        entt::entity testShadowsCaster = testScene->getECSRegistry().create();
        SGCore::Transform& testShadowsCasterTransform = testScene->getECSRegistry().emplace<SGCore::Transform>(
                testShadowsCaster);
        SGCore::DirectionalLight& testShadowsCasterDirLight = testScene->getECSRegistry().emplace<SGCore::DirectionalLight>(
                testShadowsCaster);
        testShadowsCasterDirLight.m_base.m_color = { 0.0f, 0.0f, 1.0f, 1.0f };
        testShadowsCasterDirLight.m_base.m_intensity = 20000.0f;
        testShadowsCasterTransform.m_ownTransform.m_position = { -20.0, 100.0, -20.0 };
        SGCore::RenderingBase& testShadowsCasterRenderingBase = testScene->getECSRegistry().emplace<SGCore::RenderingBase>(
                testShadowsCaster);
        SGCore::EntityBaseInfo& testShadowsCasterBaseInfo = testScene->getECSRegistry().emplace<SGCore::EntityBaseInfo>(
                testShadowsCaster);
    }*/
    // -----------------------------------------

    // IMGUI DEBUG -----------------------------------------------------------

    SGCore::ImGuiWrap::ImGuiLayer::initImGui();

    // -----------------------------------------------------------------------
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------

int framesCnt = 0;

void fixedUpdate()
{
    double angle = framesCnt / 75.0;

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(KEY_1))
    {
        _atmosphereScattering->m_sunRotation.x += 0.5f;
    }
    else if(SGCore::InputManager::getMainInputListener()->keyboardKeyDown(KEY_2))
    {
        _atmosphereScattering->m_sunRotation.x -= 0.5f;
    }

    if(_atmosphereScattering)
    {
        // _atmosphereScattering->m_sunRotation.x += 0.01f;
    }

    // auto transform0 = testShadowsCaster->getComponent<SGCore::TransformBase>();

    // transform0->m_position.y += sin(framesCnt / 30.0) / 2.5;

    SGCore::Scene::getCurrentScene()->fixedUpdate();

    framesCnt++;
}

auto testWindow = std::make_shared<SGCore::ImGuiWrap::Window>();
auto testCollapsingHeader = std::make_shared<SGCore::ImGuiWrap::CollapsingHeader>();

void update()
{
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(KEY_F11))
    {
        std::cout << "pressed f11" << std::endl;
        SGCore::CoreMain::getWindow().setFullscreen(!SGCore::CoreMain::getWindow().isFullscreen());
    }

    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();

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

            for(const auto& system : SGCore::Scene::getCurrentScene()->getAllSystems())
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

    SGCore::Scene::getCurrentScene()->update();

    auto& viewsInjector = *SGUtils::Singleton::getSharedPtrInstance<SGCore::ImGuiWrap::ViewsInjector>();
    viewsInjector.renderViews();

    SGCore::ImGuiWrap::ImGuiLayer::endFrame();
}

// --------------------------------------------

int main()
{
    SGEditor::EditorMain::start();

    auto& viewsInjector = *SGUtils::Singleton::getSharedPtrInstance<SGCore::ImGuiWrap::ViewsInjector>();
    viewsInjector["TestWindow"].m_rootView = testWindow;
    viewsInjector["TestWindow"].m_childrenViews.push_back(testCollapsingHeader);
    //viewsInjector["Scene hierarchy"]["TestCollapsingHeader"].m_rootView = testCollapsingHeader;

    auto testWindowOverdraw = SGCore::MakeEventListener<void()> ([]()
                                                                        {
                                                                            ImGui::Text("hi.");
                                                                        });

    int* dragInt = new int(0);
    auto testCollapsingHeaderOverdraw = SGCore::MakeEventListener<void()>([dragInt]()
                                                                                  {
                                                                                      ImGui::DragInt("Drag this int!", dragInt);
                                                                                      ImGui::Text("hi!!");
                                                                                  });

    (*testWindow->m_onRenderEvent) += testWindowOverdraw;
    (*testCollapsingHeader->m_onRenderEvent) += testCollapsingHeaderOverdraw;

    //SGConsole::Console::start();

    sgSetCoreInitCallback(init);
    sgSetFixedUpdateCallback(fixedUpdate);
    sgSetUpdateCallback(update);

    SGCore::CoreMain::start();

    //SGConsole::Console::stop();

    return EXIT_SUCCESS;
}