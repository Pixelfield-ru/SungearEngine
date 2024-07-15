#include "GeneratedCodeEntry.h"

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Scene/Scene.h>
#include <imgui.h>
#include <imgui_stdlib.h>
#include <entt/entt.hpp>

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
    auto inspectorViewInstance = SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView();
    auto currentChosenEntity = SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->m_currentChosenEntity;
    auto& currentSceneRegistry = SGCore::Scene::getCurrentScene()->getECSRegistry();

    // TODO: MAKE PREDICATE THAT IT IS NEEDS TO BE RENDERED
    if(currentSceneRegistry->valid(currentChosenEntity))
    {
${onInspectorViewComponentsChooseRenderFunctionCode}$
    }
}

SG_NOMANGLING SG_DLEXPORT void EditorGeneratedCodeEntry()
{
    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRender += onInspectorViewRender;

    SGCore::CoreMain::onInit += onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate += fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate += update;
}

SG_NOMANGLING SG_DLEXPORT void EditorGeneratedCodeExit()
{
    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRender -= onInspectorViewRender;

    SGCore::CoreMain::onInit -= onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate -= fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate -= update;
}