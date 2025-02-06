#include "Entry.h"
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


/*class MyCSSListener : public css3ParserBaseListener {
public:
    void enterDeclaration(css3Parser::DeclarationContext* ctx) override {
        std::cout << "Property: " << ctx->property()->getText()
                  << ", Value: " << ctx->expr()->getText() << std::endl;
    }
};*/

void coreInit()
{
    // auto cssFile = SGCore::AssetManager::getInstance()->loadAsset<SGCore::UI::CSSFile>("window.css");

    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

    /*SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() += onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() += onEntitySave<SGCore::Serde::FormatType::YAML>;*/

    // std::printf("init...\n");

    // fixme: FOR TEST
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    LOG_E(SGCORE_TAG, "Русский язык");

    const SGCore::InterpolatedPath sgEditorPath = "${enginePath}/Plugins/SungearEngineEditor";

    // hardcoded sgeditor load
    auto sgEditorPlugin =
            SGCore::PluginsManager::loadPlugin("SungearEngineEditor",
                                               sgEditorPath.resolved(),
                                               {},
                                               "cmake-build-debug");

    // std::cout << "plugin: " << sgEditorPlugin << ", sgeditor path: " << sgEditorPath << std::endl;

    const char* sungearRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    std::string sungearRootStr;
    if(sungearRoot)
    {
        sungearRootStr = sungearRoot;
    }

    SGCore::CodeGen::Generator generator;
    std::string generated = generator.generate(sungearRootStr + "/Sources/SGCore/CodeGeneration/SerdeGeneration/.templates/SerdeSpec.h");
    SGCore::FileUtils::writeToFile("generated.h", generated, false, true);

    using namespace SGCore;
    
    rapidjson::Document document;
    document.SetObject();
    
    /*TestNamespace::MyStruct<std::string> testStruct;
    
    Serializer::serialize(document, document, "testTransform", testStruct);*/
    
    /*Transform testTransform;
    Mesh mesh;
    RenderingBase renderingBase;

    Ref<AudioTrackAsset> audio = AssetManager::getInstance()->loadAsset<SGCore::AudioTrackAsset>("b2.ogg");
    audio->m_serializationType = SerializationType::SERIALIZE_DATA;

    Serializer::serialize(document, document, "test", audio);*/

    /*SGCore::Ref<SGCore::AudioTrackAsset> audio =
            SGCore::AssetManager::getInstance()->loadAsset<SGCore::AudioTrackAsset>("b2.ogg");
    SGCore::Ref<SGCore::AudioBuffer> audioBuf =
            SGCore::MakeRef<SGCore::AudioBuffer>();
    audioBuf->create();

    audioBuf->putData(audio->getAudioTrack().getNumChannels(),
                      audio->getAudioTrack().getBitsPerSample(),
                      audio->getAudioTrack().getDataBuffer(),
                      audio->getAudioTrack().getDataBufferSize(),
                      audio->getAudioTrack().getSampleRate());*/

    /*AudioSource audioSource;
    audioSource.attachAudioTrack(audioBuf);

    Serializer::serialize(document, document, "test", audioSource);*/

    std::shared_ptr<Base> tst = std::make_shared<Derived0>();
    // Base* tst = new Derived0();
    tst->a = -1;
    /*dynamic_cast<Derived0*>(tst.get())->b = 20.1f;
    dynamic_cast<Derived0*>(tst.get())->str1 = u"abra";
    dynamic_cast<Derived0*>(tst.get())->myVec3 = { -3, 5, 1 };
    dynamic_cast<Derived0*>(tst.get())->unMap["v0"] = 20.0f;*/
    /*dynamic_cast<Derived0*>(tst)->b = 20.1f;
    dynamic_cast<Derived0*>(tst)->str1 = u"abra";*/
    //dynamic_cast<Derived*>(tst)->b = 4;
    const int i = 3;
    FileUtils::writeToFile("serializer_test.txt", Serde::Serializer::toFormat(tst), false, true);

    // Serializer::serialize(document, document, "testSerde", annotationsProcessor);

    std::string outputLog;

    std::shared_ptr<Base> deser;
    // Serde::Serializer::fromFormat<Serde::custom_derived_types<Derived0>>(FileUtils::readFile("serializer_test.txt"), deser, Serde::FormatType::JSON, outputLog, 1);
    Serde::Serializer::fromFormat(FileUtils::readFile("serializer_test.txt"), deser, outputLog);

    using t0 = types_container<int, float>;
    using t1 = types_container<float, double>;

    using cat = reverse_types_container_t<types_container_cat_t<t0, t1>>;

    std::cout << "cat: " << typeid(cat).name() << std::endl;

    // auto uiDocument = AssetManager::getInstance()->loadAsset<UI::UIDocument>("test.xml");

    //

    // auto deser = Serde::Serializer::deserialize<std::unique_ptr<Base>>(document, "testSerde", outputLog);

    // std::printf("deser: %i, %f\n", deser->a, 0.0f);

    /*rapidjson::Document fromDocument;
    fromDocument.Parse(FileUtils::readFile("serializer_test.txt").c_str());

    std::string outputLog;
    auto deserializedStruct = Serializer::deserialize<AnnotationsProcessor>(document, "testSerde", outputLog);

    std::printf("file path: %s\n", Utils::toUTF8<char16_t>(deserializedStruct.getAnnotations()[0].m_filePath.u16string()).c_str());*/
    // std::printf("deserialized struct: %s, %i\n", deserializedStruct.m_name.c_str(), deserializedStruct.m_bool);
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
        std::printf("unloaded plugin\n");
        SGCore::PluginsManager::unloadAllPlugins();
    }

    if(SGCore::Scene::getCurrentScene())
    {
        SGCore::Scene::getCurrentScene()->update(dt, fixedDt);
    }
    
    /*try
    {
        void* ptr = nullptr;
        *((int*) ptr) = 5;
    }
    catch(std::exception e)
    {
        std::cout << e.what() << std::endl;
    }*/
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