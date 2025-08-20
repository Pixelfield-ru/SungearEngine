//
// Created by ilya on 28.04.24.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <nfd.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Input/InputManager.h>

#include "FileCreateDialog.h"
#include "Styles/StylesManager.h"
#include "ImGuiUtils.h"

SGE::FileCreateDialog::FileCreateDialog()
{
    m_isPopupWindow = true;
    m_enableEscapeToCloseWindow = false;

    const auto buttonsSize = ImVec2(75, 0);

    addButton({
                      .m_text = "OK",
                      .m_name = "OKButton",
                      .isFastClicked = [](auto& self) -> bool {
                          return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER);
                      },
                      .onClicked = [this](auto& self, SGCore::ImGuiWrap::IView* parentView) {
                          submit();
                      },
                      .m_color = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1),
                      .m_hoveredColor = ImVec4(0 / 255.0f, 70 / 255.0f, 110 / 255.0f, 1),
                      .m_borderColor = { 0, 0, 0, 0 },
                      .m_borderShadowColor = { 0, 0, 0, 0 },
                      .m_size = buttonsSize
              });

    addButton({
                      .m_text = "Cancel",
                      .m_name = "CancelButton",
                      .isFastClicked = [](auto& self) -> bool {
                          return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE);
                      },
                      .onClicked = [this](auto& self, SGCore::ImGuiWrap::IView* parentView) {
                          cancel();
                      },
                      .m_size = buttonsSize
              });
}

void SGE::FileCreateDialog::renderBody()
{
    switch(m_mode)
    {
        case OPEN:
            m_bodyMinSize = { 450, 40 };
            // m_name = "Open File";
            break;
        case CREATE:
            m_bodyMinSize = { 450, 65 };
            // m_name = "Create File";
            break;
    }

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1, 3));

    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if(ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 3, ImGuiTableFlags_SizingStretchProp))
    {
        if(!m_disallowPathSpecifying)
        {
            ImGui::TableNextRow();
            {
                ImGui::TableNextColumn();
                ImGui::Text("Location");

                ImGui::TableNextColumn();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputText("##DirectoryPath", &m_currentChosenDirPath);

                if(ImGui::IsItemEdited())
                {
                    m_error = "";
                }

                ImGui::TableNextColumn();

                auto folderTexture = StylesManager::getCurrentStyle()->m_folderIcon
                        ->getSpecialization(20, 20)
                        ->getTexture();

                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                if(ImGuiUtils::ImageButton(folderTexture->getTextureNativeHandler(),
                                           ImVec2(folderTexture->getWidth() + 6, folderTexture->getHeight() + 6),
                                           ImVec2(folderTexture->getWidth(),
                                                  folderTexture->getHeight())).m_isLMBClicked)
                {
                    char* dat = m_currentChosenDirPath.data();
                    nfdresult_t result = NFD_PickFolder(&dat, nullptr);
                    if(result == NFD_OKAY)
                    {
                        m_currentChosenDirPath = dat;
                    }
                }
            }
        }

        if (m_mode == FileOpenMode::CREATE)
        {
            ImGui::TableNextRow();

            {
                ImGui::TableNextColumn();
                ImGui::Text("File Name");

                ImGui::TableNextColumn();
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                ImGui::InputText("##FileName", &m_fileName);

                if (ImGui::IsItemEdited())
                {
                    m_error = "";
                }

                ImGui::TableNextColumn();
            }
        }

        ImGui::EndTable();
    }

    ImGui::PopStyleVar();
}

void SGE::FileCreateDialog::onActiveChangedListener()
{

}

void SGE::FileCreateDialog::footerRender()
{
    if(!m_error.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_error.c_str());
    }
}

void SGE::FileCreateDialog::submit()
{
    if(!std::filesystem::exists(m_currentChosenDirPath))
    {
        m_error = "This directory does not exist!";
    }
    else if(!std::filesystem::exists(m_currentChosenDirPath + "/" + m_fileName))
    {
        std::filesystem::path pathName = m_currentChosenDirPath + "/" + m_fileName;
        if(pathName.extension() != m_ext)
        {
            pathName += m_ext;
        }
        if(!m_isCreatingDirectory)
        {
            std::ofstream ofstream(pathName);
        }
        else
        {
            std::filesystem::create_directories(pathName);
        }

        m_fileName.clear();
        m_currentChosenDirPath.clear();

        m_error = "";

        setActive(false);

        onOperationEnd(pathName, false);
    }
    else
    {
        m_error = "This file already exists!";
    }
}

void SGE::FileCreateDialog::cancel()
{
    m_fileName.clear();
    m_currentChosenDirPath.clear();

    m_error = "";

    setActive(false);

    onOperationEnd(m_currentChosenDirPath + "/" + m_fileName, true);
}
