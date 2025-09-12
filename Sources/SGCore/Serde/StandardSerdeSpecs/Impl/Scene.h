//
// Created by stuka on 31.08.2025.
//

#pragma once

#include "SGCore/Serde/Serde.h"

#include "SGCore/ECS/Registry.h"
#include "SGCore/Motion/MotionPlanner.h"
#include "SGCore/Physics/Rigidbody3D.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Render/Mesh.h"
#include "SGCore/Render/RenderingBase.h"
#include "SGCore/Render/UICamera.h"
#include "SGCore/Render/Alpha/OpaqueEntityTag.h"
#include "SGCore/Render/Alpha/TransparentEntityTag.h"
#include "SGCore/Render/Atmosphere/Atmosphere.h"
#include "SGCore/Render/Gizmos/BoxGizmo.h"
#include "SGCore/Render/Gizmos/LineGizmo.h"
#include "SGCore/Render/Gizmos/SphereGizmo.h"
#include "SGCore/Render/Lighting/DirectionalLight.h"
#include "SGCore/Render/Picking/Pickable.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Serde/Components/NonSavable.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Render/RenderingBasesUpdater.h"
#include "SGCore/Render/Atmosphere/AtmosphereUpdater.h"
#include "SGCore/Render/Gizmos/BoxGizmosRenderer.h"
#include "SGCore/Render/Gizmos/LineGizmosRenderer.h"
#include "SGCore/Render/Gizmos/SphereGizmosUpdater.h"
#include "SGCore/Render/Lighting/DirectionalLightsUpdater.h"
#include "SGCore/Render/SpacePartitioning/OctreesSolver.h"

namespace SGCore::Serde
{
    // ======================================================== impl EntityRef

    template<FormatType TFormatType>
    void SerdeSpec<EntityRef, TFormatType>::serialize(SerializableValueView<const EntityRef, TFormatType>& valueView,
                                                      const ECS::entity_t& deserializedEntity,
                                                      ECS::registry_t& toRegistry) noexcept
    {
        // if referenced entity is non savable then we do not serialize reference to referenced entity
        if(toRegistry.anyOf<NonSavable>(*valueView.m_data->m_referencedEntity))
        {
            valueView.discard();
            return;
        }

        valueView.container().addMember("m_referencedEntity", valueView.m_data->m_referencedEntity);
    }

    template<FormatType TFormatType>
    void SerdeSpec<EntityRef, TFormatType>::deserialize(DeserializableValueView<EntityRef, TFormatType>& valueView,
                                                        const ECS::entity_t& deserializedEntity,
                                                        ECS::registry_t& toRegistry) noexcept
    {
        auto m_referencedEntity = valueView.container().template getMember<decltype(valueView.m_data->m_referencedEntity)>("m_referencedEntity");

        if(m_referencedEntity)
        {
            valueView.m_data->m_referencedEntity = std::move(*m_referencedEntity);

            // IT IS MUST BE GUARANTEED THAT ENTITY BASE INFO OF deserializedEntity IS ALREADY EXIST!!!
            auto* entityBaseInfo = toRegistry.tryGet<EntityBaseInfo>(deserializedEntity);
            SG_ASSERT(entityBaseInfo != nullptr,
                      fmt::format("Can not mark EntityRef (points to entity '{}') as needing to resolve! "
                                  "Entity '{}' that contains component that contains this EntityRef does not have EntityBaseInfo!",
                                  std::to_underlying(*valueView.m_data->m_referencedEntity),
                                  std::to_underlying(deserializedEntity)).c_str());

            entityBaseInfo->m_entitiesRefsToResolve.push_back(valueView.m_data->m_referencedEntity);
        }
    }

    // ======================================================== impl Layer

    template<FormatType TFormatType>
    void SerdeSpec<Layer, TFormatType>::serialize(SerializableValueView<const Layer, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_isOpaque", valueView.m_data->m_isOpaque);
        valueView.container().addMember("m_name", valueView.m_data->m_name);
        valueView.container().addMember("m_index", valueView.m_data->m_index);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Layer, TFormatType>::deserialize(DeserializableValueView<Layer, TFormatType>& valueView) noexcept
    {
        const auto m_isOpaque = valueView.container().template getMember<decltype(valueView.m_data->m_isOpaque)>("m_isOpaque");
        if(m_isOpaque)
        {
            valueView.m_data->m_isOpaque = *m_isOpaque;
        }

        const auto m_name = valueView.container().template getMember<decltype(valueView.m_data->m_name)>("m_name");
        if(m_name)
        {
            valueView.m_data->m_name = *m_name;
        }

        const auto m_index = valueView.container().template getMember<decltype(valueView.m_data->m_index)>("m_index");
        if(m_index)
        {
            valueView.m_data->m_index = *m_index;
        }
    }

    // ======================================================== impl ECS::entity_t

    template<FormatType TFormatType>
    void SerdeSpec<ECS::entity_t, TFormatType>::serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView) noexcept
    {
        valueView.container().setAsInt64(std::to_underlying(*valueView.m_data));
    }

    template<FormatType TFormatType>
    void SerdeSpec<ECS::entity_t, TFormatType>::deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView) noexcept
    {
        *valueView.m_data = static_cast<ECS::entity_t>(valueView.container().getAsInt64());
    }

    template<FormatType TFormatType>
    void SerdeSpec<ECS::entity_t, TFormatType>::serialize(SerializableValueView<const ECS::entity_t, TFormatType>& valueView, const Scene& serializableScene) noexcept
    {
        auto& serializableEntity = *valueView.m_data;

        // valueView.container().setAsArray();
        if(serializableScene.getECSRegistry()->allOf<NonSavable>(serializableEntity))
        {
            valueView.discard();
            return;
        }

        auto& entityBaseInfo = serializableScene.getECSRegistry()->get<EntityBaseInfo>(serializableEntity);


        // trying to save standard components ===============================================
        // ==================================================================================

        #pragma region Components

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<EntityBaseInfo>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<EntityBaseInfo, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<AudioSource>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<AudioSource, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Transform>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<Transform, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<AABB<float>>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<AABB<>, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<RenderingBase>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<RenderingBase, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Camera3D>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<Camera3D, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<SphereGizmo>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<SphereGizmo, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<BoxGizmo>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<BoxGizmo, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<LineGizmo>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<LineGizmo, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Atmosphere>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<Atmosphere, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Mesh>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<Mesh, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<DirectionalLight>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<DirectionalLight, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Controllable3D>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<Controllable3D, TFormatType>::type_name(), *component);
            }
        }
        {
            auto* component = serializableScene.getECSRegistry()->tryGet<UICamera>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<UICamera, TFormatType>::type_name(), *component);
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<TransparentEntityTag>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<TransparentEntityTag, TFormatType>::type_name(), *component);
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<OpaqueEntityTag>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<OpaqueEntityTag, TFormatType>::type_name(), *component);
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Pickable>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(
                    SerdeSpec<Pickable, TFormatType>::type_name(), *component,
                    serializableEntity, *serializableScene.getECSRegistry());
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<MotionPlanner>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(
                    SerdeSpec<MotionPlanner, TFormatType>::type_name(), *component,
                    serializableEntity, *serializableScene.getECSRegistry());
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<Rigidbody3D>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(
                    SerdeSpec<Rigidbody3D, TFormatType>::type_name(), *component,
                    serializableEntity, *serializableScene.getECSRegistry());
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<IKJoint>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<IKJoint, TFormatType>::type_name(), *component);
            }
        }

        {
            auto* component = serializableScene.getECSRegistry()->tryGet<IKRootJoint>(serializableEntity);

            if(component)
            {
                valueView.container().addMember(SerdeSpec<IKRootJoint, TFormatType>::type_name(), *component);
            }
        }

        #pragma endregion Components

        // ==================================================================================
        // ==================================================================================

        // calling event to serialize user-provided components
        Scene::getOnEntitySerializeEvent<TFormatType>()(
                valueView,
                serializableScene
        );

        // saving children
        valueView.container().addMember("children", entityBaseInfo.getChildren(), serializableScene);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ECS::entity_t, TFormatType>::deserialize(DeserializableValueView<ECS::entity_t, TFormatType>& valueView, Scene& deserializableScene) noexcept
    {
        std::vector<ECS::entity_t> childrenEntities;

        auto& toRegistry = *deserializableScene.getECSRegistry();

        // creating entity
        auto entity = toRegistry.create();
        // storing created entity in value view to allow adding components to this entity in event subscriber`s functions
        *valueView.m_data = entity;

        // getting EntityBaseInfo of current entity to add all children entities
        auto& entityBaseInfo = toRegistry.get<EntityBaseInfo>(entity);

        LOG_I(SGCORE_TAG, "Loading entity: {}", std::to_underlying(*valueView.m_data));

        // trying to load standard components ===============================================
        // ==================================================================================

        #pragma region Components

        if(valueView.container().hasMember(SerdeSpec<EntityBaseInfo, TFormatType>::type_name()))
        {
            auto component = valueView.container().template getMember<EntityBaseInfo::reg_t>(SerdeSpec<EntityBaseInfo, TFormatType>::type_name());

            if(component)
            {
                entityBaseInfo = std::move(*component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<AudioSource, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<AudioSource::reg_t>(SerdeSpec<AudioSource, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<AudioSource>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Transform, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Transform::reg_t>(SerdeSpec<Transform, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<Transform>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<AABB<float>, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<AABB<float>::reg_t>(SerdeSpec<AABB<float>, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<AABB<float>>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<RenderingBase, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<RenderingBase::reg_t>(SerdeSpec<RenderingBase, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<RenderingBase>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Camera3D, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Camera3D::reg_t>(SerdeSpec<Camera3D, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<Camera3D>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<SphereGizmo, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<SphereGizmo::reg_t>(SerdeSpec<SphereGizmo, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<SphereGizmo>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<BoxGizmo, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<BoxGizmo::reg_t>(SerdeSpec<BoxGizmo, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<BoxGizmo>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<LineGizmo, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<LineGizmo::reg_t>(SerdeSpec<LineGizmo, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<LineGizmo>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Atmosphere, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Atmosphere::reg_t>(SerdeSpec<Atmosphere, TFormatType>::type_name());

            if(component)
            {
                LOG_D(SGCORE_TAG, "Atmosphere component deserializing");
                toRegistry.emplace<Atmosphere>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Mesh, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Mesh::reg_t>(SerdeSpec<Mesh, TFormatType>::type_name());

            if(component)
            {
                LOG_D(SGCORE_TAG, "Mesh component deserializing");
                toRegistry.emplace<Mesh>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<DirectionalLight, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<DirectionalLight::reg_t>(SerdeSpec<DirectionalLight, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<DirectionalLight>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Controllable3D, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Controllable3D::reg_t>(SerdeSpec<Controllable3D, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<Controllable3D>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<UICamera, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<UICamera::reg_t>(SerdeSpec<UICamera, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<UICamera>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Pickable, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Pickable::reg_t>(SerdeSpec<Pickable, TFormatType>::type_name(), entity, toRegistry);

            if(component)
            {
                toRegistry.emplace<Pickable>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<TransparentEntityTag, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<TransparentEntityTag::reg_t>(SerdeSpec<TransparentEntityTag, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<TransparentEntityTag>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<OpaqueEntityTag, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<OpaqueEntityTag::reg_t>(SerdeSpec<OpaqueEntityTag, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<OpaqueEntityTag>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<MotionPlanner, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<MotionPlanner::reg_t>(SerdeSpec<MotionPlanner, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<MotionPlanner>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<Rigidbody3D, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<Rigidbody3D::reg_t>(SerdeSpec<Rigidbody3D, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<Rigidbody3D>(entity, *component)->setParentWorld(deserializableScene.getSystem<PhysicsWorld3D>());
            }
        }

        if(valueView.container().hasMember(SerdeSpec<IKJoint, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<IKJoint::reg_t>(SerdeSpec<IKJoint, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<IKJoint>(entity, *component);
            }
        }

        if(valueView.container().hasMember(SerdeSpec<IKRootJoint, TFormatType>::type_name()))
        {
            const auto component = valueView.container().template getMember<IKRootJoint::reg_t>(SerdeSpec<IKRootJoint, TFormatType>::type_name());

            if(component)
            {
                toRegistry.emplace<IKRootJoint>(entity, *component);
            }
        }

        #pragma endregion Components

        // ==================================================================================
        // ==================================================================================

        // calling event to deserialize user-provided components
        Scene::getOnEntityDeserializeEvent<TFormatType>()(
                valueView,
                deserializableScene
        );

        // creating EntityBaseInfo component with 'entity' as identifier in arguments
        entityBaseInfo.setThisEntity(entity);

        // loading children
        const auto children = valueView.container().template getMember<decltype(EntityBaseInfo::m_children)>("children", deserializableScene);
        if(children)
        {
            for(auto child : *children)
            {
                LOG_I(SGCORE_TAG, "Adding child entity '{}' to parent entity '{}'...", std::to_underlying(child), std::to_underlying(entity));
                entityBaseInfo.addChild(child, toRegistry);
            }
        }
    }

    // ======================================================== impl ECS::registry_t

    template<FormatType TFormatType>
    void SerdeSpec<ECS::registry_t, TFormatType>::serialize(SerializableValueView<const ECS::registry_t, TFormatType>& valueView, const Scene& serializableScene) noexcept
    {
        valueView.container().setAsArray();

        auto entitiesView = valueView.m_data->template view<EntityBaseInfo>();
        for(const auto& entity : entitiesView)
        {
            LOG_I(SGCORE_TAG, "Trying to save entity '{}'", std::to_underlying(entity))

            // if entity has component 'NonSavable' then skipping this entity
            if(serializableScene.getECSRegistry()->anyOf<NonSavable>(entity)) continue;

            // if current savable entity has parent then
            // skip saving this entity because parent saves children entities himself
            auto* entityBaseInfo = serializableScene.getECSRegistry()->tryGet<EntityBaseInfo>(entity);
            if(entityBaseInfo &&
               entityBaseInfo->getParent() != entt::null &&
               serializableScene.getECSRegistry()->valid(entityBaseInfo->getParent()))
            {
                continue;
            }

            LOG_I(SGCORE_TAG, "Saving ROOT entity '{}'", std::to_underlying(entity))
            valueView.container().pushBack(entity, serializableScene);
        }

        Scene::getOnSceneSavedEvent()(serializableScene);
    }

    template<FormatType TFormatType>
    void SerdeSpec<ECS::registry_t, TFormatType>::deserialize(DeserializableValueView<ECS::registry_t, TFormatType>& valueView, Scene& serializableScene) noexcept
    {
        LOG_I(SGCORE_TAG, "Loading registry...")

        for(auto entityIt = valueView.container().begin(); entityIt != valueView.container().end(); ++entityIt)
        {
            LOG_I(SGCORE_TAG, "Trying to deserialize entity...")
            // deserializing entity and passing registry to getMember to put entity in scene
            valueView.container().template getMember<ECS::entity_t>(entityIt, serializableScene);
        }

        // resolving all EntityRef`s after deserialization ================

        serializableScene.resolveAllEntitiesRefs();

        // =================================================================
    }

    // ======================================================== impl Scene::systems_container_t

    template<FormatType TFormatType>
    void SerdeSpec<Scene::systems_container_t, TFormatType>::serialize(SerializableValueView<const Scene::systems_container_t, TFormatType>& valueView, const Scene& serializableScene) noexcept
    {
        valueView.container().setAsArray();

        // serializing systems
        for(const Ref<ISystem>& system : *valueView.m_data)
        {
            auto systemPtr = system.get();

            // if system is instance of standard systems then serializing system using standard SerdeSpecs
            // TODO: THIS IS SO SHITTY. REMOVE WHEN CUSTOM DERIVED TYPES IN EXTERNAL PROJECTS WILL BE AVAILABLE IN SGCore::Serde
            if(SG_INSTANCEOF(systemPtr, Controllables3DUpdater) ||
               SG_INSTANCEOF(systemPtr, PhysicsWorld3D) ||
               SG_INSTANCEOF(systemPtr, RenderingBasesUpdater) ||
               SG_INSTANCEOF(systemPtr, AtmosphereUpdater) ||
               SG_INSTANCEOF(systemPtr, DirectionalLightsUpdater) ||
               SG_INSTANCEOF(systemPtr, TransformationsUpdater) ||
               SG_INSTANCEOF(systemPtr, BoxGizmosRenderer) ||
               SG_INSTANCEOF(systemPtr, LineGizmosRenderer) ||
               SG_INSTANCEOF(systemPtr, SphereGizmosUpdater) ||
               SG_INSTANCEOF(systemPtr, OctreesSolver) ||
               SG_INSTANCEOF(systemPtr, AudioProcessor) ||
               SG_INSTANCEOF(systemPtr, MotionPlannersResolver) ||
               SG_INSTANCEOF(systemPtr, IKResolver))
            {
                valueView.container().pushBack(system);
                continue;
            }

            // else if it is not standard system then
            Scene::getOnSystemSerializeEvent<TFormatType>()(valueView, serializableScene, system);
        }
    }

    template<FormatType TFormatType>
    void SerdeSpec<Scene::systems_container_t, TFormatType>::deserialize(DeserializableValueView<Scene::systems_container_t, TFormatType>& valueView) noexcept
    {
        // type of auto is equals to Serde::FormatInfo<TFormatType>::array_iterator_t
        for(auto systemsIt = valueView.container().begin(); systemsIt != valueView.container().end(); ++systemsIt)
        {
            // trying to deserialize current scene using standard SerdeSpecs
            const auto system = valueView.container().template getMember<Ref<ISystem>>(systemsIt);
            if(system)
            {
                LOG_D(SGCORE_TAG, "LOADING SYSTEM {}", std::string(typeid(**system).name()));

                valueView.m_data->emplace_back(std::move(*system));
            }
            else // else if we can not deserialize system using standard SerdeSpecs (that means that current system is not standard (user-provided))
            {    // then calling event to try to deserialize current system
                Scene::getOnSystemDeserializeEvent<TFormatType>()(valueView, systemsIt);
            }
        }
    }

    // ======================================================== impl SceneMetaInfo

    template<FormatType TFormatType>
    void SerdeSpec<SceneMetaInfo, TFormatType>::serialize(SerializableValueView<const SceneMetaInfo, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_sceneName", valueView.m_data->m_sceneName);
        valueView.container().addMember("m_sceneLocalPath", valueView.m_data->m_sceneLocalPath);
    }

    template<FormatType TFormatType>
    void SerdeSpec<SceneMetaInfo, TFormatType>::deserialize(DeserializableValueView<SceneMetaInfo, TFormatType>& valueView) noexcept
    {
        auto sceneName = valueView.container().template getMember<std::string>("m_sceneName");
        if(sceneName)
        {
            valueView.m_data->m_sceneName = std::move(*sceneName);
        }

        auto sceneLocalPath = valueView.container().template getMember<std::filesystem::path>("m_sceneLocalPath");
        if(sceneLocalPath)
        {
            valueView.m_data->m_sceneLocalPath = std::move(*sceneLocalPath);
        }
    }

    // ======================================================== impl Scene

    template<FormatType TFormatType>
    void SerdeSpec<Scene, TFormatType>::serialize(SerializableValueView<const Scene, TFormatType>& valueView) noexcept
    {
        valueView.container().addMember("m_metaInfo", valueView.m_data->m_metaInfo);
        valueView.container().addMember("m_systems", valueView.m_data->m_systems, *valueView.m_data);
        valueView.container().addMember("m_ecsRegistry", *valueView.m_data->m_ecsRegistry, *valueView.m_data);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Scene, TFormatType>::deserialize(DeserializableValueView<Scene, TFormatType>& valueView) noexcept
    {
        auto metaInfo = valueView.container().template getMember<SceneMetaInfo>("m_metaInfo");
        if(metaInfo)
        {
            valueView.m_data->m_metaInfo = std::move(*metaInfo);
        }

        auto systems = valueView.container().template getMember<Scene::systems_container_t>("m_systems");
        if(systems)
        {
            for(const auto& system : *systems)
            {
                valueView.m_data->addSystem(system);
            }
        }

        // we are not interested in result of getMember function. deserialize function of registry_t writes data directly in registry of this scene
        valueView.container().template getMember<ECS::registry_t>("m_ecsRegistry", *valueView.m_data);
    }

    // ======================================================== impl Node

    template<FormatType TFormatType>
    void SerdeSpec<Node, TFormatType>::serialize(SerializableValueView<const Node, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        valueView.container().addMember("m_name", valueView.m_data->m_name);
        valueView.container().addMember("m_children", valueView.m_data->m_children, assetsPackage);
        valueView.container().addMember("m_meshesData", valueView.m_data->m_meshesData, assetsPackage);
        valueView.container().addMember("m_position", valueView.m_data->m_position);
        valueView.container().addMember("m_rotationQuaternion", valueView.m_data->m_rotationQuaternion);
        valueView.container().addMember("m_scale", valueView.m_data->m_scale);
    }

    template<FormatType TFormatType>
    void SerdeSpec<Node, TFormatType>::deserialize(DeserializableValueView<Node, TFormatType>& valueView, AssetsPackage& assetsPackage) noexcept
    {
        auto name = valueView.container().template getMember<std::string>("m_name");
        if(name)
        {
            valueView.m_data->m_name = std::move(*name);
        }

        auto children = valueView.container().template getMember<std::vector<Ref<Node>>>("m_children", assetsPackage);
        if(children)
        {
            valueView.m_data->m_children = std::move(*children);
        }

        auto meshesData = valueView.container().template getMember<std::vector<AssetRef<IMeshData>>>("m_meshesData", assetsPackage);
        if(meshesData)
        {
            valueView.m_data->m_meshesData = std::move(*meshesData);
        }

        auto position = valueView.container().template getMember<glm::vec3>("m_position");
        if(position)
        {
            valueView.m_data->m_position = *position;
        }

        auto rotationQuaternion = valueView.container().template getMember<glm::quat>("m_rotationQuaternion");
        if(rotationQuaternion)
        {
            valueView.m_data->m_rotationQuaternion = *rotationQuaternion;
        }

        auto scale = valueView.container().template getMember<glm::vec3>("m_scale");
        if(scale)
        {
            valueView.m_data->m_scale = *scale;
        }
    }
}
