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

#include <SGCore/UI/UIDocument.h>
#include <SGCore/UI/UIComponent.h>

SGCore::AssetRef<SGCore::UI::CSSFile> cssFile;
SGCore::AssetRef<SGCore::UI::UIDocument> uiDocument;
SGCore::AssetRef<SGCore::IShader> divShader;
SGCore::Ref<SGCore::Scene> scene;

void coreInit()
{
    cssFile = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::CSSFile>("${enginePath}/Tests/UI/Resources/test.css");
    uiDocument = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::UIDocument>("${enginePath}/Tests/UI/Resources/test.xml");
    divShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");

    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    SGCore::Scene::setCurrentScene(scene);

    auto ecsRegistry = scene->getECSRegistry();

    // creating camera ===========================================
    auto cameraEntity = ecsRegistry->create();

    ecsRegistry->emplace<SGCore::LayeredFrameReceiver>(cameraEntity);
    ecsRegistry->emplace<SGCore::RenderingBase>(cameraEntity, SGCore::MakeRef<SGCore::RenderingBase>());
    ecsRegistry->emplace<SGCore::Transform>(cameraEntity, SGCore::MakeRef<SGCore::Transform>());

    // creating ui ===============================================
    auto uiEntity = ecsRegistry->create();

    auto& uiComponent = ecsRegistry->emplace<SGCore::UI::UIComponent>(uiEntity);
    uiComponent.m_document = uiDocument;
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
    SGCore::CoreMain::onInit.connect<&coreInit>();
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    SGCore::CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    SGCore::CoreMain::start();

    return 0;
}
