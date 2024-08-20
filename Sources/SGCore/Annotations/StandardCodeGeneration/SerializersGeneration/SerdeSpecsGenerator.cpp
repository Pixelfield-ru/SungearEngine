//
// Created by ilya on 03.07.24.
//

#include "SerdeSpecsGenerator.h"
#include "SGCore/Utils/Formatter.h"
#include "SGCore/Annotations/AnnotationsProcessor.h"

namespace fs = std::filesystem;

std::string
SGCore::CodeGen::SerdeSpecsGenerator::generateSerializers(const SGCore::AnnotationsProcessor& annotationsProcessor,
                                                          const std::filesystem::path& generatedHeaderPath)
{
    const char* sgSourcesRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    
    if(!sgSourcesRoot || !std::filesystem::exists(std::string(sgSourcesRoot) + "/cmake/SungearEngineInclude.cmake"))
    {
        return "Error: Incorrect Sungear Engine sources directory!\nCheck if the 'SUNGEAR_SOURCES_ROOT' environment variable\nexists and correct.";
    }

    m_structsBaseTypes.clear();
    m_structsDerivedTypes.clear();

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
                                return fmt::format(
                                        "Can not generate serializer code for struct '{0}'. Template argument was provided in 'sg_struct' annotation, but type of this argument was not specified.",
                                        structureFullName);
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

                // ==============================================================

                std::string serializationOps;
                std::string deserializationOps;

                // adding values to serialize and deserialize. adding members of struct
                // searching for all values that belong to structName
                std::string analyzerError;
                analyzeStruct(annotationsProcessor, structureFullName, serializationOps, deserializationOps,
                              analyzerError);
                if(!analyzerError.empty())
                {
                    return "Can not generate serializers: " + analyzerError;
                }
                // ==============================================================

                formatter["valuesSerializeCode"] = serializationOps;
                formatter["valuesDeserializeCode"] = deserializationOps;

                // ==============================================================
                // generating base and derived types
                std::string baseTypes;
                std::string derivedTypes;

                size_t currentBaseTypeIdx = 0;
                const auto& currentStructBaseTypes = m_structsBaseTypes[structureFullName];
                for(const auto& baseType : currentStructBaseTypes)
                {
                    baseTypes += baseType;
                    if(currentBaseTypeIdx != currentStructBaseTypes.size() - 1)
                    {
                        baseTypes += ", ";
                    }

                    ++currentBaseTypeIdx;
                }

                size_t currentDerivedTypeIdx = 0;
                const auto& currentStructDerivedTypes = m_structsDerivedTypes[structureFullName];
                for(const auto& derivedType : currentStructDerivedTypes)
                {
                    derivedTypes += derivedType;
                    if(currentDerivedTypeIdx != currentStructDerivedTypes.size() - 1)
                    {
                        derivedTypes += ", ";
                    }

                    ++currentDerivedTypeIdx;
                }

                formatter["structureBaseTypes"] = baseTypes;
                formatter["structureDerivedTypes"] = derivedTypes;

                // ==============================================================
                
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

                // ==============================================================

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
    serializersH << "// GENERATED BY Sungear Engine SerdeSpecsGenerator\n\n";
    serializersH << "#pragma once\n\n";
    serializersH << "#include <SGCore/Scene/Serializer.h>\n";
    serializersH << serializersSpecForwardDeclCode;
    serializersH << serializersSpecImplCode;
    
    // no error
    return "";
}

void SGCore::CodeGen::SerdeSpecsGenerator::analyzeStruct(
        const AnnotationsProcessor& annotationsProcessor,
        const std::string& currentStructName,
        std::string& serializationOps,
        std::string& deserializationOps,
        std::string& outputLog) noexcept
{
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
            if (extendsArg != annotation.m_currentArgs.end())
            {
                for (const auto& val: extendsArg->second.m_values)
                {
                    const std::string baseTypeName = std::any_cast<std::string>(val);
                    m_structsBaseTypes[currentStructName].push_back(baseTypeName);
                    m_structsDerivedTypes[baseTypeName].push_back(currentStructName);
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
                // first just assigning variable name as getter (direct access to member)
                std::string variableGetter = std::any_cast<std::string>(varNameArg->second.m_values[0]);
                std::string variableSetter = std::any_cast<std::string>(varNameArg->second.m_values[0]);

                bool isVariableSetterFound = false;
                bool isVariableGetterFound = false;

                // iterating through functions annotations
                for(const auto& functionAnnotation : annotationsProcessor.getAnnotations())
                {
                    if(functionAnnotation.m_name == "sg_function")
                    {
                        // if sg_functions annotation doest not have argument 'parentNamespace'
                        auto functionParentNamespace = functionAnnotation.m_currentArgs.find("parentNamespace");
                        if(functionParentNamespace == functionAnnotation.m_currentArgs.end())
                        {
                            continue;
                        }

                        // if annotation is not in struct %currentStructName%
                        if (std::any_cast<std::string>(functionParentNamespace->second.m_values[0]) !=
                            currentStructName)
                        {
                            continue;
                        }

                        // if sg_functions annotation doest not have argument 'name'
                        auto functionNameArgument = functionAnnotation.m_currentArgs.find("name");
                        if (functionNameArgument == functionAnnotation.m_currentArgs.end())
                        {
                            continue;
                        }

                        auto functionGetterForArgument = functionAnnotation.m_currentArgs.find(
                                "getterFor");
                        if (functionGetterForArgument != functionAnnotation.m_currentArgs.end())
                        {
                            // check if this is getter for %variableGetter% variable (variableGetter contains name of variable first)
                            if (std::any_cast<std::string>(
                                    functionGetterForArgument->second.m_values[0]) != variableGetter)
                            {
                                continue;
                            }

                            variableGetter = std::any_cast<std::string>(functionNameArgument->second.m_values[0]) + "()";
                            isVariableGetterFound = true;

                            continue;
                        }

                        auto functionSetterForArgument = functionAnnotation.m_currentArgs.find(
                                "setterFor");
                        if (functionSetterForArgument != functionAnnotation.m_currentArgs.end())
                        {
                            // check if this is setter for %variableGetter% variable (variableSetter contains name of variable first)
                            if (std::any_cast<std::string>(
                                    functionSetterForArgument->second.m_values[0]) != variableSetter)
                            {
                                continue;
                            }

                            variableSetter = std::any_cast<std::string>(functionNameArgument->second.m_values[0]);
                            isVariableSetterFound = true;
                        }
                    }
                }

                /*serializationOps += fmt::format(
                        "\tSGCore::Serializer::serialize(toDocument, v, \"{0}\", value.{1});\n",
                        std::any_cast<std::string>(serializableNameArg->second.m_values[0]),
                        variableGetter);*/

                const std::string memberVariableName = std::any_cast<std::string>(serializableNameArg->second.m_values[0]);

                serializationOps += fmt::format(
                        "\tvalueView.getValueContainer().addMember(\"{0}\", valueView.m_data->{1});\n",
                        memberVariableName,
                        variableGetter);

                // generating code to deserialize value and assign to member variable through direct access
                if(!isVariableSetterFound && !isVariableGetterFound)
                {
                    deserializationOps += "\tconst auto ";
                    deserializationOps += memberVariableName;
                    deserializationOps += " = valueView.getValueContainer().template getMember<decltype(valueView.m_data->";
                    deserializationOps += memberVariableName;
                    deserializationOps += ")>(\"" + memberVariableName + "\");\n";
                    deserializationOps += "\tif(" + memberVariableName + ")\n";
                    deserializationOps += "\t{\n\t\tvalueView.m_data->";
                    deserializationOps += memberVariableName;
                    deserializationOps += " = *" + memberVariableName + ";\n\t}\n";
                }
                else if(isVariableSetterFound && isVariableGetterFound)
                {
                    deserializationOps += "\tconst auto ";
                    deserializationOps += memberVariableName;
                    deserializationOps += " = valueView.getValueContainer().template getMember<std::remove_reference_t<std::remove_const_t<decltype(valueView.m_data->";
                    deserializationOps += variableGetter;
                    deserializationOps += ")>>>(\"" + memberVariableName + "\");\n";
                    deserializationOps += "\tif(" + memberVariableName + ")\n";
                    deserializationOps += "\t{\n\t\tvalueView.m_data->";
                    deserializationOps += variableSetter + "(";
                    deserializationOps += "*" + memberVariableName + ");\n\t}\n";
                }
                else
                {
                    // generating error
                    outputLog = "Can not generate deserialize code for struct '" + currentStructName +
                                "' and for member '" + memberVariableName + "': getter or setter was not provided.";
                }
            }
        }
    }
}
