//
// Created by ilya on 23.06.24.
//

#ifndef SUNGEARENGINEEDITOR_SCENETREEVIEW_H
#define SUNGEARENGINEEDITOR_SCENETREEVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/ImportedScenesArch/Bone.h>

namespace SGE
{
    struct SceneTreeView : public SGCore::ImGuiWrap::IView
    {
        bool begin() final;
        void renderBody() final;
        void end() final;

    private:
        void drawTreeNode(const SGCore::ECS::entity_t& parentEntity, bool checkForRoot) noexcept;
        void drawEntity(SGCore::ECS::entity_t entity) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_SCENETREEVIEW_H
