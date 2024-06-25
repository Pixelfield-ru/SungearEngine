#include "PluginMain.h"
#include <SGCore/CrashHandler/HwExceptionHandler.h>

SG_NOMANGLING SG_DLEXPORT SGCore::Ref<SGCore::IPlugin> SGPluginMain()
{
	return SGE::SungearEngineEditor::getInstance();
}

int main()
{
    return 0;
}
