#include "PluginMain.h"

extern "C" SGCore::Ref<SGCore::IPlugin> SGPluginMain()
{
	return SGE::SungearEngineEditor::getInstance();
}

int main()
{
    return 0;
}
