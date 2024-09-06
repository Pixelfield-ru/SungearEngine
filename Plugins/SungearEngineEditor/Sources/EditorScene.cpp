//
// Created by stuka on 06.09.2024.
//

#include "EditorScene.h"

#include <SGCore/Scene/Serializer.h>
#include <SGCore/Scene/StandardSerdeSpecs.h>
#include <SGCore/Utils/FileUtils.h>

template<SGCore::Serde::FormatType TFormatType>
struct SGCore::Serde::SerdeSpec<SGE::EditorSceneData, TFormatType> : BaseTypes<>, DerivedTypes<>
{
    static inline const std::string type_name = "SGE::EditorSceneData";
    static inline constexpr bool is_pointer_type = false;

    static void serialize(SerializableValueView<SGE::EditorSceneData, TFormatType>& valueView)
    {
        valueView.getValueContainer().addMember("m_editorCamera", valueView.m_data->m_editorCamera);
        valueView.getValueContainer().addMember("m_editorGrid", valueView.m_data->m_editorGrid);
    }

    static void deserialize(DeserializableValueView<SGE::EditorSceneData, TFormatType>& valueView)
    {
        const auto editorCamera = valueView.getValueContainer()
                .template getMember<SGCore::entity_t>("m_editorCamera");
        if(editorCamera)
        {
            valueView.m_data->m_editorCamera = *editorCamera;
        }

        const auto editorGrid = valueView.getValueContainer()
                .template getMember<SGCore::entity_t>("m_editorGrid");
        if(editorGrid)
        {
            valueView.m_data->m_editorGrid = *editorGrid;
        }
    }
};

void SGE::EditorScene::setCurrentScene(const SGCore::Ref<SGE::EditorScene>& scene) noexcept
{
    s_currentScene = scene;
    SGCore::Scene::setCurrentScene(scene->m_scene->m_name);
}

SGCore::Ref<SGE::EditorScene> SGE::EditorScene::getCurrentScene() noexcept
{
    return s_currentScene;
}

void SGE::EditorScene::saveByPath(const std::filesystem::path& toPath, const std::filesystem::path& fileName) const noexcept
{
    if(!m_scene) return;

    std::filesystem::path scenePath = toPath / fileName;
    scenePath += ".sgscene";

    std::filesystem::path editorScenePath = toPath / fileName;
    editorScenePath += ".es";

    m_scene->saveToFile(scenePath);

    SGCore::FileUtils::writeToFile(editorScenePath, SGCore::Serde::Serializer::toFormat(m_data), false, true);
}
