#ifndef ${upperPluginName}$_PLUGINMAIN_H
#define ${upperPluginName}$_PLUGINMAIN_H

#include <iostream>
#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/PluginsSystem/IPlugin.h>
#include "SGCore/Utils/Singleton.h"

#include "${pluginName}$.h"

SG_NOMANGLING SG_DLEXPORT SGCore::Ref<SGCore::IPlugin> SGPluginMain();

#endif // ${upperPluginName}$_PLUGINMAIN_H
