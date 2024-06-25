#ifndef SUNGEARENGINEEDITOR_PLUGINMAIN_H
#define SUNGEARENGINEEDITOR_PLUGINMAIN_H

#include <iostream>
#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/PluginsSystem/IPlugin.h>
#include "SGCore/Utils/Singleton.h"

#include "SungearEngineEditor.h"

SG_NOMANGLING SG_DLEXPORT SGCore::Ref<SGCore::IPlugin> SGPluginMain();

int main();

#endif // SUNGEARENGINEEDITOR_PLUGINMAIN_H
