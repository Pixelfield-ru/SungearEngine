#include "Entry.h"

#include <SGCore/Input/InputManager.h>
#include <SGCore/PluginsSystem/PluginsManager.h>

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