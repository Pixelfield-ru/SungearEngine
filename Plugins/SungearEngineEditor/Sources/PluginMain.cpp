#include "PluginMain.h"

extern "C" __declspec(dllexport) SGCore::Ref<SGCore::IPlugin> SGPluginMain()
{
	return SGE::SungearEngineEditor::getInstance();
}

int main()
{
    return 0;
}
