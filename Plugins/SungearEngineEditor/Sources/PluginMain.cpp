#include "PluginMain.h"
#include <SGUtils/CrashHandler/HwExceptionHandler.h>

SG_NOMANGLING SG_DLEXPORT SGCore::Ref<SGCore::IPlugin> SGPluginMain()
{
	return SGE::SungearEngineEditor::getInstance();
}

int main()
{
    return 0;
}
