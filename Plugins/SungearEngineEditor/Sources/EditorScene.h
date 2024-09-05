//
// Created by stuka on 06.09.2024.
//

#ifndef SUNGEARENGINEEDITOR_EDITORSCENE_H
#define SUNGEARENGINEEDITOR_EDITORSCENE_H

#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Scene/Scene.h>

namespace SGE
{
    struct EditorScene
    {
        SGCore::Ref<SGCore::Scene> m_scene;

        SGCore::entity_t m_editorCamera { };
        SGCore::entity_t m_editorGrid { };

        static void setCurrentScene(const SGCore::Ref<EditorScene>& scene) noexcept;
        SG_NOINLINE static SGCore::Ref<EditorScene> getCurrentScene() noexcept;

    private:
        static inline SGCore::Ref<EditorScene> s_currentScene;
    };
}

#endif //SUNGEARENGINEEDITOR_EDITORSCENE_H
