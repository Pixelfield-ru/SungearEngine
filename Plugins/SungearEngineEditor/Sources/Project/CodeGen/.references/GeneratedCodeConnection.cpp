#include "GeneratedCodeConnection.h"

#include <SGCore/Main/CoreMain.h>

${includes}$

void onInit()
{

}

void update(const double& dt, const double& fixedDt)
{

}

void fixedUpdate(const double& dt, const double& fixedDt)
{

}

void onInspectorViewRender()
{
    ${onInspectorViewRenderFunctionCode}$
}

SG_NOMANGLING SG_DLEXPORT void EditorGeneratedCodeMain()
{
    SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRender += onInspectorViewRender;

    SGCore::CoreMain::onInit += onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate += fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate += update;
}