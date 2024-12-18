//
// Created by stuka on 06.09.2024.
//

#ifndef SUNGEARENGINEEDITOR_EDITORSCENE_H
#define SUNGEARENGINEEDITOR_EDITORSCENE_H

#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Scene/Scene.h>

#include <SGCore/Serde/Serde.h>
#include <SGCore/Serde/StandardSerdeSpecs.h>

namespace SGE
{
    // enum editor_camera : std::underlying_type_t<SGCore::entity_t> { };

    struct EditorSceneData
    {
        SGCore::entity_t m_editorCamera { };
        SGCore::entity_t m_editorGrid { };
    };

    struct EditorScene
    {
        SGCore::Ref<SGCore::Scene> m_scene;

        EditorSceneData m_data;

        void saveByPath(const std::filesystem::path& toPath, const std::filesystem::path& fileName) const noexcept;
        static SGCore::Ref<EditorScene> loadByPath(const std::filesystem::path& fromPath, const std::filesystem::path& fileName) noexcept;

        static void setCurrentScene(const SGCore::Ref<EditorScene>& scene) noexcept;
        SG_NOINLINE static SGCore::Ref<EditorScene> getCurrentScene() noexcept;

        /**
         * ONLY FOR EDITOR.
         * @param name
         * @return
         */
        static SGCore::Ref<EditorScene> createBasicScene(const std::string& name) noexcept;

        /**
         * ONLY FOR EDITOR.
         * @param name
         * @return
         */
        void addEditorEntities() noexcept;

    private:
        static inline SGCore::Ref<EditorScene> s_currentScene;
    };
}

/*template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::editor_camera, TFormatType> : SGCore::Serde::BaseTypes<>, SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGE::editor_camera";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<SGE::editor_camera, TFormatType>& valueView, registry_t& registry)
    {
        valueView.getValueContainer().setAsArray();

        {
            auto* component = registry.template try_get<EntityBaseInfo>(*valueView.m_data);

            if(component)
            {
                valueView.getValueContainer().pushBack(*component);
            }
        }

        {
            auto* component = registry.template try_get<Ref<Transform>>(*valueView.m_data);

            if(component)
            {
                valueView.getValueContainer().pushBack(*component);
            }
        }

        {
            auto* component = registry.template try_get<Ref<Camera3D>>(*valueView.m_data);

            if(component)
            {
                valueView.getValueContainer().pushBack(*component);
            }
        }

        {
            auto* component = registry.template try_get<Ref<RenderingBase>>(*valueView.m_data);

            if(component)
            {
                valueView.getValueContainer().pushBack(*component);
            }
        }

        {
            auto* component = registry.template try_get<Controllable3D>(*valueView.m_data);

            if(component)
            {
                valueView.getValueContainer().pushBack(*component);
            }
        }

        // TODO:
        *//*{
            auto* component = registry.template try_get<LayeredFrameReceiver>(*valueView.m_data);

            if(component)
            {
                valueView.getValueContainer().pushBack(*component);
            }
        }*//*
    }

    static void deserialize(DeserializableValueView<SGE::editor_camera, TFormatType>& valueView, registry_t& toRegistry)
    {
        // creating entity
        auto entity = toRegistry.create();

        // iterating through all elements of entityView
        for(auto componentsIt = valueView.getValueContainer().begin(); componentsIt != valueView.getValueContainer().end(); ++componentsIt)
        {
            const auto& currentElementTypeName = valueView.getValueContainer().getMemberTypeName(componentsIt);

            if(currentElementTypeName == SerdeSpec<SGCore::EntityBaseInfo, TFormatType>::type_name)
            {
                const auto component = valueView.getValueContainer().template getMember<SGCore::EntityBaseInfo>(componentsIt);

                if(component)
                {
                    toRegistry.emplace<SGCore::EntityBaseInfo>(entity, *component);

                    continue;
                }
            }

            if(currentElementTypeName == SerdeSpec<SGCore::Transform, TFormatType>::type_name)
            {
                const auto component = valueView.getValueContainer().template getMember<SGCore::Ref<SGCore::Transform>>(componentsIt);

                if(component)
                {
                    toRegistry.emplace<SGCore::Ref<SGCore::Transform>>(entity, *component);

                    continue;
                }
            }

            if(currentElementTypeName == SerdeSpec<SGCore::Camera3D, TFormatType>::type_name)
            {
                const auto component = valueView.getValueContainer().template getMember<SGCore::Ref<SGCore::Camera3D>>(componentsIt, entity, toRegistry);

                if(component)
                {
                    toRegistry.emplace<SGCore::Ref<SGCore::Camera3D>>(entity, *component);

                    continue;
                }
            }

            if(currentElementTypeName == SerdeSpec<SGCore::RenderingBase, TFormatType>::type_name)
            {
                const auto component = valueView.getValueContainer().template getMember<SGCore::Ref<SGCore::RenderingBase>>(componentsIt);

                if(component)
                {
                    toRegistry.emplace<SGCore::Ref<SGCore::RenderingBase>>(entity, *component);

                    continue;
                }
            }

            if(currentElementTypeName == SerdeSpec<SGCore::Controllable3D, TFormatType>::type_name)
            {
                const auto component = valueView.getValueContainer().template getMember<SGCore::Controllable3D>(componentsIt);

                if(component)
                {
                    toRegistry.emplace<SGCore::Controllable3D>(entity, *component);

                    continue;
                }
            }

            *//*if(currentElementTypeName == SerdeSpec<SGCore::LayeredFrameReceiver, TFormatType>::type_name)
            {
                const auto component = valueView.getValueContainer().template getMember<SGCore::LayeredFrameReceiver>(componentsIt);

                if(component)
                {
                    toRegistry.emplace<SGCore::LayeredFrameReceiver>(entity, *component);

                    continue;
                }
            }*//*

            toRegistry.emplace<SGCore::LayeredFrameReceiver>(entity);
            toRegistry.emplace<SGCore::NonSavable>(entity);
        }

        // getting EntityBaseInfo of current entity to add all children entities
        EntityBaseInfo* entityBaseInfo = toRegistry.template try_get<EntityBaseInfo>(entity);
        if(!entityBaseInfo)
        {
            return;
        }

        LOG_I(SGCORE_TAG, "Creating EntityBaseInfo component for editor camera '{}'....", std::to_underlying(entity));
        // creating EntityBaseInfo component with 'entity' as identifier in arguments
        *entityBaseInfo = EntityBaseInfo(entity);

        *valueView.m_data = entity;
    }
};*/

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::EditorSceneData, TFormatType> : SGCore::Serde::BaseTypes<>, SGCore::Serde::DerivedTypes<>
{
    static inline const std::string type_name = "SGE::EditorSceneData";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<SGE::EditorSceneData, TFormatType>& valueView, const Scene& scene)
    {
        {
            SceneEntitySaveInfo entitySaveInfo;
            entitySaveInfo.m_serializableEntity = valueView.m_data->m_editorCamera;
            entitySaveInfo.m_serializableScene = &scene;
            valueView.getValueContainer().addMember("m_editorCamera", entitySaveInfo);
        }

        {
            SceneEntitySaveInfo gridSaveInfo;
            gridSaveInfo.m_serializableEntity = valueView.m_data->m_editorGrid;
            gridSaveInfo.m_serializableScene = &scene;
            valueView.getValueContainer().addMember("m_editorGrid", gridSaveInfo);
        }
    }

    static void deserialize(DeserializableValueView<SGE::EditorSceneData, TFormatType>& valueView, Scene& scene)
    {
        const auto m_editorCamera = valueView.getValueContainer().template getMember<SceneEntitySaveInfo>("m_editorCamera", *scene.getECSRegistry());
        if(m_editorCamera)
        {
            valueView.m_data->m_editorCamera = m_editorCamera->m_serializableEntity;

            scene.getECSRegistry()->emplace<SGCore::LayeredFrameReceiver>(valueView.m_data->m_editorCamera);
            scene.getECSRegistry()->emplace<SGCore::NonSavable>(valueView.m_data->m_editorCamera);
        }

        const auto m_editorGrid = valueView.getValueContainer().template getMember<SceneEntitySaveInfo>("m_editorGrid", *scene.getECSRegistry());
        if(m_editorGrid)
        {
            valueView.m_data->m_editorGrid = m_editorGrid->m_serializableEntity;

            /*scene.getECSRegistry()->emplace<SGCore::LayeredFrameReceiver>(valueView.m_data->m_editorCamera);
            scene.getECSRegistry()->emplace<SGCore::NonSavable>(valueView.m_data->m_editorCamera);*/
        }
    }
};

#endif //SUNGEARENGINEEDITOR_EDITORSCENE_H
