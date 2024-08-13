#include "PluginMain.h"

SG_NOMANGLING SG_DLEXPORT SGCore::Ref<SGCore::IPlugin> SGPluginMain()
{
    return ${pluginName}$::getInstance();
}

