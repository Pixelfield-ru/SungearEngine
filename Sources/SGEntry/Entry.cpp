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

#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Memory/Assets/ModelAsset.h>
#include <SGCore/UI/UIDocument.h>

#include "SGCore/Memory/Assets/Atlas/Atlas.h"
#include "SGCore/Render/Batching/Batch.h"
#include "SGCore/Render/ShadowMapping/CSM/CSMTarget.h"

#if SG_PLATFORM_OS_WINDOWS
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

SGCore::Ref<SGCore::ITexture2D> depthAttachment;
SGCore::Ref<SGCore::ITexture2D> cameraDepthAttachment;

void coreInit()
{
    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

    std::unique_ptr<Base> d = std::make_unique<Derived0>();
    d->a = 10;
    dynamic_cast<Derived0*>(d.get())->b = 0.14f;
    dynamic_cast<Derived0*>(d.get())->str0 = "edited";

    const std::string output = SGCore::Serde::Serializer::toFormat(SGCore::Serde::FormatType::JSON, d);
    SGCore::FileUtils::writeToFile("serializer_test.txt", output, false, false);

    const std::string output0 = SGCore::Serde::Serializer::toFormat(SGCore::Serde::FormatType::JSON, std::vector { 1, 2, 3 });
    SGCore::FileUtils::writeToFile("serializer_test1.txt", output0, false, false);

    const std::string output1 = SGCore::Serde::Serializer::toFormat(SGCore::Serde::FormatType::JSON, std::string("hello eto vashe kek"));
    SGCore::FileUtils::writeToFile("serializer_test2.txt", output1, false, false);

    const int a = 1;
    SGCore::Ref<SGCore::AABB<float>> t = SGCore::MakeRef<MyTest0>();
    const std::string output2 = SGCore::Serde::Serializer::toFormat(SGCore::Serde::FormatType::JSON, t, a);
    SGCore::FileUtils::writeToFile("serializer_test3.txt", output2, false, false);
    std::cout << "new a: " << a << std::endl;

    std::string testDeserLog;
    std::unique_ptr<Base> dd;
    SGCore::Serde::Serializer::fromFormat(
        SGCore::FileUtils::readFile("serializer_test.txt"),
        dd, SGCore::Serde::FormatType::JSON, testDeserLog);

    std::string testDeserLog0;
    std::vector<int> dd0;
    SGCore::Serde::Serializer::fromFormat(
        SGCore::FileUtils::readFile("serializer_test1.txt"),
        dd0, SGCore::Serde::FormatType::JSON, testDeserLog0);

    std::string testDeserLog1;
    std::string dd1;
    SGCore::Serde::Serializer::fromFormat(
        SGCore::FileUtils::readFile("serializer_test2.txt"),
        dd1, SGCore::Serde::FormatType::JSON, testDeserLog1);

    std::string testDeserLog2;
    SGCore::Ref<SGCore::AABB<float>> dd2;
    SGCore::Serde::Serializer::fromFormat(
        SGCore::FileUtils::readFile("serializer_test3.txt"),
        dd2, SGCore::Serde::FormatType::JSON, testDeserLog2);

    const std::filesystem::path configPath = "SungearEngineConfig.json";
    SGCore::Config loadedConfig;
    std::string configLoadLog;
    SGCore::Serde::Serializer::fromFormat(
        SGCore::FileUtils::readFile(configPath),
        loadedConfig, SGCore::Serde::FormatType::JSON, configLoadLog);

    if(!configLoadLog.empty())
    {
        LOG_E(SGCORE_TAG,
              "Can not load config by path: '{}'.\nError: {}",
              SGCore::Utils::toUTF8(configPath.u16string()),
              configLoadLog);
    }

    for(const auto& loadablePluginConfig : loadedConfig.m_loadablePlugins)
    {
        if(!loadablePluginConfig.m_isLoadable) continue;

        const auto pluginWrap = SGCore::PluginsManager::loadPlugin(loadablePluginConfig.m_pluginName,
                                           loadablePluginConfig.m_pluginPath.resolved(),
                                           loadablePluginConfig.m_pluginEntryArgs,
                                           loadablePluginConfig.m_pluginCMakeBuildDir);

        pluginWrap->getPlugin()->m_isActive = loadablePluginConfig.m_isActive;
    }

    if(SGCore::Scene::getCurrentScene())
    {
        const auto batchView = SGCore::Scene::getCurrentScene()->getECSRegistry()->view<SGCore::Batch>();
        batchView.each([](const SGCore::Batch& batch) {
            testAtlas = &batch.getAtlas();
        });
    }

    if(SGCore::Scene::getCurrentScene())
    {
        const auto camerasView = SGCore::Scene::getCurrentScene()->getECSRegistry()->view<SGCore::CSMTarget, SGCore::LayeredFrameReceiver>();
        camerasView.each([](const SGCore::CSMTarget& csm, const SGCore::LayeredFrameReceiver::reg_t& layeredFrameReceiver) {
            // depthAttachment = csm.getCascades()[0].m_frameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0);
            cameraDepthAttachment = layeredFrameReceiver.m_layersFrameBuffer->getAttachment(SGFrameBufferAttachmentType::SGG_DEPTH_ATTACHMENT0);
            return;
        });
    }

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

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_P) &&
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

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_O) &&
            ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        SGCore::PluginsManager::unloadAllPlugins();
    }

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

    CoreMain::init();
    CoreMain::startCycle();
    
    return 0;
}
