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

        ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1, 10));
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
        if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 3, ImGuiTableFlags_SizingStretchProp))
        {
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
            }

            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                ImGui::GetWindowDrawList()->AddLine({ ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - 10 } ,
                                                    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x - 17,
                                                           ImGui::GetCursorScreenPos().y - 10),
                                                    ImGui::ColorConvertFloat4ToU32({ 0.60f, 0.60f, 0.60f, 0.29f }));
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
                ImGui::Text("Path");

                ImGui::TableNextColumn();
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::InputText("##ToolchainPathInputText", &m_currentToolchainPath, ImGuiInputTextFlags_EnterReturnsTrue);

                if(ImGui::IsItemEdited())
                {
                    try
                    {
                        m_selectedToolchain->setPath(m_currentToolchainPath);
                    }
                    catch(const std::exception& e)
                    {
                        // TODO: MAKE ERROR SHOW BELOW
                    }

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

            ImGui::EndTable();
        }
        ImGui::PopStyleVar();

        // ImGui::Separator();
    }
}

void SGE::SelectedToolchainDockedWindow::setSelectedToolchain(const SGCore::Ref<Toolchain>& toolchain) noexcept
{
    m_selectedToolchain = toolchain;

    if(m_selectedToolchain)
    {
        m_currentToolchainName = toolchain->m_name.getName();
        m_currentToolchainPath = SGCore::Utils::toUTF8(toolchain->getPath().u16string());
    }
}

SGCore::Ref<SGE::Toolchain> SGE::SelectedToolchainDockedWindow::getSelectedToolchain() const noexcept
{
    return m_selectedToolchain;
}
