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

#include <glm/vec3.hpp>
#include <imgui.h>

#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/ImGuiWrap/Views/IView.h"

// SGCore::AudioSource darkWindSrc;
SGCore::Ref<SGCore::AudioBuffer> darkWindAudioBuf = SGCore::MakeRef<SGCore::AudioBuffer>();

glm::vec3 listenerPosition { 0, 0, 0 };
glm::vec3 darkWindPos { 0, 0, 0 };

SGCore::Ref<SGCore::Scene> testScene2;

void audioStateChanged(SGCore::AudioSource& audioSource, SGCore::AudioSourceState lastState, SGCore::AudioSourceState newState)
{
    switch(newState)
    {
        case SGCore::SOURCE_PLAYING:
            std::cout << "tha play of some audio" << std::endl;
            break;
        case SGCore::SOURCE_PAUSED:
            break;
        case SGCore::SOURCE_STOPPED:
            std::cout << "tha stop of some audio" << std::endl;
            break;
    }
}

template<typename RetT>
struct get_address;

template<typename RetT, typename... Args>
struct get_address<RetT(Args...)>
{
    size_t operator()(std::function<RetT(Args...)> f)
    {
        typedef RetT (fnType)(Args...);
        fnType** fnPointer = f.template target<fnType*>();
        return (size_t) *fnPointer;
    }
};

template<typename RetT, typename T>
size_t getAddress(std::function<RetT(T &)> f) {
    typedef void (fnType)(T &);
    fnType** fnPointer = f.template target<fnType*>();
    return (size_t) *fnPointer;
}

void coreInit()
{
    testScene2 = SGCore::MakeRef<SGCore::Scene>();
    testScene2->m_name = "TestScene";
    testScene2->createDefaultSystems();
    SGCore::Scene::addScene(testScene2);
    SGCore::Scene::setCurrentScene("TestScene");

    char* sgSourcesPath = std::getenv("SUNGEAR_SOURCES_ROOT");

    system("chcp 65001");
    setlocale(LC_ALL, "Russian");

    if(sgSourcesPath)
    {
        std::string sgEditorPath = std::string(sgSourcesPath) + "/Plugins/SungearEngineEditor";

        // hardcoded sgeditor load
        auto sgEditorPlugin =
                SGCore::PluginsManager::loadPlugin("SungearEngineEditor", "1.0.0",
                                                   sgEditorPath,
                                                   {},
                                                   SGCore::PluginBuildType::PBT_RELEASE);

        std::cout << "plugin: " << sgEditorPlugin  << ", sgeditor path: " << sgEditorPath << std::endl;

        std::string err;

        SGCore::DynamicLibrary dynamicLibrary;
        dynamicLibrary.load("F:/Pixelfield/SungearEngine/SungearEngine/Plugins/SungearEngineEditor/cmake-build-release/SungearEngineEditor.dll", err);
        if(!dynamicLibrary.getNativeHandler())
        {
            std::cout << "ERROR WHILE LOADING LIBRARY: " << err << std::endl;
        }
        else
        {
            auto func = dynamicLibrary.loadFunction<SGCore::Ref<SGCore::IPlugin>()>("SGPluginMain", err);
            if(!func)
            {
                std::cout << "ERROR WHILE LOADING FUNCTION: " << err << std::endl;
            }
            else
            {
                auto plug = func();
                // std::cout << "FUNC: " << get_address<SGCore::Ref<SGCore::IPlugin>()>()(func) << std::endl;
            }
        }
    }
    else
    {
        std::cout << "CANNOT LOAD SUNGEAR EDITOR PLUGIN" << std::endl;
    }
    
    // SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("rnd_darkwind6.wav");
    // SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("hoof_hard5.ogg");
    SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::getInstance()->loadAsset<SGCore::AudioTrackAsset>("b1.ogg");
    // SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("radar_2.ogg");
    
    darkWindAudioBuf->create();
    darkWindAudioBuf->putData(darkWindAudio->getAudioTrack().getNumChannels(),
                              darkWindAudio->getAudioTrack().getBitsPerSample(),
                              darkWindAudio->getAudioTrack().getDataBuffer(),
                              darkWindAudio->getAudioTrack().getDataBufferSize(),
                              darkWindAudio->getAudioTrack().getSampleRate());
    
    auto darWindEntity = testScene2->getECSRegistry()->create();
    auto& darkWindSrc = testScene2->getECSRegistry()->emplace<SGCore::AudioSource>(darWindEntity);
    
    darkWindSrc.onStateChanged += audioStateChanged;
    
    darkWindSrc.create();
    darkWindSrc.attachBuffer(darkWindAudioBuf);
    
    darkWindSrc.setIsLooping(false);
    darkWindSrc.setState(SGCore::AudioSourceState::SOURCE_PLAYING);
    
    darkWindSrc.setPitch(1.0f);
    // darkWindSrc.setPitch(0.2f);
    darkWindSrc.setGain(1.0f);
    
    std::cout << "state: " << darkWindSrc.getState() << std::endl;
    
    std::cout << darkWindAudio->getAudioTrack().getSummary() << std::endl;
}

void onUpdate(const double& dt, const double& fixedDt)
{
    testScene2->update(dt, fixedDt);
    
    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();
    
    SGCore::ImGuiWrap::IView::getRoot()->render();
    
    SGCore::ImGuiWrap::ImGuiLayer::endFrame();
    
    // darkWindPos.z -= 0.001;
    // listenerPosition.z -= 0.001;
    // darkWindSrc.setPosition(darkWindPos);
    SGCore::AudioListener::setPosition(listenerPosition);
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_P))
    {
        auto sgEditorPlugin = SGCore::PluginsManager::reloadPlugin("SungearEngineEditor", "1.0.0", { }, SGCore::PluginBuildType::PBT_RELEASE);
        std::cout << sgEditorPlugin << std::endl;
    }
}

int main()
{
    using namespace SGCore;
    
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    
    CoreMain::start();
    
    return 0;
}