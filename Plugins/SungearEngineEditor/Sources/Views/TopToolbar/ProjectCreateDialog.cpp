//
// Created by ilya on 30.04.24.
//

#include <imgui.h>
#include <imgui_stdlib.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <nfd.h>
#include <SGCore/Input/InputManager.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/Memory/Assets/SVGImage.h>
#include <SGCore/Annotations/AnnotationsProcessor.h>
#include <SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/SerializersGenerator.h>

#include "ProjectCreateDialog.h"
#include "SungearEngineEditor.h"
#include "ImGuiUtils.h"

#include "Views/Explorer/DirectoryExplorer.h"
#include "Views/Explorer/DirectoriesTreeExplorer.h"
#include "Project/CodeGen/CodeGeneration.h"

void SGE::ProjectCreateDialog::renderBody()
{
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    
    ImGui::SetNextWindowSize(ImVec2(m_size.x, m_size.y));
    ImGui::SetNextWindowPos(ImVec2(center.x - m_size.x / 2.0f, center.y - m_size.y / 2.0f));
    
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    
    if(isActive())
    {
        ImGui::OpenPopup("Create Project");
    }
    
    ImGui::BeginPopupModal("Create Project", nullptr,
                 ImGuiWindowFlags_NoDocking | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize |
                 ImGuiWindowFlags_NoMove);
    
    ImVec2 texSize0 = ImGui::CalcTextSize("Location");
    ImVec2 texSize1 = ImGui::CalcTextSize("Project Name");
    ImVec2 texSize2 = ImGui::CalcTextSize("C++ Standard");
    
    float biggestX = texSize0.x;
    if(biggestX < texSize1.x) biggestX = texSize1.x;
    if(biggestX < texSize2.x) biggestX = texSize2.x;
    
    ImGui::Text("Location");
    ImGui::SameLine();
    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (biggestX - texSize0.x) + 10.0f);
    ImGui::SetNextItemWidth(320);
    ImGui::InputText("##DirectoryPath", &m_dirPath);
    
    if(ImGui::IsItemEdited())
    {
        m_error = "";
        m_size.y = m_originalSize.y;
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
        char* dat = m_dirPath.data();
        nfdresult_t result = NFD_PickFolder("", &dat);
        if(result == NFD_OKAY)
        {
            m_dirPath = dat;
        }
    }
    
    if(m_mode == FileOpenMode::CREATE)
    {
        ImGui::Text("Project Name");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (biggestX - texSize1.x) + 10.0f);
        ImGui::SetNextItemWidth(320);
        ImGui::InputText("##FileName", &m_projectName);
        
        if(ImGui::IsItemEdited())
        {
            m_error = "";
            m_size.y = m_originalSize.y;
        }
    }
    
    static const char* cppStandards[] = { "C++98", "C++03", "C++11", "C++14", "C++17", "C++20", "C++23" };
    if(m_mode == FileOpenMode::CREATE)
    {
        ImGui::Text("C++ Standard");
        ImGui::SameLine();
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + (biggestX - texSize2.x) + 10.0f);
        
        ImGui::Combo("##CreateProject_CPPStandard", &m_currentSelectedCPPStandard, cppStandards, 7);
    }

    if(!m_error.empty())
    {
        ImGui::TextColored(ImVec4(1, 0, 0, 1), m_error.c_str());
    }
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER))
    {
        if(!std::filesystem::exists(m_dirPath))
        {
            m_error = "This directory does not exist!";
            m_size.y = m_originalSize.y + 20;
        }
        else if(!std::filesystem::exists(m_dirPath + "/" + m_projectName) && m_mode == FileOpenMode::CREATE)
        {
            std::filesystem::create_directory(m_dirPath + "/" + m_projectName);
            std::string projectSourcesCreateError;
            SGCore::PluginProject pluginProject = SGCore::PluginsManager::createPluginProject(
                    m_dirPath + "/" + m_projectName + "/Sources", m_projectName,
                    cppStandards[m_currentSelectedCPPStandard], projectSourcesCreateError);
            
            if(!projectSourcesCreateError.empty())
            {
                m_error = projectSourcesCreateError;
                m_size.y = m_originalSize.y + 20;
                
                std::filesystem::remove(m_dirPath + "/" + m_projectName);
                
                return;
            }
            
            std::filesystem::create_directory(m_dirPath + "/" + m_projectName + "/Resources");

            auto& currentEditorProject = SungearEngineEditor::getInstance()->m_currentProject;

            // PROJECT SUCCESSFULLY CREATED ==============
            
            SungearEngineEditor::getInstance()->getMainView()->getDirectoriesTreeExplorer()->m_rootPath = m_dirPath + "/" + m_projectName;
            currentEditorProject.m_pluginProject = pluginProject;

            // PARSING SUNGEAR ENGINE ANNOTATIONS AND GENERATING CODE ==========================

            SGCore::AnnotationsProcessor annotationsProcessor;

            const char* sungearRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
            std::string sungearRootStr;
            if(sungearRoot)
            {
                sungearRootStr = sungearRoot;
            }

            annotationsProcessor.processAnnotations(sungearRootStr + "/Sources",
                                                    {sungearRootStr + "/Sources/SGCore/Annotations/Annotations.h",
                                                     sungearRootStr +
                                                     "/Sources/SGCore/Annotations/AnnotationsProcessor.cpp",
                                                     sungearRootStr +
                                                     "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/SerializersGenerator.cpp"});

            SGCore::CodeGen::SerializersGenerator serializersGenerator;
            std::string serializersGenerationError = serializersGenerator.generateSerializers(annotationsProcessor,
                                                                                              sungearRootStr + "/.SG_GENERATED/Serializers.h");

            annotationsProcessor.saveToFile(sungearRootStr + "/.SG_GENERATED/AnnotationsProcessor.json");

            // TODO: MAKE ERROR DIALOG SHOW IF ERROR
            if(!serializersGenerationError.empty())
            {
                spdlog::error("Error while generating serializers for Sungear Engine: {0}", serializersGenerationError);
                std::printf("Error while generating serializers for Sungear Engine: %s\n", serializersGenerationError.c_str());
            }

            // CREATING GENERATED CODE ENTRY POINT

            CodeGeneration::generateCode({ annotationsProcessor }, pluginProject.m_pluginPath);

            // =====================================================================================
            // BUILDING CREATED PROJECT

            // currentEditorProject.m_editorHelper.load("")

            // =====================================================================================

            m_projectName.clear();
            m_dirPath.clear();
            
            m_error = "";
            
            setActive(false);
        }
        else if(std::filesystem::exists(m_dirPath) && m_mode == FileOpenMode::OPEN)
        {
            SungearEngineEditor::getInstance()->getMainView()->getDirectoriesTreeExplorer()->m_rootPath = m_dirPath;
            
            m_projectName.clear();
            m_dirPath.clear();
            
            m_error = "";
            
            setActive(false);
        }
        else
        {
            m_error = "This project already exists!";
            m_size.y = m_originalSize.y + 20;
        }
    }
    else if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE))
    {
        m_projectName.clear();
        m_dirPath.clear();
        
        m_error = "";
        
        setActive(false);
        
        m_size.y = m_originalSize.y;
    }
    
    ImGui::EndPopup();
    
    ImGui::PopStyleVar();
}

void SGE::ProjectCreateDialog::onActiveChangedListener()
{
    if(m_mode == FileOpenMode::OPEN)
    {
        m_originalSize.y = 60;
    }
    else
    {
        m_originalSize.y = 110;
    }
    
    m_size = m_originalSize;
}
