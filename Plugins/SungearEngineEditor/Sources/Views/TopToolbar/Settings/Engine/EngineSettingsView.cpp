//
// Created by Ilya on 22.07.2024.
//

#include "EngineSettingsView.h"
#include "imgui_internal.h"
#include "SungearEngineEditor.h"

SGE::EngineSettingsView::EngineSettingsView()
{
    m_name = "Engine Settings";

    m_toolchainsDockedWindow = SGCore::MakeRef<ToolchainsDockedWindow>();
    m_toolchainsDockedWindow->m_name = "EngineSettingsToolchains";
    m_toolchainsDockedWindow->m_minSize = { 200, 0 };
    m_toolchainsDockedWindow->m_padding = { 0, 0 };
    m_toolchainsDockedWindow->m_itemsSpacing = { 0, 0 };
    m_toolchainsDockedWindow->m_engineSettingsRef = m_engineSettingsInstance;

    /*if(SungearEngineEditor::getInstance()->getMainView())
    {
        SungearEngineEditor::getInstance()->getMainView()->addChild(m_toolchainsDockedWindow);
    }*/

    TreeNode toolchainsNode {
            .m_text = "Toolchains",
            .onClicked = [this](TreeNode& self) {
                m_toolchainsDockedWindow->setActive(true);
                m_toolchainsDockedWindow->getSelectedToolchainDockedWindow()->setActive(true);
                m_toolchainsDockedWindow->getSelectedToolchainDockedWindow()->updateSelectedToolchain();
                m_isSettingsContentViewContainsDockedWindow = true;

                onSettingsContentDraw = [this]() {
                    m_toolchainsDockedWindow->getSelectedToolchainDockedWindow()->render();
                    m_toolchainsDockedWindow->render();
                };
            }
    };

    TreeNode devAndSupportNode {
            .m_text = "Development and Support",
            .onClicked = [this](TreeNode& self) {
                m_toolchainsDockedWindow->setActive(false);
                m_toolchainsDockedWindow->getSelectedToolchainDockedWindow()->setActive(false);
                m_isSettingsContentViewContainsDockedWindow = false;

                onSettingsContentDraw = [this]() {
                };
            },
            .m_children = { toolchainsNode },
            .m_isTree = true
    };

    m_settingsTree.addTreeNode(devAndSupportNode);
}

void SGE::EngineSettingsView::onActiveChangedListener()
{
    if(isActive())
    {
        *m_engineSettingsInstance = *EngineSettings::getInstance();
        m_toolchainsDockedWindow->refreshToolchainsList();
        m_toolchainsDockedWindow->getSelectedToolchainDockedWindow()->updateSelectedToolchain();
    }
    // m_toolchainsDockedWindow->m_currentToolchains = *Toolchains::getInstance().get();
}

void SGE::EngineSettingsView::onDock()
{
    m_settingsContentDockedWindow->split(ImGuiDir_Left, 0.3f, m_toolchainsDockedWindow.get(),
                                         m_toolchainsDockedWindow->getSelectedToolchainDockedWindow().get());
}

void SGE::EngineSettingsView::onOKPressed() noexcept
{
    onApplyPressed();
    onCancelPressed();
}

void SGE::EngineSettingsView::onApplyPressed() noexcept
{
    *EngineSettings::getInstance() = *m_engineSettingsInstance;
    EngineSettings::getInstance()->save("configs/engine_settings.json");
}

void SGE::EngineSettingsView::onCancelPressed() noexcept
{
    setActive(false);
}
