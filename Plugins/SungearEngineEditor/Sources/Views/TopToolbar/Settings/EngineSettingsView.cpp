//
// Created by Ilya on 22.07.2024.
//

#include "EngineSettingsView.h"

SGE::EngineSettingsView::EngineSettingsView()
{
    m_name = "Engine Settings";

    TreeNode toolchainsNode {
            .m_text = "Toolchains",
            .onClicked = [this](TreeNode& self) {
                onSettingsContentDraw = []() {
                    ImGui::Text("Hello from Toolchains!");
                };
            }
    };

    TreeNode devAndSupportNode {
            .m_text = "Development and Support",
            .onClicked = [this](TreeNode& self) {
                onSettingsContentDraw = [this]() {
                };
            },
            .m_children = { toolchainsNode },
            .m_isTree = true
    };

    m_settingsTree.addTreeNode(devAndSupportNode);
}
