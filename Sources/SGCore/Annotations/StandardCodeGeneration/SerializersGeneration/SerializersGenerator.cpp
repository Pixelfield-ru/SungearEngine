//
// Created by ilya on 03.07.24.
//

#include "SerializersGenerator.h"
#include "SGCore/Utils/Formatter.h"
#include "SGCore/Annotations/AnnotationsProcessor.h"

namespace fs = std::filesystem;

std::string
SGCore::CodeGen::SerializersGenerator::generateSerializers(const SGCore::AnnotationsProcessor& annotationsProcessor,
                                                           const std::filesystem::path& generatedHeaderPath) const
{
    const char* sgSourcesRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    
    if(!sgSourcesRoot || !std::filesystem::exists(std::string(sgSourcesRoot) + "/cmake/SungearEngineInclude.cmake"))
    {
        return "Error: Incorrect Sungear Engine sources directory!\nCheck if the 'SUNGEAR_SOURCES_ROOT' environment variable\nexists and correct.";
    }
    
    const std::string sgSourcesRootStr = sgSourcesRoot;
    
    fs::create_directories(generatedHeaderPath.parent_path());
    
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
                
                auto structureFullName = std::any_cast<std::string>(fullNameArg->second.m_values[0]);
                auto namespaceType = std::any_cast<std::string>(fullNameArg->second.m_values[1]);
                
                // not 'struct' or 'class'
                if(namespaceType == "namespace" || alreadyGeneratedStructs.contains(structureFullName)) continue;
                
                std::string templateArgs;
                std::string templateNames;
                
                auto templateArgIt = annotation.m_currentArgs.find("template");
                bool templateArgProvided = templateArgIt != annotation.m_currentArgs.end();
                if(templateArgProvided)
                {
                    std::int64_t templateArgsCnt = 0;
                    for(const auto& templateArg : templateArgIt->second.m_values)
                    {
                        auto* templateValAsSubAnnotation = std::any_cast<AnnotationsProcessor::Annotation>(&templateArg);
                        
                        if(templateValAsSubAnnotation)
                        {
                            auto templateArgTypeIt = templateValAsSubAnnotation->m_currentArgs.find("type");
                            if(templateArgTypeIt == templateValAsSubAnnotation->m_currentArgs.end())
                            {
                                throw std::runtime_error(fmt::format(
                                        "Can not generate serializer code for struct '{0}'. Template argument was provided in 'sg_struct' annotation, but type of this argument was not specified.",
                                        structureFullName));
                            }
                            
                            templateArgs += std::any_cast<std::string>(templateArgTypeIt->second.m_values[0]);
                            
                            auto templateArgNameIt = templateValAsSubAnnotation->m_currentArgs.find("name");
                            if(templateArgNameIt != templateValAsSubAnnotation->m_currentArgs.end())
                            {
                                if(templateArgsCnt < templateArgIt->second.m_values.size() - 2)
                                {
                                    templateArgs += " " + std::any_cast<std::string>(templateArgNameIt->second.m_values[0]) + ", ";
                                    templateNames += std::any_cast<std::string>(templateArgNameIt->second.m_values[0]) + ", ";
                                }
                                else
                                {
                                    templateArgs += " " + std::any_cast<std::string>(templateArgNameIt->second.m_values[0]);
                                    templateNames += std::any_cast<std::string>(templateArgNameIt->second.m_values[0]);
                                }
                            }
                        }
                        
                        ++templateArgsCnt;
                    }
                    
                    formatter["structureTemplateArgs"] = templateArgs;
                    formatter["structureTemplateNames"] = templateNames;
                }
                
                formatter["structureName"] = structureFullName;
                
                serializersSpecForwardDeclCode += fmt::format("// SERIALIZER FORWARD DECL FOR {0} '{1}'\n", namespaceType, structureFullName);
                serializersSpecForwardDeclCode += "#include \"" + Utils::toUTF8<char16_t>(annotation.m_filePath.u16string()) + "\"\n";
                serializersSpecForwardDeclCode += formatter.format(
                        FileUtils::readFile(
                                sgSourcesRootStr +
                                (templateArgProvided ?
                                 "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/.references/TemplatedSerializerSpecForwardDecl.h"
                                                     :
                                 "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/.references/NotTemplatedSerializerSpecForwardDecl.h")) +
                        "\n");
                serializersSpecForwardDeclCode += "// =================================================================================\n";
                
                std::string serializationOps;
                
                // adding values to serialize. adding members of struct
                // searching for all values that belong to structName
                serializationOps = generateSerializationOps(annotationsProcessor, structureFullName);
                // ==============================================================
                
                formatter["valuesAddCode"] = serializationOps;
                
                serializersSpecImplCode += fmt::format("// SERIALIZER IMPLEMENTATION FOR {0} '{1}'\n", namespaceType, structureFullName);
                serializersSpecImplCode += formatter.format(
                        FileUtils::readFile(
                                sgSourcesRootStr +
                                (templateArgProvided ?
                                 "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/.references/TemplatedSerializerSpecImpl.cpp"
                                                     :
                                 "/Sources/SGCore/Annotations/StandardCodeGeneration/SerializersGeneration/.references/NotTemplatedSerializerSpecImpl.cpp"))) + "\n";
                serializersSpecImplCode += "// =================================================================================\n";
                
                alreadyGeneratedStructs.insert(structureFullName);
            }
        }
    }

    std::ofstream serializersH(generatedHeaderPath);
    serializersH << "#include <SGCore/Scene/Serializer.h>\n";
    serializersH << serializersSpecForwardDeclCode;

    std::filesystem::path serializersCppPath = generatedHeaderPath.parent_path();
    serializersCppPath += "/";
    serializersCppPath += generatedHeaderPath.stem();
    serializersCppPath += ".cpp";

    std::ofstream serializersCpp(serializersCppPath);
    serializersCpp << "#include \"" + Utils::toUTF8<char16_t>(generatedHeaderPath.u16string()) + "\"\n";
    serializersCpp << serializersSpecImplCode;
    
    // no error
    return "";
}

std::string SGCore::CodeGen::SerializersGenerator::generateSerializationOps(
        const SGCore::AnnotationsProcessor& annotationsProcessor, const std::string& currentStructName) const noexcept
{
    std::string serializationOps;

    for(const auto& annotation : annotationsProcessor.getAnnotations())
    {
        if(annotation.m_name == "sg_struct")
        {
            auto fullNameArg = annotation.m_currentArgs.find("fullName");

            if (fullNameArg == annotation.m_currentArgs.end() ||
                std::any_cast<std::string>(fullNameArg->second.m_values[0]) != currentStructName)
            {
                continue;
            }

            auto extendsArg = annotation.m_currentArgs.find("extends");
            if(extendsArg != annotation.m_currentArgs.end())
            {
                for(const auto& val : extendsArg->second.m_values)
                {
                    serializationOps += generateSerializationOps(annotationsProcessor, std::any_cast<std::string>(val));
                }
            }
        }

        if(annotation.m_name == "sg_member")
        {
            auto parentNamespaceArg = annotation.m_currentArgs.find("parentNamespace");
            if(parentNamespaceArg == annotation.m_currentArgs.end() ||
               parentNamespaceArg->second.m_values.empty() ||
               std::any_cast<std::string>(parentNamespaceArg->second.m_values[0]) != currentStructName)
            {
                continue;
            }

            auto varNameArg = annotation.m_currentArgs.find("varName");
            auto serializableNameArg = annotation.m_currentArgs.find("serializableName");
            if(varNameArg != annotation.m_currentArgs.end() &&
               serializableNameArg != annotation.m_currentArgs.end())
            {
                std::string variableGetter = std::any_cast<std::string>(varNameArg->second.m_values[0]);

                // iterating through functions annotations
                for(const auto& functionAnnotation : annotationsProcessor.getAnnotations())
                {
                    if(functionAnnotation.m_name == "sg_function")
                    {
                        auto functionParentNamespace = functionAnnotation.m_currentArgs.find("parentNamespace");
                        if(functionParentNamespace == functionAnnotation.m_currentArgs.end())
                        {
                            continue;
                        }

                        if (std::any_cast<std::string>(functionParentNamespace->second.m_values[0]) !=
                            currentStructName)
                        {
                            continue;
                        }

                        auto functionGetterForArgument = functionAnnotation.m_currentArgs.find(
                                "getterFor");
                        if (functionGetterForArgument == functionAnnotation.m_currentArgs.end())
                        {
                            continue;
                        }

                        if (std::any_cast<std::string>(
                                functionGetterForArgument->second.m_values[0]) != variableGetter)
                        {
                            continue;
                        }

                        auto functionNameArgument = functionAnnotation.m_currentArgs.find("name");

                        if (functionNameArgument == functionAnnotation.m_currentArgs.end())
                        {
                            continue;
                        }

                        variableGetter = std::any_cast<std::string>(functionNameArgument->second.m_values[0]) + "()";
                    }
                }

                serializationOps += fmt::format(
                        "\tSGCore::Serializer::serialize(toDocument, v, \"{0}\", value.{1});\n",
                        std::any_cast<std::string>(serializableNameArg->second.m_values[0]),
                        variableGetter);
            }
        }
    }

    return serializationOps;
}
