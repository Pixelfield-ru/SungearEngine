//
// Created by Ilya on 24.07.2024.
//

#include "SelectedToolchainDockedWindow.h"
#include "Styles/StylesManager.h"
#include "ImGuiUtils.h"
#include "nfd.h"
#include <SGCore/Exceptions/FileNotFoundException.h>

#include <imgui_stdlib.h>

void SGE::SelectedToolchainDockedWindow::renderBody()
{
    if(m_selectedToolchain)
    {
        auto folderTexture = StylesManager::getCurrentStyle()->m_folderIcon
                ->getSpecialization(20, 20)
                ->getTexture();

        auto greenCheckMarkTexture = StylesManager::getCurrentStyle()->m_greenCheckmark
                ->getSpecialization(16, 16)
                ->getTexture();

        auto redCrossTexture = StylesManager::getCurrentStyle()->m_redCross
                ->getSpecialization(22, 22)
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
                    setSelectedToolchainPath(m_currentToolchainPath);

                    if(onToolchainChanged)
                    {
                        onToolchainChanged();
                    }
                }

                if (!m_toolchainPathError.empty())
                {
                    if(!m_toolchainPathError.contains("Correct toolchain"))
                    {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                        ImGui::Image(redCrossTexture->getTextureNativeHandler(),
                                     { (float) redCrossTexture->getWidth(),
                                       (float) redCrossTexture->getHeight() });
                        ImGui::SameLine();
                        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
                        ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, m_toolchainPathError.c_str());
                    }
                    else
                    {
                        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                        ImGui::Image(greenCheckMarkTexture->getTextureNativeHandler(),
                                     { (float) greenCheckMarkTexture->getWidth(),
                                       (float) greenCheckMarkTexture->getHeight() });
                        ImGui::SameLine();
                        ImGui::TextColored({ 1.0, 1.0, 1.0, 1.0 }, m_toolchainPathError.c_str());
                    }
                }

                ImGui::TableNextColumn();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                if (ImGuiUtils::ImageButton(folderTexture->getTextureNativeHandler(),
                                            ImVec2(folderTexture->getWidth() + 6, folderTexture->getHeight() + 6),
                                            ImVec2(folderTexture->getWidth(), folderTexture->getHeight())).m_isLMBClicked)
                {
                    char* dat = m_currentToolchainPath.data();
                    nfdresult_t result = NFD_PickFolder("", &dat);
                    if (result == NFD_OKAY)
                    {
                        m_currentToolchainPath = dat;
                        setSelectedToolchainPath(m_currentToolchainPath);
                    }
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
        setSelectedToolchainPath(m_currentToolchainPath);
    }
}

SGCore::Ref<SGE::Toolchain> SGE::SelectedToolchainDockedWindow::getSelectedToolchain() const noexcept
{
    return m_selectedToolchain;
}

void SGE::SelectedToolchainDockedWindow::setSelectedToolchainPath(const std::filesystem::path& path)
{
    try
    {
        m_selectedToolchain->setPath(m_currentToolchainPath);
        m_toolchainPathError = "Correct toolchain";
    }
    catch(const SGCore::FileNotFoundException& e)
    {
        std::string errWhat = e.what();
        if(errWhat.contains("cmake.exe"))
        {
            m_cmakePathError = errWhat;
        }
        else if(errWhat.contains("build tool"))
        {
            m_buildToolPathError = errWhat;
        }
        else
        {
            m_toolchainPathError = errWhat;
        }
    }
    catch(const std::exception& e)
    {
        m_toolchainPathError = "Incorrect path";
    }
}
