#include "Entry.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Audio/AudioBuffer.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Audio/AudioDevice.h"

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

SGCore::AudioSource darkWindSrc;
SGCore::Ref<SGCore::AudioBuffer> darkWindAudioBuf = SGCore::MakeRef<SGCore::AudioBuffer>();

void coreInit()
{
    // hardcoded sgeditor load
    /*auto sgEditorPlugin =
            SGCore::PluginsManager::loadPlugin("SungearEngineEditor", "1.0.0", "/home/ilya/pixelfield/SungearEngine/Plugins/SungearEngineEditor", { },
                                               SGCore::PluginBuildType::PBT_RELEASE);*/

    auto sgEditorPlugin =
            SGCore::PluginsManager::loadPlugin("SungearEngineEditor", "1.0.0", "F:/Pixelfield/SungearEngine/Plugins/SungearEngineEditor", { },
                                               SGCore::PluginBuildType::PBT_RELEASE);
    
    std::cout << sgEditorPlugin << std::endl;
    
    SGCore::AudioDevice::getDefaultDevice()->makeCurrent();
    
    SGCore::Ref<SGCore::AudioTrackAsset> darkWindAudio = SGCore::AssetManager::loadAsset<SGCore::AudioTrackAsset>("rnd_darkwind6.wav");
    
    darkWindAudioBuf->create();
    darkWindAudioBuf->putData(darkWindAudio->getAudioTrack().getAudioFormat(),
                              darkWindAudio->getAudioTrack().getDataBuffer(),
                              darkWindAudio->getAudioTrack().getDataBufferSize(),
                              darkWindAudio->getAudioTrack().getSampleRate());
    darkWindAudioBuf->setBitsPerSample(darkWindAudio->getAudioTrack().getBitsPerSample());
    darkWindAudioBuf->setNumChannels(darkWindAudio->getAudioTrack().getNumChannels());
    
    darkWindSrc.attachBuffer(darkWindAudioBuf);
    
    darkWindSrc.setIsLooping(true);
    darkWindSrc.setState(SGCore::AudioSourceState::SOURCE_PLAYING);
    
    std::cout << darkWindAudio->getAudioTrack().getSummary() << std::endl;
}

void onUpdate(const double& dt, const double& fixedDt)
{
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