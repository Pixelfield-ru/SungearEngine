#include "GeneratedCodeEntry.h"

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Serde/Components/NonSavable.h>
#include <SGCore/Logger/Logger.h>
#include <.SG_GENERATED/Serializers.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <entt/entt.hpp>

#include <Plugins/SungearEngineEditor/Sources/Views/InspectorView.h>
#include <Plugins/SungearEngineEditor/Sources/SungearEngineEditor.h>

${includes}$

template<SGCore::Serde::FormatType TFormatType>
void onEntitySave(const SGCore::Scene& savableScene,
                  const SGCore::entity_t& savableEntity,
                  SGCore::Serde::SerializableValueView<SGCore::registry_t, TFormatType>& registryView) noexcept
{
    if(savableScene.getECSRegistry()->all_of<SGCore::NonSavable>(savableEntity)) return;

    LOG_I("GENERATED", "Saving entity '{}'...", std::to_underlying(savableEntity));

${componentsSave}$
}

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
    auto currentSceneRegistry = SGCore::Scene::getCurrentScene()->getECSRegistry();

    // TODO: MAKE PREDICATE THAT IT IS NEEDS TO BE RENDERED
    if(currentSceneRegistry->valid(currentChosenEntity))
    {
${onInspectorViewComponentsChooseRenderFunctionCode}$
    }
}

SG_NOMANGLING SG_DLEXPORT void editorGeneratedCodeEntry()
{
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::JSON>() += onEntitySave<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() += onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() += onEntitySave<SGCore::Serde::FormatType::YAML>;

    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRenderBody += onInspectorViewRender;

    SGCore::CoreMain::onInit += onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate += fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate += update;
}

SG_NOMANGLING SG_DLEXPORT void editorGeneratedCodeExit()
{
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::JSON>() -= onEntitySave<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() -= onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() -= onEntitySave<SGCore::Serde::FormatType::YAML>;

    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRenderBody -= onInspectorViewRender;

    SGCore::CoreMain::onInit -= onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate -= fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate -= update;
}