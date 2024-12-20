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
