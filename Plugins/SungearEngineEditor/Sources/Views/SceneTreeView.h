//
// Created by ilya on 23.06.24.
//

#ifndef SUNGEARENGINEEDITOR_SCENETREEVIEW_H
#define SUNGEARENGINEEDITOR_SCENETREEVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/ImportedScenesArch/Bone.h>

#include "ImGuiUtils.h"
#include "Popup.h"
#include "Styles/StylesManager.h"

namespace SGE
{
    struct SceneTreeView : public SGCore::ImGuiWrap::IView
    {
        SceneTreeView() noexcept;

        bool begin() final;
        void renderBody() final;
        void end() final;

    private:
        void drawTreeNode(SGCore::ECS::entity_t entity) noexcept;
        void drawEntity(SGCore::ECS::entity_t entity) noexcept;
        void acceptDragNDrop(SGCore::ECS::entity_t entity, DragNDropType dndType) noexcept;

        SGCore::ECS::entity_t m_rightClickedEntity = entt::null;
        bool m_isTreeRightClicked = false;

        Popup m_popup {
            "SceneTreeViewActions",
            {
                {
                    .m_text = "New Entity",
                    .m_ID = "NewEntityAction",
                    .m_icon = StylesManager::getCurrentStyle()->m_plusIcon->getSpecialization(18,
                        18)->getTexture(),
                },
                {
                    .m_text = "Delete Entity",
                    .m_ID = "DeleteEntityAction",
                    .m_icon = StylesManager::getCurrentStyle()->m_trashBinIcon->getSpecialization(18,
                        18)->getTexture(),
                }
            }
        };
    };
}

#endif //SUNGEARENGINEEDITOR_SCENETREEVIEW_H
