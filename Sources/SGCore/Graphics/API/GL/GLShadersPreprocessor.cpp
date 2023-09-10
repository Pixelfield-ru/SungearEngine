//
// Created by Ilya on 10.09.2023.
//

#include <sstream>
#include <regex>
#include <filesystem>

#include "GLShadersPreprocessor.h"

#include "SGCore/Logging/Log.h"

#include "SGCore/Memory/AssetManager.h"
#include "SGCore/Memory/Assets/FileAsset.h"

std::string Core::Graphics::GLShadersPreprocessor::processShader(const std::string& shaderPath,
                                                                 const std::string& inputCode,
                                                                 std::string& outError)
{
    std::string processedShaderCode;

    std::istringstream codeStream(inputCode);

    std::string line;

    // regex for preprocessor directives
    std::regex preprocDirectivesRegex("#[a-z_]+");

    // regex for paths
    std::regex pathsRegex(R"("[a-z\\\/._ ]+")");

    auto endedIter = std::sregex_token_iterator();

    size_t curLine = 1;

    while(std::getline(codeStream, line))
    {
        // process #sg_include directive
        if(line.starts_with(std::string("#") + SG_SHADER_INCLUDE_KEYWORD))
        {
            auto pathsBegin = std::sregex_token_iterator(line.begin(), line.end(), pathsRegex);

            size_t dist = std::distance(pathsBegin, endedIter);

            // if trying to multiple include by one #sg_include directive
            if(dist > 1)
            {
                outError += "Error in string " + std::to_string(curLine) + ": " +
                        std::string(SG_SHADER_MULTIPLE_SHADERS_INCLUDE_ERROR) + "\n";
            }
            else if(dist == 1)
            {
                std::string relativePath = pathsBegin->str();
                // path in file system
                std::filesystem::path shaderFSPath(shaderPath);

                // Transforming final of included shader. Example: from
                // shaders/pbr/default_shader.glsl/"../only_geom/default_shader.glsl"
                // to
                // shaders/pbr/../only_geom/default_shader.glsl
                std::string includedPath =
                        shaderFSPath.parent_path().string() + "/" + relativePath.substr(1, relativePath.size() - 2);

                if(!pathsBegin->matched || !std::filesystem::exists(includedPath)) // if path is not valid
                {
                    outError += "Error in string " + std::to_string(curLine) + ": " +
                                std::string(SG_SHADER_INCLUDE_INVALID_PATH) + ". Path: '" + includedPath + "'\n";
                }
                else // if path valid and file exists
                {
                    SGCF_SUCCESS(("Included shader path for shader '" + shaderPath + "' is: ").append(includedPath),
                                 SG_LOG_CURRENT_SESSION_FILE);

                    // loading code of included shader
                    auto includedShaderFile =
                            Memory::AssetManager::loadAsset<Memory::Assets::FileAsset>(includedPath);

                    // adding included shader code
                    processedShaderCode += "\n// ---------------- " + includedPath + " ----------------\n\n";
                    processedShaderCode += includedShaderFile->getData() + "\n";
                    processedShaderCode += "\n// ------------------------------------------------------\n\n";
                }
            }
            else
            {
                outError += "Error in string " + std::to_string(curLine) + ": " +
                            std::string(SG_SHADER_INCLUDE_NO_PATH) + "\n";
            }
        }
        else
        {
            processedShaderCode += line + "\n";
        }

        curLine++;
    }

    return processedShaderCode;
}
