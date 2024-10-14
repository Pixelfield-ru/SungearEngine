#include <set>

#include "EditorHelper.h"
#include "../Serde.h"

#include <SGCore/Main/CoreMain.h>
#include <SGCore/Scene/Scene.h>
#include <SGCore/Serde/Components/NonSavable.h>
#include <SGCore/Logger/Logger.h>
#include <SGCore/Scene/EntityBaseInfo.h>

#include <imgui.h>
#include <imgui_stdlib.h>
#include <entt/entt.hpp>

#include <Plugins/SungearEngineEditor/Sources/Views/InspectorView.h>
#include <Plugins/SungearEngineEditor/Sources/SungearEngineEditor.h>

// including all structs
## for struct in structs
## if struct.hasMember(name: "filePath")
#include "{{ struct.filePath }}"
## endif
## endfor

// BARRIER FOR STATIC ANALYZER (FOR HIGHLIGHTING)
void DO_NOT_USE()
{

}

template<SGCore::Serde::FormatType TFormatType>
void onEntitySave(const SGCore::Scene& savableScene,
                  const SGCore::entity_t& savableEntity,
                  SGCore::Serde::SerializableValueView<SGCore::SceneEntitySaveInfo, TFormatType>& entityView) noexcept
{
    if(savableScene.getECSRegistry()->all_of<SGCore::NonSavable>(savableEntity)) return;

    auto* entityBaseInfo = savableScene.getECSRegistry()->try_get<SGCore::EntityBaseInfo>(savableEntity);
    if(entityBaseInfo)
    {
        // saving all children entities
        for(const auto& childEntity : entityBaseInfo->getChildren())
        {
            LOG_I("GENERATED", "Saving CHILD entity '{}'...", std::to_underlying(childEntity));

            SGCore::SceneEntitySaveInfo childSaveInfo;
            childSaveInfo.m_savableScene = &savableScene;
            childSaveInfo.m_savableEntity = childEntity;

            entityView.getValueContainer().pushBack(childSaveInfo);
        }
    }

    LOG_I("GENERATED", "Saving entity '{}'...", std::to_underlying(savableEntity));

    // saving components of savableEntity

    ## for struct in structs
    ## if struct.hasMember(name: "type")
    ## if struct.type.equals(value: "component")
    {
        ## if struct.hasMember(name: "getFromRegistryBy")
        auto* component = savableScene.getECSRegistry()->try_get<{{ struct.getFromRegistryBy }}>(savableEntity);
        ## else
        auto* component = savableScene.getECSRegistry()->try_get<{{ struct.fullName }}>(savableEntity);
        ## endif

        if(component)
        {
            entityView.getValueContainer().pushBack(*component);
        }
    }
    ## endif
    ## endif
    ## endfor
}

template<SGCore::Serde::FormatType TFormatType>
void onSystemSave(const SGCore::Scene& savableScene,
                  const SGCore::Ref<SGCore::ISystem>& savableSystem,
                  SGCore::Serde::SerializableValueView<SGCore::Scene::systems_container_t, TFormatType>& systemsContainerView) noexcept
{
    systemsContainerView.getValueContainer().pushBack(savableSystem);
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

    }
}

SG_NOMANGLING SG_DLEXPORT void editorGeneratedCodeEntry()
{
    LOG_I("GENERATED", "Calling editorGeneratedCodeEntry()...");

    SGCore::Scene::getOnEntitySaveEvent<SGCore::Serde::FormatType::JSON>() += onEntitySave<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnSystemSaveEvent<SGCore::Serde::FormatType::JSON>() += onSystemSave<SGCore::Serde::FormatType::JSON>;
    // TODO: supporting BSON and YAML
    /*SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() += onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() += onEntitySave<SGCore::Serde::FormatType::YAML>;*/

    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRenderBody += onInspectorViewRender;

    SGCore::CoreMain::onInit += onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate += fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate += update;
}

SG_NOMANGLING SG_DLEXPORT void editorGeneratedCodeExit()
{
    SGCore::Scene::getOnEntitySaveEvent<SGCore::Serde::FormatType::JSON>() -= onEntitySave<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnSystemSaveEvent<SGCore::Serde::FormatType::JSON>() -= onSystemSave<SGCore::Serde::FormatType::JSON>;
    // TODO: supporting BSON and YAML
    /*SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() -= onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() -= onEntitySave<SGCore::Serde::FormatType::YAML>;*/

    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRenderBody -= onInspectorViewRender;

    SGCore::CoreMain::onInit -= onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate -= fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate -= update;
}