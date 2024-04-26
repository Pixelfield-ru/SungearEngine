#include "Entry.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Audio/AudioBuffer.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/Audio/AudioListener.h"
#include "SGCore/Scene/Scene.h"

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

void coreInit()
{
    testScene2 = SGCore::MakeRef<SGCore::Scene>();
    testScene2->m_name = "TestScene";
    testScene2->createDefaultSystems();
    SGCore::Scene::addScene(testScene2);
    SGCore::Scene::setCurrentScene("TestScene");
    
    // hardcoded sgeditor load
    auto sgEditorPlugin =
            SGCore::PluginsManager::loadPlugin("SungearEngineEditor", "1.0.0", "/home/ilya/pixelfield/SungearEngine/Plugins/SungearEngineEditor", { },
                                               SGCore::PluginBuildType::PBT_RELEASE);

    /*auto sgEditorPlugin =
            SGCore::PluginsManager::loadPlugin("SungearEngineEditor", "1.0.0", "Plugins/SungearEngineEditor", { },
                                               SGCore::PluginBuildType::PBT_RELEASE);*/
    
    std::cout << "plugin: " << sgEditorPlugin << std::endl;
    
    // SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("rnd_darkwind6.wav");
    // SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("hoof_hard5.ogg");
    SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("b1.ogg");
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