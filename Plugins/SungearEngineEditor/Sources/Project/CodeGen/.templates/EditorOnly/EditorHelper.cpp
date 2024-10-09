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
#include "{{ struct.filePath }}"
## endfor

// BARRIER FOR STATIC ANALYZER (FOR HIGHLIGHTING)
void DO_NOT_USE()
{

}

/*void addChildrenEntitiesToAlreadySavedEntities(const SGCore::entity_t& parentEntity,
                                               SGCore::Scene& savableScene) noexcept
{
    auto* entityBaseInfo = savableScene.getECSRegistry()->try_get<SGCore::EntityBaseInfo>(parentEntity);
    if(entityBaseInfo)
    {
        // saving all children entities
        for(const auto& childEntity : entityBaseInfo->getChildren())
        {
            savableScene.m_alreadySavedEntities.insert(childEntity);

            addChildrenEntitiesToAlreadySavedEntities(childEntity, savableScene);
        }
    }
}*/

template<SGCore::Serde::FormatType TFormatType>
void onEntitySave(SGCore::Scene& savableScene,
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

    // TODO: FIX
    ## if struct.hasMember(name: "type")
    if(std::string("{{ struct.type }}") == "component")
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

    ## endfor
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

    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::JSON>() += onEntitySave<SGCore::Serde::FormatType::JSON>;
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
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::JSON>() -= onEntitySave<SGCore::Serde::FormatType::JSON>;
    // TODO: supporting BSON and YAML
    /*SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() -= onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() -= onEntitySave<SGCore::Serde::FormatType::YAML>;*/

    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRenderBody -= onInspectorViewRender;

    SGCore::CoreMain::onInit -= onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate -= fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate -= update;
}