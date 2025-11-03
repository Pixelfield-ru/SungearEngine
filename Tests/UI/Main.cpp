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
#include "SGCore/Input/PCInput.h"
#include "SGCore/UI/FontsManager.h"
#include "SGCore/UI/Elements/Text.h"

SGCore::AssetRef<SGCore::UI::CSSFile> cssFile;
SGCore::AssetRef<SGCore::UI::UIDocument> uiDocument;
SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::Scene> scene;
SGCore::Ref<SGCore::IMeshData> quadMeshData;
SGCore::MeshRenderState quadMeshRenderState;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;
SGCore::AssetRef<SGCore::ITexture2D> someTexture;

SGCore::AssetRef<SGCore::ITexture2D> testTex;

SGCore::Ref<SGCore::RenderingBase> cameraRenderingBase { };

void coreInit()
{
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::instance().createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::instance().registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::instance().setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    SGCore::Scene::setCurrentScene(scene);
    
    cssFile = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::CSSFile>("${enginePath}/Tests/UI/Resources/test.css");
    uiDocument = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::UIDocument>("${enginePath}/Tests/UI/Resources/test.xml");
    auto tmpDoc = uiDocument;
    screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");
    someTexture = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/no_material.png");

    testTex = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>("${enginePath}/Resources/textures/test.png");

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
    uiComponent.m_attachedToCamera = cameraEntity;

    // creating quad model ======================================
    quadMeshData = SGCore::Ref<SGCore::IMeshData>(SGCore::CoreMain::getRenderer()->createMeshData());

    quadMeshData->m_vertices.resize(4);

    quadMeshData->m_vertices[0] = {
        .m_position = { -1, -1, 0.0f },
        .m_uv = glm::vec3 { 0.0f, 0.0f, 0.0f }
    };

    quadMeshData->m_vertices[1] = {
        .m_position = { -1, 1, 0.0f },
        .m_uv = glm::vec3 { 0.0f, 1.0f, 0.0f }
    };

    quadMeshData->m_vertices[2] = {
        .m_position = { 1, 1, 0.0f },
        .m_uv = glm::vec3 { 1.0f, 1.0f, 0.0f }
    };

    quadMeshData->m_vertices[3] = {
        .m_position = { 1, -1, 0.0f },
        .m_uv = glm::vec3 { 1.0f, 0.0f, 0.0f }
    };

    quadMeshData->m_indices.resize(6);

    quadMeshData->m_indices[0] = 0;
    quadMeshData->m_indices[1] = 2;
    quadMeshData->m_indices[2] = 1;
    quadMeshData->m_indices[3] = 0;
    quadMeshData->m_indices[4] = 3;
    quadMeshData->m_indices[5] = 2;

    quadMeshData->prepare();

    glfwGetError(nullptr);

    glfwInit();

    glfwSetCharCallback(SGCore::CoreMain::getWindow().getNativeHandle(), [](GLFWwindow* window, unsigned int c) {
        std::cout << "char: " << c << std::endl;
        auto text = uiDocument->findElement("InputText");
        static_cast<SGCore::UI::Text*>(text.get())->m_text += c;
    });

    const char* error { };
    glfwGetError(&error);

    if(error)
    {
        std::cout << "glfw error after glfwSetCharCallback: " << error << std::endl;
    }

    SGCore::Input::PC::onKeyboardKeyEvent += [](SGCore::Window& inWindow, SGCore::Input::KeyboardKey key, int scancode, SGCore::Input::KeyState state, int mods) {
        auto elem = uiDocument->findElement("InputText");
        auto* textElem = static_cast<SGCore::UI::Text*>(elem.get());
        if(key == SGCore::Input::KeyboardKey::KEY_BACKSPACE && (state == SGCore::Input::KeyState::REPEAT || state == SGCore::Input::KeyState::PRESS) && !textElem->m_text.empty())
        {
            textElem->m_text.erase(textElem->m_text.length() - 1);
        }
        else if(key == SGCore::Input::KeyboardKey::KEY_ENTER && (state == SGCore::Input::KeyState::REPEAT || state == SGCore::Input::KeyState::PRESS))
        {
            textElem->m_text += U'\n';
        }
    };
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

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_1))
    {
        uiDocument->reloadFromDisk();
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_2))
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

    SGCore::CoreMain::init();
    SGCore::CoreMain::startCycle();

    return 0;
}
