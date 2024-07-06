#include "Entry.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Audio/AudioBuffer.h"
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

#include "F:\Pixelfield\SungearEngine\SungearEngine\cmake-build-release\Sources\SGEntry\.generated\Serializers.h"
/*#include "/home/ilya/pixelfield/SungearEngine/cmake-build-release/Sources/SGEntry/.generated/Serializers.h"
#include "/home/ilya/pixelfield/SungearEngine/Sources/SGCore/Annotations/.references/TestStruct.h"*/
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"

void coreInit()
{
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
                                                   SGCore::PluginBuildType::PBT_RELEASE);

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
    
    SGCore::CodeGen::SerializersGenerator serializersGenerator;
    std::printf("Error of serializers generator: %s\n", serializersGenerator.generateSerializers(annotationsProcessor, "./").c_str());
    
    std::cout << annotationsProcessor.stringifyAnnotations() << std::endl;

    std::shared_ptr<int> d;
    d.operator*();

    using namespace SGCore;
    
    rapidjson::Document document;
    document.SetObject();
    
    /*TestNamespace::MyStruct<std::string> testStruct;
    
    Serializer::serialize(document, document, "testTransform", testStruct);*/
    
    Transform testTransform;
    Mesh mesh;
    RenderingBase renderingBase;
    
    // Serializer::serialize(document, document, "testTransform", testTransform);

    std::shared_ptr<int> sharedInt = std::make_shared<int>(3);

    Serializer::serialize(document, document, "testTransform", sharedInt);
    
    rapidjson::StringBuffer stringBuffer;
    stringBuffer.Clear();
    rapidjson::PrettyWriter<rapidjson::StringBuffer> writer(stringBuffer);
    document.Accept(writer);
    
    FileUtils::writeToFile("serializer_test.txt", stringBuffer.GetString(), false, true);
}

void onUpdate(const double& dt, const double& fixedDt)
{
    testScene2->update(dt, fixedDt);
    
    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();
    SGCore::ImGuiWrap::IView::getRoot()->render();
    SGCore::ImGuiWrap::ImGuiLayer::endFrame();
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_P))
    {
        if(SGCore::PluginsManager::isPluginExists("SungearEngineEditor"))
        {
            SGCore::PluginsManager::reloadPlugin("SungearEngineEditor", {}, SGCore::PluginBuildType::PBT_RELEASE);
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
                                                   SGCore::PluginBuildType::PBT_RELEASE);
            }
        }
    }

    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_O))
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