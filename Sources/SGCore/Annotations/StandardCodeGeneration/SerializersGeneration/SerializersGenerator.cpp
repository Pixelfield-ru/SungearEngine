//
// Created by ilya on 03.07.24.
//

#include "SerializersGenerator.h"
#include "SGCore/Utils/Formatter.h"

namespace fs = std::filesystem;

std::string
SGCore::CodeGen::SerializersGenerator::generateSerializers(const SGCore::AnnotationsProcessor& annotationsProcessor,
                                                           const std::filesystem::path& toPath) const noexcept
{
    const char* sgSourcesRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    
    if(!sgSourcesRoot || !std::filesystem::exists(std::string(sgSourcesRoot) + "/cmake/SungearEngineInclude.cmake"))
    {
        return "Error: Incorrect Sungear Engine sources directory!\nCheck if the 'SUNGEAR_SOURCES_ROOT' environment variable\nexists and correct.";
    }
    
    const std::string sgSourcesRootStr = sgSourcesRoot;
    const std::string generatedCodeDir = Utils::toUTF8<char16_t>(toPath.u16string()) + "/.generated";
    
    fs::create_directories(generatedCodeDir);
    
    std::string serializersSpecForwardDeclCode;
    std::string serializersSpecImplCode;
    
    Formatter formatter;
    
    std::unordered_set<std::string> alreadyGeneratedStructs;
    
    for(const auto& annotation : annotationsProcessor.getAnnotations())
    {
        if(annotation.m_name == "sg_struct")
        {
            auto fullNameArg = annotation.m_currentArgs.find("fullName");
            if(fullNameArg != annotation.m_currentArgs.end())
            {
                if(fullNameArg->second.m_values.size() < 2) continue;
                
                std::string structName = fullNameArg->second.m_values[0];
                std::string namespaceType = fullNameArg->second.m_values[1];
                
                // not 'struct' or 'class'
                if(namespaceType == "namespace" || alreadyGeneratedStructs.contains(structName)) continue;
                
                formatter["structureName"] = structName;
                
                serializersSpecForwardDeclCode += fmt::format("// SERIALIZER FORWARD DECL FOR {0} '{1}'\n", namespaceType, structName);
                serializersSpecForwardDeclCode += "#include \"" + Utils::toUTF8<char16_t>(annotation.m_filePath.u16string()) + "\"\n";
                serializersSpecForwardDeclCode += formatter.format(FileUtils::readFile(sgSourcesRootStr +
                        "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/.references/SerializerSpecForwardDecl.h")) + "\n";
                serializersSpecForwardDeclCode += "// =================================================================================\n";
                
                std::string serializationOps;
                
                // adding values to serialize. adding members of struct
                // searching for all values that belong to structName
                for(const auto& memberAnnotation : annotationsProcessor.getAnnotations())
                {
                    if(memberAnnotation.m_name == "sg_member")
                    {
                        auto parentNamespaceArg = memberAnnotation.m_currentArgs.find("parentNamespace");
                        if(parentNamespaceArg == memberAnnotation.m_currentArgs.end() ||
                           parentNamespaceArg->second.m_values.empty() ||
                           parentNamespaceArg->second.m_values[0] != structName)
                        {
                            continue;
                        }
                        
                        auto varNameArg = memberAnnotation.m_currentArgs.find("varName");
                        auto serializableNameArg = memberAnnotation.m_currentArgs.find("serializableName");
                        if(varNameArg != memberAnnotation.m_currentArgs.end() &&
                           serializableNameArg != memberAnnotation.m_currentArgs.end())
                        {
                            serializationOps += fmt::format("\tSGCore::Serializer::serialize(toDocument, v, \"{0}\", value.{1});\n",
                                                            serializableNameArg->second.m_values[0], varNameArg->second.m_values[0]);
                        }
                    }
                }
                // ==============================================================
                
                formatter["valuesAddCode"] = serializationOps;
                
                serializersSpecImplCode += fmt::format("// SERIALIZER IMPLEMENTATION FOR {0} '{1}'\n", namespaceType, structName);
                serializersSpecImplCode += formatter.format(FileUtils::readFile(sgSourcesRootStr +
                        "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/.references/SerializerSpecImpl.h")) + "\n";
                serializersSpecImplCode += "// =================================================================================\n";
                
                alreadyGeneratedStructs.insert(structName);
            }
        }
    }
    
    std::ofstream serializersH(generatedCodeDir + "/Serializers.h");
    serializersH << "#include <SGCore/Scene/Serializer.h>\n";
    serializersH << serializersSpecForwardDeclCode;
    serializersH << serializersSpecImplCode;
    
    // no error
    return "";
}
