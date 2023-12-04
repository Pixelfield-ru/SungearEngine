//#define SUNGEAR_DEBUG

#include <cstdlib>

#include "CoreTest.h"
#include "SGCore/Main/CoreMain.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Graphics/API/IShader.h"
#include "SGCore/Graphics/API/ShaderDefine.h"
#include "SGCore/Graphics/API/IUniformBuffer.h"
#include "SGCore/Graphics/API/IShaderUniform.h"
#include "SGCore/Memory/Assets/ModelAsset.h"

#include "SGCore/ECS/Transformations/TransformationsUpdater.h"

#include "glm/gtx/euler_angles.hpp"

#include "SGCore/Main/Callbacks.h"

#include "SGCore/ECS/Scene.h"
#include "SGCore/ECS/ECSUtils.h"

#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Rendering/Camera.h"
#include "SGCore/ECS/Rendering/Lighting/DirectionalLight.h"
#include "SGCore/ECS/Rendering/Skybox.h"
#include "SGCore/Memory/Assets/CubemapAsset.h"
#include "SGCore/ECS/Rendering/Gizmos/BoxGizmo.h"

#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/ECS/Rendering/Gizmos/LineGizmo.h"
#include "EditorMain.h"
#include "SGCore/ImGuiWrap/Views/Base/Window.h"
#include "SGCore/ImGuiWrap/Views/Base/CollapsingHeader.h"
#include "SGCore/ImGuiWrap/ViewsInjector.h"

SGCore::Ref<SGCore::ModelAsset> testModel;

SGCore::Ref<SGCore::Entity> testCameraEntity;
SGCore::Ref<SGCore::Scene> testScene;

// TODO: ALL THIS CODE WAS WRITTEN JUST FOR THE SAKE OF THE TEST. remove

SGCore::Ref<SGCore::Entity> testShadowsCaster;

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
            //"../SGResources/models/test/wooden_table/scene.gltf"
            //"../SGResources/models/test/svd/scene.gltf"
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

    sphereModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                      [](const SGCore::Ref<SGCore::Entity>& entity)
                                      {
                                          auto meshComponent = entity->getComponent<SGCore::Mesh>();
                                          auto transformComponent = entity->getComponent<SGCore::Transform>();
                                          if(transformComponent)
                                          {
                                              transformComponent->m_position = { 0, 6.0, -20 };
                                              transformComponent->m_rotation = { 0, 0, 0 };
                                              transformComponent->m_scale = { 0.5, 0.5, 0.5 };
                                          }
                                          if(meshComponent)
                                          {
                                              meshComponent->m_meshData->m_material->m_metallicFactor = 1;
                                              meshComponent->m_meshData->m_material->m_roughnessFactor = 1;

                                              meshComponent->m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTP_DIFFUSE,
                                                                                                         "../SGResources/textures/spotted_rust/spotted-rust_albedo.png");

                                              meshComponent->m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTP_LIGHTMAP,
                                                                                                         "../SGResources/textures/spotted_rust/spotted-rust_ao.png");

                                              meshComponent->m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTP_METALNESS,
                                                                                                         "../SGResources/textures/spotted_rust/spotted-rust_metallic.png");

                                              meshComponent->m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTP_NORMALS,
                                                                                                         "../SGResources/textures/spotted_rust/spotted-rust_normal-ogl.png");

                                              meshComponent->m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTP_DIFFUSE_ROUGHNESS,
                                                                                                         "../SGResources/textures/spotted_rust/spotted-rust_roughness.png");
                                          }
                                      }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    testModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                      [](const SGCore::Ref<SGCore::Entity>& entity)
                                      {
                                          auto meshComponent = entity->getComponent<SGCore::Mesh>();
                                          auto transformComponent = entity->getComponent<SGCore::Transform>();
                                          if(meshComponent)
                                          {
                                              meshComponent->m_meshDataRenderInfo.m_enableFacesCulling = false;
                                              meshComponent->m_meshData->m_material->findAndAddTexture2D(SGTextureType::SGTP_DIFFUSE, "../SGResources/textures/chess.jpg");
                                              meshComponent->m_meshData->setVertexUV(0, 200, 0, 0);
                                              meshComponent->m_meshData->setVertexUV(1, 0, 200, 0);
                                              meshComponent->m_meshData->setVertexUV(2, 200, 200, 0);
                                              meshComponent->m_meshData->prepare();
                                          }
                                          if(transformComponent)
                                          {
                                              transformComponent->m_scale = { 400.0, 400.0, 400.0 };
                                          }
                                      }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    model0->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                   [](const SGCore::Ref<SGCore::Entity>& entity)
    {
        auto transformComponent = entity->getComponent<SGCore::Transform>();
        if(transformComponent)
        {
            /*transformComponent->m_position = { 3, 4, -20 };
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
            transformComponent->m_position = { 0, 2.91, -20 };
            transformComponent->m_rotation = { 0, 0, 0 };
            transformComponent->m_scale = { 0.7, 0.7, 0.7 };

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
                                      [](const SGCore::Ref<SGCore::Entity>& entity)
                                      {
                                          auto transformComponent = entity->getComponent<SGCore::Transform>();
                                          if(transformComponent)
                                          {
                                              // wooden table
                                              transformComponent->m_position = { 0, 1.4, -20 };
                                              transformComponent->m_rotation = { 0, 90, 0 };
                                              transformComponent->m_scale = { 0.1, 0.1, 0.1 };
                                          }
                                      }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    // adding skybox
    {
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

        cubeModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                          [standardCubemap](const SGCore::Ref<SGCore::Entity>& entity)
                                          {
                                              entity->addComponent(SGCore::MakeRef<SGCore::Skybox>());

                                              auto transformComponent = entity->getComponent<SGCore::Transform>();
                                              auto meshComponent = entity->getComponent<SGCore::Mesh>();

                                              if(transformComponent)
                                              {
                                                  transformComponent->m_scale = {1000, 1000, 1000 };
                                              }

                                              if(meshComponent)
                                              {
                                                  meshComponent->m_meshDataRenderInfo.m_enableFacesCulling = false;
                                                  meshComponent->m_meshData->m_material->m_textures.emplace_back(
                                                          SGTextureType::SGTP_SKYBOX,
                                                          standardCubemap->getTexture2D()
                                                  );
                                              }
                                          }
        );
    }

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

    auto geniusJPG = SGCore::AssetManager::loadAsset<SGCore::Texture2DAsset>(
            "../SGResources/textures/genius.jpg"
    );

    cubeModel1->m_nodes[0]->addOnScene(testScene, SG_LAYER_TRANSPARENT_NAME,
                                       [geniusJPG](const SGCore::Ref<SGCore::Entity>& entity)
                                       {
                                           auto transformComponent = entity->getComponent<SGCore::Transform>();
                                           auto meshComponent = entity->getComponent<SGCore::Mesh>();

                                           if(transformComponent)
                                           {
                                               transformComponent->m_position = { -5, 3, -30 };
                                               transformComponent->m_rotation = { 0, 0, 0 };
                                               transformComponent->m_scale = {0.1 * 10.0, 0.4 * 10.0, 0.1 * 10.0 };
                                           }

                                           if(meshComponent)
                                           {
                                               meshComponent->m_meshData->m_material->m_textures.emplace_back(
                                                       SGTextureType::SGTP_DIFFUSE,
                                                       geniusJPG->m_texture2D
                                               );
                                           }
                                       }
    );

    // ==========================================================================================
    // ==========================================================================================
    // ==========================================================================================

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
    shadowsCasterTransform->m_position.y = 25;
    shadowsCasterTransform->m_position.z = 5.0;
    shadowsCasterTransform->m_position.x = -5.0;
    shadowsCasterTransform->m_rotation.x = 50;
    testShadowsCaster->addComponent(shadowsCasterTransform);
    auto directionalLight = SGCore::MakeRef<SGCore::DirectionalLight>();
    // directionalLight->m_color.r = 10.0f / 255.0f;
    // directionalLight->m_color.g = 129.0f / 255.0f;
    // directionalLight->m_color.b = 100.0f / 255.0f;
    directionalLight->m_intensity = 2000.0;
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
    testShadowsCaster1->addComponent(shadowsCasterTransform1);
    auto directionalLight1 = SGCore::MakeRef<SGCore::DirectionalLight>();
    directionalLight1->m_color.r = 139.0f / 255.0f;
    directionalLight1->m_color.g = 184.0f / 255.0f;
    directionalLight1->m_color.b = 241.0f / 255.0f;
    directionalLight1->m_intensity = 700.0;
    testShadowsCaster1->addComponent(directionalLight1);
    testShadowsCaster1->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto testShadowsCaster2 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster2);
    auto shadowsCasterTransform2 = SGCore::MakeRef<SGCore::Transform>();
    shadowsCasterTransform2->m_position.x = 10;
    shadowsCasterTransform2->m_position.y = 10;
    shadowsCasterTransform2->m_position.z = -50.0;
    shadowsCasterTransform2->m_rotation.y = 180;
    testShadowsCaster2->addComponent(shadowsCasterTransform2);
    auto directionalLight2 = SGCore::MakeRef<SGCore::DirectionalLight>();
    directionalLight2->m_color.r = 20.0f / 255.0f;
    directionalLight2->m_color.g = 184.0f / 255.0f;
    directionalLight2->m_color.b = 241.0f / 255.0f;
    directionalLight2->m_intensity = 500.0;
    testShadowsCaster2->addComponent(directionalLight2);
    testShadowsCaster2->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto testShadowsCaster3 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster3);
    auto shadowsCasterTransform3 = SGCore::MakeRef<SGCore::Transform>();
    shadowsCasterTransform3->m_position.x = -20;
    shadowsCasterTransform3->m_position.y = 5;
    shadowsCasterTransform3->m_position.z = -20.0;
    shadowsCasterTransform3->m_rotation.y = 90;
    testShadowsCaster3->addComponent(shadowsCasterTransform3);
    auto directionalLight3 = SGCore::MakeRef<SGCore::DirectionalLight>();
    directionalLight3->m_color.r = 139.0f / 255.0f;
    directionalLight3->m_color.g = 15.0f / 255.0f;
    directionalLight3->m_color.b = 5.0f / 255.0f;
    testShadowsCaster3->addComponent(directionalLight3);
    directionalLight3->m_intensity = 100.0;
    testShadowsCaster3->addComponent(SGCore::MakeRef<SGCore::BoxGizmo>());

    auto testShadowsCaster4 = SGCore::MakeRef<SGCore::Entity>();
    testScene->addEntity(testShadowsCaster4);
    auto shadowsCasterTransform4 = SGCore::MakeRef<SGCore::Transform>();
    shadowsCasterTransform4->m_position.x = 20;
    shadowsCasterTransform4->m_position.y = 5;
    shadowsCasterTransform4->m_position.z = -20.0;
    shadowsCasterTransform4->m_rotation.y = -90;
    testShadowsCaster4->addComponent(shadowsCasterTransform4);
    auto directionalLight4 = SGCore::MakeRef<SGCore::DirectionalLight>();
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
    zLineGizmo->m_color = { 0.0, 0.0, 1.0, 1.0 };

    /*testShadowsCaster1->addComponent(xLineGizmo);
    testShadowsCaster1->addComponent(yLineGizmo);
    testShadowsCaster1->addComponent(zLineGizmo);*/

    /// -----------------------------------------

    // IMGUI DEBUG -----------------------------------------------------------

    SGCore::ImGuiWrap::ImGuiLayer::initImGui();

    // -----------------------------------------------------------------------
}

// -------------- CAMERA JUST FOR FIRST STABLE VERSION. MUST BE DELETED --------

int framesCnt = 0;

void fixedUpdate()
{
    double angle = framesCnt / 75.0;

    auto transform0 = testShadowsCaster->getComponent<SGCore::Transform>();

    transform0->m_position.y += sin(framesCnt / 30.0) / 2.5;

    SGCore::Scene::getCurrentScene()->fixedUpdate();

    framesCnt++;
}

auto testWindow = std::make_shared<SGCore::ImGuiWrap::Window>();
auto testCollapsingHeader = std::make_shared<SGCore::ImGuiWrap::CollapsingHeader>();

void update()
{
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

            for(const auto& system : SGCore::Scene::getCurrentScene()->getSystems())
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

    auto& viewsInjector = *SGSingleton::getSharedPtrInstance<SGCore::ImGuiWrap::ViewsInjector>();
    viewsInjector.renderViews();

    SGCore::ImGuiWrap::ImGuiLayer::endFrame();
}

// --------------------------------------------

int main()
{
    SGEditor::EditorMain::start();

    auto& viewsInjector = *SGSingleton::getSharedPtrInstance<SGCore::ImGuiWrap::ViewsInjector>();
    viewsInjector["TestWindow"].m_rootView = testWindow;
    viewsInjector["TestWindow"].m_childrenViews.push_back(testCollapsingHeader);
    //viewsInjector["Scene hierarchy"]["TestCollapsingHeader"].m_rootView = testCollapsingHeader;

    auto* testWindowOverdraw = new SGCore::EventListenerHolder<void()> {[]()
                                                                        {
                                                                            ImGui::Text("hi.");
                                                                        }};

    int* dragInt = new int(0);
    auto* testCollapsingHeaderOverdraw = new SGCore::EventListenerHolder<void()> {[dragInt]()
                                                                                  {
                                                                                      ImGui::DragInt("Drag this int!", dragInt);
                                                                                      ImGui::Text("hi!!");
                                                                                  }};

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