//
// Created by Ilya on 22.07.2024.
//

#include "EngineSettingsView.h"
#include "imgui_internal.h"

SGE::EngineSettingsView::EngineSettingsView()
{
    m_name = "Engine Settings";

    m_toolchainsDockedWindow = SGCore::MakeRef<ToolchainsDockedWindow>();
    m_toolchainsDockedWindow->m_name = "EngineSettingsToolchains";
    m_toolchainsDockedWindow->m_minSize = { 200, 0 };
    m_toolchainsDockedWindow->m_padding = { 0, 0 };
    m_toolchainsDockedWindow->m_itemsSpacing = { 0, 0 };

    TreeNode toolchainsNode {
            .m_text = "Toolchains",
            .onClicked = [this](TreeNode& self) {
                m_settingsContentDockedWindow->split(ImGuiDir_Left, 0.3f, m_toolchainsDockedWindow.get(), nullptr);

                onSettingsContentDraw = [this]() {
                    if(m_toolchainsDockedWindow->begin())
                    {
                        m_toolchainsDockedWindow->renderBody();

                        m_toolchainsDockedWindow->end();
                    }
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