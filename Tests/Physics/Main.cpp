//
// Created by stuka on 02.10.2024.
//

#include "Main.h"

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Scene/EntityBaseInfo.h>
#include <SGCore/Transformations/Transform.h>
#include <SGCore/Render/Camera3D.h>
#include <SGCore/Transformations/Controllable3D.h>
#include <SGCore/Render/RenderingBase.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/Memory/AssetManager.h>

// #include <SGCore/

using namespace SGCore;

// VARIABLES =================================================

Ref<ModelAsset> cubeModel;
Ref<ModelAsset> sphereModel;

Ref<Scene> testScene;

highp_entity testCameraEntity;

highp_entity planeEntity;

// ===========================================================

void coreInit()
{
    // creating PBR render pipeline and setting as current
    auto pbrrpPipeline = RenderPipelinesManager::createRenderPipeline<PBRRenderPipeline>();
    RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    RenderPipelinesManager::setCurrentRenderPipeline<PBRRenderPipeline>();

    // creating scene and setting as current
    testScene = SGCore::MakeRef<SGCore::Scene>();
    testScene->m_name = "TestScene";
    testScene->createDefaultSystems();
    Scene::addScene(testScene);
    Scene::setCurrentScene("TestScene");

    // creating camera entity
    testCameraEntity = testScene->getECSRegistry()->create();
    // adding base info to entity
    EntityBaseInfo& cameraBaseInfo = testScene->getECSRegistry()->emplace<EntityBaseInfo>(testCameraEntity);
    cameraBaseInfo.setRawName("SGMainCamera");

    auto& cameraTransform = testScene->getECSRegistry()->emplace<Ref<Transform>>(testCameraEntity, MakeRef<Transform>());

    auto& cameraEntityCamera3D = testScene->getECSRegistry()->emplace<Ref<Camera3D>>(testCameraEntity, MakeRef<Camera3D>());
    auto& cameraEntityControllable = testScene->getECSRegistry()->emplace<Controllable3D>(testCameraEntity);
    auto& cameraRenderingBase = testScene->getECSRegistry()->emplace<Ref<RenderingBase>>(testCameraEntity, MakeRef<RenderingBase>());

    // loading models ==============================================

    cubeModel = AssetManager::getInstance()->loadAsset<ModelAsset>(
            "../../Sources/SGResources/models/standard/cube.obj"
    );

    sphereModel = AssetManager::getInstance()->loadAsset<ModelAsset>(
            "../../Sources/SGResources/models/standard/sphere.obj"
    );

    // adding entities on scene ===================================
    std::vector<highp_entity> floorEntities;
    cubeModel->m_nodes[0]->addOnScene(testScene, SG_LAYER_OPAQUE_NAME,
                                      [&floorEntities](const entity_t& entity)
                                      {
                                          floorEntities.push_back(entity);
                                          //testScene->getECSRegistry()->emplace<EntityBaseInfo>(meshEntity);
                                      }
    );

    auto transform = testScene->getECSRegistry()->get<Ref<Transform>>(floorEntities[0]);

    transform->m_ownTransform.m_scale = { 1000.0f, 1.0f, 1000.0f };
}

void onUpdate(const double& dt, const double& fixedDt)
{
    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
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
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    CoreMain::start();

    return 0;
}
