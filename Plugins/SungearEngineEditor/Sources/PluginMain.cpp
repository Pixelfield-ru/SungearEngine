#include "PluginMain.h"

extern "C" SGCore::Ref<SGCore::IPlugin> SGPluginMain()
{
	s_SungearEngineEditorInstance = SGCore::MakeRef<SungearEngineEditor>();

	return s_SungearEngineEditorInstance;
}
