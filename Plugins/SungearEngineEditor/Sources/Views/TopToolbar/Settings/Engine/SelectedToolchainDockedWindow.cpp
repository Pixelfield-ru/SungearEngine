//
// Created by Ilya on 24.07.2024.
//

#include "SelectedToolchainDockedWindow.h"

#include <imgui_stdlib.h>

void SGE::SelectedToolchainDockedWindow::renderBody()
{
    if(m_selectedToolchain)
    {
        if(ImGui::InputText("##ToolchainNameInputText", &m_currentToolchainName, ImGuiInputTextFlags_EnterReturnsTrue))
        {
            m_selectedToolchain->m_name = m_currentToolchainName;
            m_currentToolchainName = m_selectedToolchain->m_name.getName();

            if(onToolchainChanged)
            {
                onToolchainChanged();
            }
        }
    }
}

void SGE::SelectedToolchainDockedWindow::setSelectedToolchain(const SGCore::Ref<Toolchain>& toolchain) noexcept
{
    m_selectedToolchain = toolchain;

    if(m_selectedToolchain)
    {
        m_currentToolchainName = toolchain->m_name.getName();
    }
}

SGCore::Ref<SGE::Toolchain> SGE::SelectedToolchainDockedWindow::getSelectedToolchain() const noexcept
{
    return m_selectedToolchain;
}
