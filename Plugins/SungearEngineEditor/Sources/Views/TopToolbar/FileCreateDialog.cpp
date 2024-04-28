//
// Created by ilya on 28.04.24.
//

#include <imgui.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Input/InputManager.h>

#include "FileCreateDialog.h"
#include "Styles/StylesManager.h"

SGE::FileCreateDialog::FileCreateDialog()
{
    m_dirPath.reserve(128);
    m_fileName.reserve(128);
}

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
    
    ImGui::Begin("Create File", nullptr,
                 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove);
    
    auto asset = SGCore::AssetManager::loadAsset<SGCore::ITexture2D>("air");
    // asset->bind(0);
    // ImGui::Image((void*) asset.get(), ImVec2(16, 16));
    
    // auto asset = SGCore::AssetManager::loadAsset<SGCore::ITexture2D>(SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/textures/img.png");
    ImGui::Image(asset->getTextureNativeHandler(), ImVec2(32, 32));
    
    ImGui::SetWindowFocus();
    
    ImVec2 texSize0 = ImGui::CalcTextSize("Directory Path");
    ImVec2 texSize1 = ImGui::CalcTextSize("File Name");
    
    ImGui::Text("Directory Path");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 10.0f);
    ImGui::InputText("##DirectoryPath", m_dirPath.data(), 128);
    
    ImGui::Text("File Name");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (texSize0.x - texSize1.x) + 10.0f);
    ImGui::InputText("##FileName", m_fileName.data(), 128);
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER))
    {
        m_fileName.clear();
        m_dirPath.clear();
        
        setActive(false);
    }
    else if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE))
    {
        m_fileName.clear();
        m_dirPath.clear();
        
        setActive(false);
    }
    
    ImGui::End();
}

void SGE::FileCreateDialog::end()
{
    IView::end();
}

void SGE::FileCreateDialog::onActiveChangedListener()
{

}
