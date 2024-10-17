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
void onEntitySerialize(SGCore::Serde::SerializableValueView<SGCore::SceneEntitySaveInfo, TFormatType>& entityView,
                       const SGCore::Scene& serializableScene,
                       const SGCore::entity_t& serializableEntity) noexcept
{
    if(serializableScene.getECSRegistry()->all_of<SGCore::NonSavable>(serializableEntity)) return;

    auto* entityBaseInfo = serializableScene.getECSRegistry()->try_get<SGCore::EntityBaseInfo>(serializableEntity);
    if(entityBaseInfo)
    {
        // saving all children entities
        for(const auto& childEntity : entityBaseInfo->getChildren())
        {
            LOG_I("GENERATED", "Saving CHILD entity '{}'...", std::to_underlying(childEntity));

            SGCore::SceneEntitySaveInfo childSaveInfo;
            childSaveInfo.m_serializableScene = &serializableScene;
            childSaveInfo.m_serializableEntity = childEntity;

            entityView.getValueContainer().pushBack(childSaveInfo);
        }
    }

    LOG_I("GENERATED", "Saving entity '{}'...", std::to_underlying(serializableEntity));

    // saving components of savableEntity

    ## for struct in structs
    ## if struct.hasMember(name: "type")
    ## if struct.type.equals(value: "component")
    {
        ## if struct.hasMember(name: "getFromRegistryBy")
        auto* component = serializableScene.getECSRegistry()->try_get<{{ struct.getFromRegistryBy }}>(serializableEntity);
        ## else
        auto* component = serializableScene.getECSRegistry()->try_get<{{ struct.fullName }}>(serializableEntity);
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
void onEntityDeserialize(SGCore::Serde::DeserializableValueView<SGCore::SceneEntitySaveInfo, TFormatType>& entityView,
                         SGCore::registry_t& toRegistry) noexcept
{
    std::vector<SGCore::entity_t> childrenEntities;

    auto entity = toRegistry.create();
    entityView.m_data->m_serializableEntity = entity;

    // iterating through all elements of entityView
    for(auto componentIt = entityView.getValueContainer().begin(); componentIt != entityView.getValueContainer().end(); ++componentIt)
    {
        const std::optional<SGCore::SceneEntitySaveInfo> asChild =
                entityView.getValueContainer().template getMember<SGCore::SceneEntitySaveInfo, SGCore::Serde::custom_derived_types<>>(componentIt, toRegistry);
        if(asChild)
        {
            childrenEntities.push_back(asChild->m_serializableEntity);

            continue;
        }

        ## for struct in structs
        ## if struct.hasMember(name: "type")
        ## if struct.type.equals(value: "component")
        {
            ## if struct.hasMember(name: "getFromRegistryBy")
            const auto component = entityView.getValueContainer().template getMember<{{ struct.getFromRegistryBy }}>(componentIt);
            ## else
            const auto component = entityView.getValueContainer().template getMember<{{ struct.fullName }}>(componentIt);
            ## endif

            if(component)
            {
                ## if struct.hasMember(name: "getFromRegistryBy")
                toRegistry.emplace<{{ struct.getFromRegistryBy }}>(entity, *component);
                ## else
                toRegistry.emplace<{{ struct.fullName }}>(entity, *component);
                ## endif
            }
        }
        ## endif
        ## endif
        ## endfor
    }

    SGCore::EntityBaseInfo* entityBaseInfo = toRegistry.try_get<SGCore::EntityBaseInfo>(entity);
    if(!entityBaseInfo)
    {
        return;
    }

    *entityBaseInfo = SGCore::EntityBaseInfo(entity);

    for(const auto& childEntity : childrenEntities)
    {
        entityBaseInfo->addChild(childEntity, toRegistry);
    }
}

template<SGCore::Serde::FormatType TFormatType>
void onSystemSerialize(
        SGCore::Serde::SerializableValueView<SGCore::Scene::systems_container_t, TFormatType>& systemsContainerView,
        const SGCore::Scene& savableScene,
        const SGCore::Ref<SGCore::ISystem>& savableSystem) noexcept
{
    systemsContainerView.getValueContainer().pushBack(savableSystem);
}

template<SGCore::Serde::FormatType TFormatType>
void onSystemDeserialize(
        SGCore::Serde::DeserializableValueView<SGCore::Scene::systems_container_t, TFormatType>& systemsContainerView,
        const typename SGCore::Serde::FormatInfo<TFormatType>::array_iterator_t& curIterator) noexcept
{
    const auto system = systemsContainerView.getValueContainer().template getMember<SGCore::Ref<SGCore::ISystem>>(curIterator);
    if(system)
    {
        LOG_D(SGCORE_TAG, "LOADING SYSTEM {}", std::string(typeid(**system).name()));

        systemsContainerView.m_data->emplace_back(std::move(*system));
    }
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

    SGCore::Scene::getOnEntitySerializeEvent<SGCore::Serde::FormatType::JSON>() += onEntitySerialize<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnEntityDeserializeEvent<SGCore::Serde::FormatType::JSON>() += onEntityDeserialize<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnSystemSerializeEvent<SGCore::Serde::FormatType::JSON>() += onSystemSerialize<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnSystemDeserializeEvent<SGCore::Serde::FormatType::JSON>() += onSystemDeserialize<SGCore::Serde::FormatType::JSON>;
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
    SGCore::Scene::getOnEntitySerializeEvent<SGCore::Serde::FormatType::JSON>() -= onEntitySerialize<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnEntityDeserializeEvent<SGCore::Serde::FormatType::JSON>() -= onEntityDeserialize<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnSystemSerializeEvent<SGCore::Serde::FormatType::JSON>() -= onSystemSerialize<SGCore::Serde::FormatType::JSON>;
    SGCore::Scene::getOnSystemDeserializeEvent<SGCore::Serde::FormatType::JSON>() -= onSystemDeserialize<SGCore::Serde::FormatType::JSON>;
    // TODO: supporting BSON and YAML
    /*SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::BSON>() -= onEntitySave<SGCore::Serde::FormatType::BSON>;
    SGCore::Scene::getOnEntitySave<SGCore::Serde::FormatType::YAML>() -= onEntitySave<SGCore::Serde::FormatType::YAML>;*/

    SGE::SungearEngineEditor::getInstance()->getMainView()->getInspectorView()->onRenderBody -= onInspectorViewRender;

    SGCore::CoreMain::onInit -= onInit;
    SGCore::CoreMain::getFixedTimer().onUpdate -= fixedUpdate;
    SGCore::CoreMain::getRenderTimer().onUpdate -= update;
}