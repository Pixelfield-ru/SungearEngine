//
// Created by stuka on 06.02.2025.
//

#include "Main.h"

#include <stb_image_write.h>
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
#include "SGCore/Input/InputManager.h"
#include "SGCore/UI/FontsManager.h"

SGCore::AssetRef<SGCore::UI::CSSFile> cssFile;
SGCore::AssetRef<SGCore::UI::UIDocument> uiDocument;
SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::Scene> scene;
SGCore::Ref<SGCore::IMeshData> quadMeshData;
SGCore::MeshRenderState quadMeshRenderState;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;
SGCore::AssetRef<SGCore::ITexture2D> someTexture;

SGCore::AssetRef<SGCore::ITexture2D> testTex;
SGCore::SDFTexture sdfTest;

SGCore::Ref<SGCore::RenderingBase> cameraRenderingBase { };

void coreInit()
{
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    SGCore::Scene::setCurrentScene(scene);
    
    cssFile = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::CSSFile>("${enginePath}/Tests/UI/Resources/test.css");
    uiDocument = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::UIDocument>("${enginePath}/Tests/UI/Resources/test.xml");
    screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");
    someTexture = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/no_material.png");

    testTex = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test.png");
    // sdfTest.generate(testTex->getData().get(), testTex->getWidth(), testTex->getHeight(), 20.0f);

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
    SGCore::CoreMain::getWindow().setTitle("UI Test. FPS: " + std::to_string(SGCore::CoreMain::getFPS()));

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
    cameraRenderingBase->m_zNear = -100;
    cameraRenderingBase->m_zFar = 100;

    screenShader->bind();

    // auto spec = SGCore::UI::FontsManager::getInstance().getAssetManager()->getAssetsWithType<SGCore::UI::Font>()[0]->getSpecialization({ .m_height = 52, .m_name = "default" });

    attachmentToDisplay->bind(0);
    screenShader->useTextureBlock("u_bufferToDisplay", 0);
    
    // use this to flip screen output
    screenShader->useInteger("u_flipOutput", true);

    SGCore::CoreMain::getRenderer()->renderArray(
        quadMeshData->getVertexArray(),
        quadMeshRenderState,
        quadMeshData->m_vertices.size(),
        quadMeshData->m_indices.size()
    );

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_1))
    {
        uiDocument->reloadFromDisk();
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_2))
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
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
