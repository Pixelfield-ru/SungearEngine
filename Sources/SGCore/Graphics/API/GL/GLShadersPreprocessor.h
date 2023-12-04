//
// Created by Ilya on 10.09.2023.
//

#ifndef SUNGEARENGINE_GLSHADERSPREPROCESSOR_H
#define SUNGEARENGINE_GLSHADERSPREPROCESSOR_H

#include <string>

#include "SGCore/Graphics/ShadersPreprocessorKeywords.h"

namespace SGCore
{
    struct GLShadersPreprocessor
    {
        /**
         * @param shaderPath - Path to shader
         * @param inputCode - Input code of shader
         * @param outError - A string with an error description or if there are no errors, an empty string
         * @return Processed shader code
         */
        static std::string processShader(const std::string& shaderPath, const std::string& inputCode, std::string& outError);
    };
}


#endif //SUNGEARENGINE_GLSHADERSPREPROCESSOR_H
