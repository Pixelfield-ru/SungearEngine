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
#include "SGCore/Annotations/AnnotationsProcessor.h"
#include "SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/SerializersGenerator.h"

SGCore::Ref<SGCore::Scene> testScene2;

// #include "F:\Pixelfield\SungearEngine\SungearEngine\cmake-build-release\Sources\SGEntry\.generated\Serializers.h"
/*#include "/home/ilya/pixelfield/SungearEngine/cmake-build-release/Sources/SGEntry/.generated/Serializers.h"
#include "/home/ilya/pixelfield/SungearEngine/Sources/SGCore/Annotations/.references/TestStruct.h"*/
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"

struct MyStruct
{
    std::string m_name = "test";
    bool m_bool = true;
};

// ЭТАЛОН!!!!!!!!!!!!!!!!!!!!!

void SGCore::SerializerSpec<Derived>::serialize(rapidjson::Document& toDocument,
                                                rapidjson::Value& parentKey,
                                                rapidjson::Value& parent,
                                                rapidjson::Value& valueTypeName,
                                                const Derived& value) noexcept
{
    SGCore::Serializer::serialize(toDocument, parent, "b", value.b);
}

void SGCore::SerializerSpec<Derived>::deserializeDynamic(const rapidjson::Value& parent,
                                                         const rapidjson::Value& value,
                                                         const std::string& typeName,
                                                         std::string& outputLog,
                                                         Derived*& outputValue) noexcept
{
    outputValue = new Derived();

    SGCore::Serializer::deserializeUsing3rdPartyDynamicCasts<Base>(value, value, typeName, outputLog, static_cast<Base&>(*outputValue));
    outputValue->b = SGCore::Serializer::deserialize<float>(value, "b", outputLog);
}

void SGCore::SerializerSpec<Derived>::deserializeStatic(const rapidjson::Value& parent,
                                                        const rapidjson::Value& value,
                                                        const std::string& typeName,
                                                        std::string& outputLog,
                                                        Derived& outputValue) noexcept
{
    SGCore::Serializer::deserializeUsing3rdPartyDynamicCasts<Base>(value, value, typeName, outputLog, static_cast<Base&>(outputValue));
    outputValue.b = SGCore::Serializer::deserialize<float>(value, "b", outputLog);
}

// ============================================================
// ============================================================
// ============================================================

void SGCore::SerializerSpec<Base>::serialize(rapidjson::Document& toDocument,
                                             rapidjson::Value& parentKey,
                                             rapidjson::Value& parent,
                                             rapidjson::Value& valueTypeName,
                                             const Base& value) noexcept
{
    SGCore::Serializer::serialize(toDocument, parent, "a", value.a);
}

void SGCore::SerializerSpec<Base>::deserializeDynamic(const rapidjson::Value& parent, const rapidjson::Value& value,
                                                      const std::string& typeName,
                                                      std::string& outputLog,
                                                      Base*& outputValue) noexcept
{
    if(typeName == SGCore::SerializerSpec<Derived>::type_name)
    {
        outputValue = SGCore::Serializer::deserializeUsing3rdPartyDynamicCasts<Derived*>(parent, value, typeName, outputLog);
        return;
    }

    outputValue = new Base();

    outputValue->a = SGCore::Serializer::deserialize<int>(value, "a", outputLog);
}

void SGCore::SerializerSpec<Base>::deserializeStatic(const rapidjson::Value& parent,
                                                     const rapidjson::Value& value,
                                                     const std::string& typeName,
                                                     std::string& outputLog,
                                                     Base& outputValue) noexcept
{
    outputValue.a = SGCore::Serializer::deserialize<int>(value, "a", outputLog);
}

// ==================================================================================

void coreInit()
{
    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

    std::printf("init...\n");

    testScene2 = SGCore::MakeRef<SGCore::Scene>();
    testScene2->m_name = "TestScene";
    testScene2->createDefaultSystems();
    SGCore::Scene::addScene(testScene2);
    SGCore::Scene::setCurrentScene("TestScene");

    char* sgSourcesPath = std::getenv("SUNGEAR_SOURCES_ROOT");
    
    if(sgSourcesPath)
    {
        std::string sgEditorPath = std::string(sgSourcesPath) + "/Plugins/SungearEngineEditor";

        // hardcoded sgeditor load
        auto sgEditorPlugin =
                SGCore::PluginsManager::loadPlugin("SungearEngineEditor",
                                                   sgEditorPath,
                                                   {},
                                                   SGCore::PluginBuildType::PBT_DEBUG);

        std::cout << "plugin: " << sgEditorPlugin  << ", sgeditor path: " << sgEditorPath << std::endl;
    }
    else
    {
        std::cout << "CANNOT LOAD SUNGEAR EDITOR PLUGIN" << std::endl;
    }
    
    SGCore::AnnotationsProcessor annotationsProcessor;

    const char* sungearRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    std::string sungearRootStr;
    if(sungearRoot)
    {
        sungearRootStr = sungearRoot;
    }

    // annotationsProcessor.processAnnotations(std::vector<std::filesystem::path> { "/home/ilya/pixelfield/SungearEngine/Sources/SGCore/Annotations/.references/TestStruct.h" });
    annotationsProcessor.processAnnotations(sungearRootStr + "/Sources",
                                            { sungearRootStr + "/Sources/SGCore/Annotations/Annotations.h",
                                              sungearRootStr + "/Sources/SGCore/Annotations/AnnotationsProcessor.cpp",
                                              sungearRootStr + "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/SerializersGenerator.cpp"});
    
    /*SGCore::CodeGen::SerializersGenerator serializersGenerator;
    std::printf("Error of serializers generator: %s\n", serializersGenerator.generateSerializers(annotationsProcessor, "./").c_str());
    
    std::cout << annotationsProcessor.stringifyAnnotations() << std::endl;*/

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

    MyStruct testSerde;
    testSerde.m_name = "Ilya";
    testSerde.m_bool = true;

    std::unique_ptr<Base> tst = std::make_unique<Derived>();
    tst->a = -1;
    dynamic_cast<Derived*>(tst.get())->b = 4;
    Serializer::serialize(document, document, "testSerde", tst);

    // Serializer::serialize(document, document, "testSerde", annotationsProcessor);

    rapidjson::StringBuffer stringBuffer;
    stringBuffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
    document.Accept(writer);
    
    FileUtils::writeToFile("serializer_test.txt", stringBuffer.GetString(), false, true);

    rapidjson::Document fromDocument;
    fromDocument.Parse(FileUtils::readFile("serializer_test.txt").c_str());

    std::string outputLog;
    auto deser = Serializer::deserialize<std::unique_ptr<Base>>(document, "testSerde", outputLog);

    std::printf("deser: %i, %f\n", deser->a, (dynamic_cast<Derived*>(deser.get()))->b);

    std::printf("deser log: %s\n", outputLog.c_str());

    /*rapidjson::Document fromDocument;
    fromDocument.Parse(FileUtils::readFile("serializer_test.txt").c_str());

    std::string outputLog;
    auto deserializedStruct = Serializer::deserialize<AnnotationsProcessor>(document, "testSerde", outputLog);

    std::printf("file path: %s\n", Utils::toUTF8<char16_t>(deserializedStruct.getAnnotations()[0].m_filePath.u16string()).c_str());*/
    // std::printf("deserialized struct: %s, %i\n", deserializedStruct.m_name.c_str(), deserializedStruct.m_bool);
}

void onUpdate(const double& dt, const double& fixedDt)
{
    testScene2->update(dt, fixedDt);
    
    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();
    SGCore::ImGuiWrap::IView::getRoot()->render();
    SGCore::ImGuiWrap::ImGuiLayer::endFrame();

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_P) &&
        ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        if(SGCore::PluginsManager::isPluginExists("SungearEngineEditor"))
        {
            SGCore::PluginsManager::reloadPlugin("SungearEngineEditor", {}, SGCore::PluginBuildType::PBT_DEBUG);
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
                                                   SGCore::PluginBuildType::PBT_DEBUG);
            }
        }
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_O) &&
            ImGui::GetCurrentContext() && !ImGui::GetIO().WantTextInput)
    {
        std::printf("unloaded plugin\n");
        SGCore::PluginsManager::unloadPlugin("SungearEngineEditor");
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
    
    CoreMain::start();
    
    return 0;
}