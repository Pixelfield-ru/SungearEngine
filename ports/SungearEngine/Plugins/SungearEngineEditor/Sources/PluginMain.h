#ifndef SUNGEARENGINEEDITOR_PLUGINMAIN_H
#define SUNGEARENGINEEDITOR_PLUGINMAIN_H

#include <iostream>
#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/PluginsSystem/IPlugin.h>
#include <SGUtils/Singleton.h>
#include <SGCore/ImGuiWrap/ViewsInjector.h>

#include "SungearEngineEditor.h"

static SGCore::Ref<SungearEngineEditor> s_SungearEngineEditorInstance;

static auto& mainViewsInjector = *SGUtils::Singleton::getSharedPtrInstance<SGCore::ImGuiWrap::ViewsInjector>();

extern "C" SGCore::Ref<SGCore::IPlugin> SGPluginMain();

#endif // SUNGEARENGINEEDITOR_PLUGINMAIN_H
