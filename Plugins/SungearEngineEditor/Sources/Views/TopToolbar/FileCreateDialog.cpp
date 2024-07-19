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

void SGE::FileCreateDialog::renderBody()
{
    m_isPopupWindow = true;

    switch(m_mode)
    {
        case OPEN:
            m_name = "Open File";
            break;
        case CREATE:
            m_name = "Create File";
            break;
    }

    /*if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
    {
        ImGui::SetKeyboardFocusHere(1);
    }*/
    
    ImGui::SetWindowFocus();
    
    ImVec2 texSize0 = ImGui::CalcTextSize("Location");
    ImVec2 texSize1 = ImGui::CalcTextSize("File Name");
    
    ImGui::Text("Location");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
    ImGui::SetNextItemWidth(320);
    ImGui::InputText("##DirectoryPath", &m_currentChosenDirPath);
    
    if(ImGui::IsItemEdited())
    {
        m_error = "";
    }
    
    ImGui::SameLine();
    
    auto folderTexture = StylesManager::getCurrentStyle()->m_folderIcon
            ->getSpecialization(20, 20)
            ->getTexture();
    
    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
    if(ImGuiUtils::ImageButton(folderTexture->getTextureNativeHandler(),
                               ImVec2(folderTexture->getWidth() + 6, folderTexture->getHeight() + 6),
                               ImVec2(folderTexture->getWidth(), folderTexture->getHeight())).m_isLMBClicked)
    {
        char* dat = m_currentChosenDirPath.data();
        nfdresult_t result = NFD_PickFolder(m_defaultPath.c_str(), &dat);
        if(result == NFD_OKAY)
        {
            m_currentChosenDirPath = dat;
        }
    }
    
    
    ImGui::Text("File Name");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (texSize0.x - texSize1.x) + 10.0f);
    ImGui::SetNextItemWidth(320);
    ImGui::InputText("##FileName", &m_fileName);
    
    if(ImGui::IsItemEdited())
    {
        m_error = "";
    }
    
    /*if(!m_error.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_error.c_str());
    }*/
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER))
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
    else if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE))
    {
        m_fileName.clear();
        m_currentChosenDirPath.clear();
        
        m_error = "";
        
        setActive(false);
        
        onOperationEnd(m_currentChosenDirPath + "/" + m_fileName, true);
    }
}

void SGE::FileCreateDialog::onActiveChangedListener()
{

}

void SGE::FileCreateDialog::postRenderBody()
{
    if(!m_error.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_error.c_str());
    }
}
