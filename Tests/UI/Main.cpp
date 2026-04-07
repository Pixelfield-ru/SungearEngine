#include "Main.h"

#include "SGCore/Main/CoreMain.h"
#include <SGCore/UI/UIElement.h>

#include "SGCore/Graphics/API/IFrameBuffer.h"
#include "SGCore/Input/PCInput.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/UI/UIDocument.h"
#include "SGCore/UI/Parser/SIIML/Node/Node.h"
#include "SGCore/UI/UIComponent.h"
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>

std::string testString = "<div>Test text <div>Other text</div></div>";
std::string simlTestString = R"(
div {
    "Test text";
    div {
        "other text";
    }
})";

SGCore::AssetRef<SGCore::UI::UIDocument> uiDocument;
SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::Scene> scene;
SGCore::Ref<SGCore::IMeshData> quadMeshData;
SGCore::MeshRenderState quadMeshRenderState;

SGCore::Ref<SGCore::ITexture2D> attachmentToDisplay;
SGCore::AssetRef<SGCore::ITexture2D> someTexture;

SGCore::AssetRef<SGCore::ITexture2D> testTex;

SGCore::Ref<SGCore::RenderingBase> cameraRenderingBase { };

using Container = std::unique_ptr<struct SGCore::UI::Style,struct std::default_delete<struct
 SGCore::UI::Style> >;

static_assert(SGCore::UI::Deserialization::IsValidContainer<Container>::value
        && (!SGCore::UI::Deserialization::ImplDeserializableContainer<Container>)
        && SGCore::UI::Deserialization::ImplDeserializable<typename SGCore::UI::Deserialization::IsValidContainer<Container>::ValueType_t>);

using Test = SGCore::UI::Deserialization::Deserializer<Container>::_is_shared_container_specialization;

void coreInit()
{
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::instance().createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::instance().registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::instance().setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    scene = SGCore::MakeRef<SGCore::Scene>();
    scene->createDefaultSystems();

    SGCore::Scene::setCurrentScene(scene);

    // UI DOCUMENT
    uiDocument = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::UIDocument>("${enginePath}/Tests/UI/Resources/test.siml");
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

    screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");

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
    });

    const char* error { };
    glfwGetError(&error);

    if(error)
    {
        std::cout << "glfw error after glfwSetCharCallback: " << error << std::endl;
    }
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
