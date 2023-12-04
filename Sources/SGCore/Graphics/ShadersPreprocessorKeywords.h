//
// Created by Ilya on 10.09.2023.
//

#ifndef SUNGEARENGINE_SHADERSPREPROCESSORKEYWORDS_H
#define SUNGEARENGINE_SHADERSPREPROCESSORKEYWORDS_H

#include <string>

#define SG_SHADER_INCLUDE_KEYWORD   "include"

// errors ---------------------

#define SG_SHADER_NO_ERROR "No errors."
#define SG_SHADER_MULTIPLE_SHADERS_INCLUDE_ERROR "One #include directive tries to include more than one shader."
#define SG_SHADER_INCLUDE_INVALID_PATH "Incorrect path for including the shader with the #include directive."
#define SG_SHADER_INCLUDE_NO_PATH "The path is not specified after the #include directive.\
 It is possible that invalid characters are written in the path."

#endif //SUNGEARENGINE_SHADERSPREPROCESSORKEYWORDS_H
