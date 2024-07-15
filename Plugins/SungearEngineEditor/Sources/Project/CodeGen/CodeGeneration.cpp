//
// Created by Ilya on 12.07.2024.
//

#include "CodeGeneration.h"
#include "SungearEngineEditor.h"
#include <SGCore/Utils/Formatter.h>
#include <SGCore/Scene/Scene.h>

void SGE::CodeGeneration::generateCode(const std::vector<SGCore::AnnotationsProcessor>& annotationsProcessors,
                                       const std::filesystem::path& toDirectory) noexcept
{
    // MAKING includes ===========================
    std::string includes;
    includes += "#include \"" + SungearEngineEditor::getInstance()->getLocalPath() + "/Sources/Views/InspectorView.h\"\n";
    includes += "#include \"" + SungearEngineEditor::getInstance()->getLocalPath() + "/Sources/SungearEngineEditor.h\"\n";
    // ===========================================

    // MAKING onInspectorViewComponentsChooseRenderFunctionCode =====
    std::string onInspectorViewComponentsChooseRenderFunctionCode;

    for(const auto& annotationsProcessor : annotationsProcessors)
    {
        for(const auto& annotation : annotationsProcessor.getAnnotations())
        {
            if(annotation.m_name == "sg_struct")
            {
                auto structTypeIt = annotation.m_currentArgs.find("type");
                if(structTypeIt != annotation.m_currentArgs.end())
                {
                    if(std::any_cast<std::string>(structTypeIt->second.m_values[0]) == "component")
                    {
                        auto structName = annotation.m_currentArgs.find("fullName");
                        if(structName != annotation.m_currentArgs.end())
                        {
                            std::string structNameStr = std::any_cast<std::string>(structName->second.m_values[0]);
                            onInspectorViewComponentsChooseRenderFunctionCode += fmt::format("\t\tImGui::Text(\"{0}\");\n", structNameStr);
                        }
                    }
                }
            }
        }
    }
    // ==============================================================

    SGCore::Formatter formatter;
    formatter["includes"] = includes;
    formatter["onInspectorViewComponentsChooseRenderFunctionCode"] = onInspectorViewComponentsChooseRenderFunctionCode;

    // ENTRY POINT GENERATION =============
    std::string generatedCodeEntryHeader = SGCore::FileUtils::readFile(
            SungearEngineEditor::getInstance()->getLocalPath() +
            "/Sources/Project/CodeGen/.references/EditorOnly/GeneratedCodeEntry.h");

    generatedCodeEntryHeader = formatter.format(generatedCodeEntryHeader);

    auto generatedCodeEntryHeaderPath = toDirectory;
    generatedCodeEntryHeaderPath += "/.SG_GENERATED/EditorOnly/GeneratedCodeEntry.h";
    SGCore::FileUtils::writeToFile(generatedCodeEntryHeaderPath, generatedCodeEntryHeader, false, true);
    // ====================================
    // ENTRY POINT GENERATION =============
    std::string generatedCodeEntryCpp = SGCore::FileUtils::readFile(
            SungearEngineEditor::getInstance()->getLocalPath() +
            "/Sources/Project/CodeGen/.references/EditorOnly/GeneratedCodeEntry.cpp");

    generatedCodeEntryCpp = formatter.format(generatedCodeEntryCpp);

    auto generatedCodeEntryCppPath = toDirectory;
    generatedCodeEntryCppPath += "/.SG_GENERATED/EditorOnly/GeneratedCodeEntry.cpp";
    SGCore::FileUtils::writeToFile(generatedCodeEntryCppPath, generatedCodeEntryCpp, false, true);
    // ====================================
}
