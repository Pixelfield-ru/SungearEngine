//
// Created by Ilya on 24.07.2024.
//

#include "SelectedToolchainDockedWindow.h"
#include "Styles/StylesManager.h"
#include "ImGuiUtils.h"
#include "nfd.h"

#include <imgui_stdlib.h>

void SGE::SelectedToolchainDockedWindow::renderBody()
{
    if(m_selectedToolchain)
    {
        auto folderTexture = StylesManager::getCurrentStyle()->m_folderIcon
                ->getSpecialization(20, 20)
                ->getTexture();

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1, 3));

        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 3, ImGuiTableFlags_SizingStretchProp))
        {
            /*ImGui::TableSetupColumn("test", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("test0", ImGuiTableColumnFlags_WidthStretch);
            ImGui::TableSetupColumn("test1", ImGuiTableColumnFlags_WidthStretch);*/

            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
                ImGui::Text("Name");

                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::InputText("##ToolchainNameInputText", &m_currentToolchainName, ImGuiInputTextFlags_EnterReturnsTrue);

                if(ImGui::IsItemEdited())
                {
                    m_selectedToolchain->m_name = m_currentToolchainName;
                    m_currentToolchainName = m_selectedToolchain->m_name.getName();

                    if(onToolchainChanged)
                    {
                        onToolchainChanged();
                    }
                }

                ImGui::TableNextColumn();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                if (ImGuiUtils::ImageButton(folderTexture->getTextureNativeHandler(),
                                            ImVec2(folderTexture->getWidth() + 6, folderTexture->getHeight() + 6),
                                            ImVec2(folderTexture->getWidth(), folderTexture->getHeight())).m_isLMBClicked)
                {
                    /*char* dat = m_dirPath.data();
                    nfdresult_t result = NFD_PickFolder("", &dat);
                    if (result == NFD_OKAY)
                    {
                        m_dirPath = dat;
                    }*/
                }
            }


            ImGui::GetWindowDrawList()->AddLine(ImGui::GetCursorPos(),
                                                ImVec2(ImGui::GetCursorPos().x + ImGui::GetContentRegionAvail().x,
                                                       ImGui::GetCursorPos().y), ImGui::ColorConvertFloat4ToU32({ 1.0, 1.0, 1.0, 1.0 }));

            /*ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1, 3));
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                ImGui::Separator();

                ImGui::TableNextColumn();
                ImGui::Separator();

                ImGui::TableNextColumn();
                ImGui::Separator();
            }
            ImGui::PopStyleVar();*/

            ImGui::EndTable();
        }

        ImGui::PopStyleVar();
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
