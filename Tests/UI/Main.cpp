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

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Graphics/API/IFrameBufferAttachment.h"
#include "SGCore/Graphics/API/ITexture2D.h"

SGCore::AssetRef<SGCore::UI::CSSFile> cssFile;
SGCore::AssetRef<SGCore::UI::UIDocument> uiDocument;
SGCore::AssetRef<SGCore::IShader> divShader;
SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::Scene> scene;
SGCore::Ref<SGCore::IMeshData> quadMesh;
SGCore::MeshRenderState quadMeshRenderState;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;
SGCore::AssetRef<SGCore::ITexture2D> someTexture;

SGCore::Ref<SGCore::RenderingBase> cameraRenderingBase { };

void coreInit()
{
    cssFile = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::CSSFile>("${enginePath}/Tests/UI/Resources/test.css");
    uiDocument = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::UIDocument>("${enginePath}/Tests/UI/Resources/test.xml");
    divShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/ui/div.sgshader");
    screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");
    someTexture = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/no_material.png");

    uiDocument->m_rootElement->m_shader = divShader;

    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    SGCore::Scene::setCurrentScene(scene);

    auto ecsRegistry = scene->getECSRegistry();

    // creating camera ===========================================
    auto cameraEntity = ecsRegistry->create();

    auto& cameraReceiver = ecsRegistry->emplace<SGCore::LayeredFrameReceiver>(cameraEntity);
    cameraRenderingBase = ecsRegistry->emplace<SGCore::RenderingBase>(cameraEntity, SGCore::MakeRef<SGCore::RenderingBase>());
    ecsRegistry->emplace<SGCore::Transform>(cameraEntity, SGCore::MakeRef<SGCore::Transform>());

    attachmentToDisplay = cameraReceiver.m_layersFXFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_COLOR_ATTACHMENT7);

    // creating ui ===============================================
    auto uiEntity = ecsRegistry->create();

    auto& uiComponent = ecsRegistry->emplace<SGCore::UI::UIComponent>(uiEntity);
    uiComponent.m_document = uiDocument;

    // creating quad model ======================================
    quadMesh = SGCore::Ref<SGCore::IMeshData>(SGCore::CoreMain::getRenderer()->createMeshData());

    quadMesh->m_vertices.resize(4);

    /*quadMesh->m_vertices[0] = {
        .m_position = { 0, 0, -1.0f }
    };

    quadMesh->m_vertices[1] = {
        .m_position = { 0, 1, -1.0f }
    };

    quadMesh->m_vertices[2] = {
        .m_position = { 1, 1, -1.0f }
    };

    quadMesh->m_vertices[3] = {
        .m_position = { 1, 0, -1.0f }
    };*/

    quadMesh->m_vertices[0] = {
        .m_position = { -1, -1, 0.0f }
    };

    quadMesh->m_vertices[1] = {
        .m_position = { -1, 1, 0.0f }
    };

    quadMesh->m_vertices[2] = {
        .m_position = { 1, 1, 0.0f }
    };

    quadMesh->m_vertices[3] = {
        .m_position = { 1, -1, 0.0f }
    };

    quadMesh->m_indices.resize(6);

    quadMesh->m_indices[0] = 0;
    quadMesh->m_indices[1] = 2;
    quadMesh->m_indices[2] = 1;
    quadMesh->m_indices[3] = 0;
    quadMesh->m_indices[4] = 3;
    quadMesh->m_indices[5] = 2;
    /*quadMesh->m_indices[0] = 0;
    quadMesh->m_indices[1] = 1;

    quadMesh->m_indices[2] = 1;
    quadMesh->m_indices[3] = 2;

    quadMesh->m_indices[4] = 2;
    quadMesh->m_indices[5] = 0;

    quadMesh->m_indices[6] = 0;
    quadMesh->m_indices[7] = 3;

    quadMesh->m_indices[8] = 3;
    quadMesh->m_indices[9] = 2;*/

    quadMesh->prepare();

    // quadMeshRenderState.m_useFacesCulling = false;
}

void onUpdate(const double& dt, const double& fixedDt)
{
    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
    }

    int windowSizeX = 0;
    int windowSizeY = 0;

    SGCore::CoreMain::getWindow().getSize(windowSizeX, windowSizeY);

    cameraRenderingBase->m_left = -windowSizeX / 2.0f;
    cameraRenderingBase->m_right = windowSizeX / 2.0f;
    cameraRenderingBase->m_top = windowSizeY / 2.0f;
    cameraRenderingBase->m_bottom = -windowSizeY / 2.0f;

    screenShader->bind();

    attachmentToDisplay->bind(0);
    screenShader->useTextureBlock("u_bufferToDisplay", 0);

    SGCore::CoreMain::getRenderer()->renderMeshData(
        quadMesh.get(),
        quadMeshRenderState
    );
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
