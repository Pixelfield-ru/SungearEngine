#include "Entry.h"

#include <stb_image_write.h>

#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/Audio/AudioListener.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/RenderingBase.h"

#include <SGCore/Input/InputManager.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/UI/UIDocument.h>

#include "SGCore/Memory/Assets/Atlas/Atlas.h"
#include "SGCore/Render/Batching/Batch.h"

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


#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/ImGuiWrap/Views/IView.h"
#include "SGCore/Graphics/API/ITexture2D.h"
#include "SGCore/Utils/Formatter.h"
#include "SGCore/CodeGeneration/CodeGenerator.h"

#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"

#include "SGCore/UI/CSS/CSSFile.h"

#include "SGCore/Main/Config.h"


/*class MyCSSListener : public css3ParserBaseListener {
public:
    void enterDeclaration(css3Parser::DeclarationContext* ctx) override {
        std::cout << "Property: " << ctx->property()->getText()
                  << ", Value: " << ctx->expr()->getText() << std::endl;
    }
};*/

#include <BulletCollision/btBulletCollisionCommon.h>

// SGCore::Atlas testAtlas;
const SGCore::Atlas* testAtlas { };
std::vector<SGCore::AssetRef<SGCore::ITexture2D>> testTextures;

SGCore::AssetRef<SGCore::IShader> screenShader;
SGCore::Ref<SGCore::IMeshData> m_screenQuadMeshData;
SGCore::MeshRenderState m_screenQuadMeshRenderState;

void coreInit()
{
    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

    screenShader = SGCore::AssetManager::getInstance()->loadAsset<SGCore::IShader>("${enginePath}/Resources/sg_shaders/features/screen.sgshader");

    const std::filesystem::path configPath = "SungearEngineConfig.json";
    SGCore::Config loadedConfig;
    std::string configLoadLog;
    SGCore::Serde::Serializer::fromFormat(
        SGCore::FileUtils::readFile(configPath),
        loadedConfig, SGCore::Serde::FormatType::JSON, configLoadLog);

    std::vector<SGCore::InterpolatedPath> testTexturesPaths {
        "${enginePath}/Resources/textures/no_material.png",
        "${enginePath}/Resources/textures/test.png",
        "${enginePath}/Resources/textures/test.png"
    };

    {
        std::filesystem::path p = "D:\\xampp\\phpMyAdmin\\themes\\pmahomme\\img\\designer";
        for (const auto & entry : std::filesystem::directory_iterator(p))
            testTexturesPaths.emplace_back(entry.path());
    }

    {
        std::filesystem::path p = "D:\\Wireshark\\Wireshark User's Guide\\wsug_graphics\\toolbar";
        for (const auto & entry : std::filesystem::directory_iterator(p))
            testTexturesPaths.emplace_back(entry.path());
    }

    /*{
        std::filesystem::path p = "D:\\Pixelfield\\NativeSungearEngine\\NativeSungearEngine\\Projects\\sample_shooter\\Resources\\loc0\\textures";
        for (const auto & entry : std::filesystem::directory_iterator(p))
            testTexturesPaths.emplace_back(entry.path());
    }*/

    /*for(const auto& testTexturePath : testTexturesPaths)
    {
        const auto texture = SGCore::AssetManager::getInstance()->loadAsset<SGCore::ITexture2D>(testTexturePath);
        testTextures.push_back(texture);
    }

    std::sort(testTextures.begin(), testTextures.end(),
         [](const SGCore::AssetRef<SGCore::ITexture2D>& a, const SGCore::AssetRef<SGCore::ITexture2D>& b) {
             return std::max(a->getWidth(), a->getHeight()) > std::max(b->getWidth(), b->getHeight());
         });

    for(const auto& texture : testTextures)
    {
        SGCore::AtlasRect rect;
        testAtlas.findBestRect({ texture->getWidth(), texture->getHeight() }, rect);
        testAtlas.packTexture(rect, texture.get());
    }*/

    /*for(const auto& texture : testTextures)
    {
        SGCore::AtlasRect rect;
        testAtlas.findBestRect({ texture->getWidth(), texture->getHeight() }, rect);
        testAtlas.packTexture(rect, texture.get());
    }

    {
        SGCore::AtlasRect rect;
        testAtlas.findBestRect({ testTextures[0]->getWidth(), testTextures[0]->getHeight() }, rect);
        testAtlas.packTexture(rect, testTextures[0].get());
    }

    {
        SGCore::AtlasRect rect;
        testAtlas.findBestRect({ testTextures[1]->getWidth(), testTextures[1]->getHeight() }, rect);
        testAtlas.packTexture(rect, testTextures[1].get());
    }*/

    // testAtlas.getTexture()->resizeDataBuffer(6000, 6000);

    /*testTextures[0]->resizeDataBuffer(2000, 2000);
    testTextures[0]->bind(0);
    testTextures[0]->subTextureData(testTextures[1]->getData().get(), testTextures[1]->getWidth(), testTextures[1]->getHeight(), 1000, 0);*/

    if(!configLoadLog.empty())
    {
        LOG_E(SGCORE_TAG,
              "Can not load config by path: '{}'.\nError: {}",
              SGCore::Utils::toUTF8(configPath.u16string()),
              configLoadLog);
    }
    else
    {
        for(const auto& loadablePluginConfig : loadedConfig.m_loadablePlugins)
        {
            if(!loadablePluginConfig.m_enabled) continue;

            SGCore::PluginsManager::loadPlugin(loadablePluginConfig.m_pluginName,
                                               loadablePluginConfig.m_pluginPath.resolved(),
                                               loadablePluginConfig.m_pluginEntryArgs,
                                               loadablePluginConfig.m_pluginCMakeBuildDir);
        }
    }

    const auto batchView = SGCore::Scene::getCurrentScene()->getECSRegistry()->view<SGCore::Batch>();
    batchView.each([](const SGCore::Batch& batch) {
        testAtlas = &batch.getAtlas();
    });

    {
        const size_t pixelsCnt = testAtlas->getTexture()->getWidth() * testAtlas->getTexture()->getHeight();

        SGCore::Scope<std::uint8_t[]> pixels8 = SGCore::MakeScope<std::uint8_t[]>(pixelsCnt * 4);

        for(size_t p = 0; p < pixelsCnt; ++p)
        {
            const float r = *reinterpret_cast<float*>(&testAtlas->getTexture()->getData()[p * 16]);
            const float g = *reinterpret_cast<float*>(&testAtlas->getTexture()->getData()[p * 16 + 4]);
            const float b = *reinterpret_cast<float*>(&testAtlas->getTexture()->getData()[p * 16 + 8]);
            const float a = *reinterpret_cast<float*>(&testAtlas->getTexture()->getData()[p * 16 + 12]);

            pixels8[p] = static_cast<std::uint8_t>(r * 255);
            pixels8[p + 1] = static_cast<std::uint8_t>(g * 255);
            pixels8[p + 2] = static_cast<std::uint8_t>(b * 255);
            pixels8[p + 3] = static_cast<std::uint8_t>(a * 255);
        }

        stbi_write_png("test_atlas.png", testAtlas->getTexture()->getWidth(), testAtlas->getTexture()->getHeight(), 4, pixels8.get(), testAtlas->getTexture()->getWidth() * 4);
    }

    m_screenQuadMeshData = SGCore::Ref<SGCore::IMeshData>(SGCore::CoreMain::getRenderer()->createMeshData());

    m_screenQuadMeshData->m_vertices.resize(4);

    m_screenQuadMeshData->m_vertices[0] = {
        .m_position = glm::vec3 { -1, -1, 0.0f }
    };

    m_screenQuadMeshData->m_vertices[1] = {
        .m_position = glm::vec3 { -1, 1, 0.0f }
    };

    m_screenQuadMeshData->m_vertices[2] = {
        .m_position = glm::vec3 { 1, 1, 0.0f }
    };

    m_screenQuadMeshData->m_vertices[3] = {
        .m_position = glm::vec3 { 1, -1, 0.0f }
    };

    m_screenQuadMeshData->m_indices.resize(6);

    m_screenQuadMeshData->m_indices[0] = 0;
    m_screenQuadMeshData->m_indices[1] = 2;
    m_screenQuadMeshData->m_indices[2] = 1;
    m_screenQuadMeshData->m_indices[3] = 0;
    m_screenQuadMeshData->m_indices[4] = 3;
    m_screenQuadMeshData->m_indices[5] = 2;

    m_screenQuadMeshData->prepare();

    /*SGCore::CodeGen::Generator generator;
    const std::string generated = generator.generate(SGCore::InterpolatedPath("${enginePath}/Sources/SGCore/CodeGeneration/SerdeGeneration/.templates/SerdeSpec.h").resolved());
    SGCore::FileUtils::writeToFile("generated.h", generated, false, true);*/
}

void onFixedUpdate(const double& dt, const double& fixedDt)
{
    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->fixedUpdate(dt, fixedDt);
    }
}

void onUpdate(const double& dt, const double& fixedDt)
{
    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();
    SGCore::ImGuiWrap::IView::getRoot()->render();
    SGCore::ImGuiWrap::ImGuiLayer::endFrame();

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_P) &&
        ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        if(SGCore::PluginsManager::isPluginExists("SungearEngineEditor"))
        {
            SGCore::PluginsManager::reloadPlugin("SungearEngineEditor", {}, "cmake-build-debug");
        }
        else
        {
            char* sgSourcesPath = std::getenv("SUNGEAR_SOURCES_ROOT");

            if(sgSourcesPath)
            {
                std::string sgEditorPath = std::string(sgSourcesPath) + "/Plugins/SungearEngineEditor";
                SGCore::PluginsManager::loadPlugin("SungearEngineEditor",
                                                   sgEditorPath,
                                                   {},
                                                   "cmake-build-debug");
            }
        }
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_O) &&
            ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        SGCore::PluginsManager::unloadAllPlugins();
    }

    /*screenShader->bind();

    // use this to flip screen output
    screenShader->useInteger("u_flipOutput", false);

    // someTexture->bind(0);
    if(testAtlas->getTexture())
    {
        testAtlas->getTexture()->bind(0);
    }*/

    /*if(testTextures[0])
    {
        testTextures[0]->bind(0);
    }*/

    /*screenShader->useTextureBlock("u_bufferToDisplay", 0);

    SGCore::CoreMain::getRenderer()->renderArray(
        m_screenQuadMeshData->getVertexArray(),
        m_screenQuadMeshRenderState,
        m_screenQuadMeshData->m_vertices.size(),
        m_screenQuadMeshData->m_indices.size()
    );*/

    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
    }
}

int main()
{
    using namespace SGCore;
    //std::printf("definitions: %i\n", register_definition<SGCore::SerializerSpec<std::int8_t>>::counter);
    
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    CoreMain::getFixedTimer().onUpdate.connect<&onFixedUpdate>();

    CoreMain::start();
    
    return 0;
}