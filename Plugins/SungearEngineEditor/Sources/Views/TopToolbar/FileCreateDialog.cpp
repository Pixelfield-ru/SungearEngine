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

bool SGE::FileCreateDialog::begin()
{
    return true;
}

void SGE::FileCreateDialog::renderBody()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    
    ImGui::SetNextWindowSize(ImVec2(m_size.x, m_size.y));
    ImGui::SetNextWindowPos(ImVec2(center.x - m_size.x / 2.0f, center.y - m_size.y / 2.0f));
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));

    if(isActive())
    {
        ImGui::OpenPopup(m_dialogTitle.c_str());
    }
    
    ImGui::BeginPopupModal(m_dialogTitle.c_str(), nullptr,
                           ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                           ImGuiWindowFlags_NoMove);
    
    if (ImGui::IsWindowFocused(ImGuiFocusedFlags_RootAndChildWindows) && !ImGui::IsAnyItemActive() && !ImGui::IsMouseClicked(0))
    {
        ImGui::SetKeyboardFocusHere(1);
    }
    
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
        m_size.y = 85;
    }
    
    ImGui::SameLine();
    
    auto folderTexture = Resources::getMainAssetManager().loadAsset<SGCore::SVGImage>("folder")
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
        m_size.y = 85;
    }
    
    if(!m_error.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_error.c_str());
    }
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER))
    {
        if(!std::filesystem::exists(m_currentChosenDirPath))
        {
            m_error = "This directory does not exist!";
            m_size.y = 105;
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
            m_size.y = 105;
        }
    }
    else if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE))
    {
        m_fileName.clear();
        m_currentChosenDirPath.clear();
        
        m_error = "";
        
        setActive(false);
        
        m_size.y = 85;
        
        onOperationEnd(m_currentChosenDirPath + "/" + m_fileName, true);
    }
    
    ImGui::EndPopup();
    
    ImGui::PopStyleVar();
}

void SGE::FileCreateDialog::end()
{
    IView::end();
}

void SGE::FileCreateDialog::onActiveChangedListener()
{

}
