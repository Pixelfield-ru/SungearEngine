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
#include "Toolchains//VisualStudioToolchain.h"
#include "Toolchains/Toolchains.h"

void SGE::ProjectCreateDialog::renderBody()
{
    m_isPopupWindow = true;

    switch(m_mode)
    {
        case OPEN:
            m_name = "Open Project";
            break;
        case CREATE:
            m_name = "Create Project";
            break;
    }

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

            if(!Toolchains::getToolchains().empty())
            {
                auto firstToolchain = Toolchains::getToolchains()[0];
                firstToolchain->buildProject(currentEditorProject.m_pluginProject.m_pluginPath, "release-host");
            }
            else // TODO: MAKE WARNING DIALOG
            {

            }

            // TEST!!!!!

            /*VisualStudioToolchain testToolchain;
            testToolchain.setPath("F:/VisualStudio/IDE");
            testToolchain.m_archType = VCArchType::AMD64;

            testToolchain.buildProject(currentEditorProject.m_pluginProject.m_pluginPath, "release-host");*/

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
        }
    }
    else if(SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE))
    {
        m_projectName.clear();
        m_dirPath.clear();
        
        m_error = "";
        
        setActive(false);
    }
}
