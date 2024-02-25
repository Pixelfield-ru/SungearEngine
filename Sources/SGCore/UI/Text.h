//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_TEXT_H
#define SUNGEARENGINE_TEXT_H

#include <string>
#include <vector>
#include "glm/vec4.hpp"

namespace SGCore
{
    struct Text
    {
        std::u16string m_text;
        
        // todo: move in style
        glm::vec4 m_color = { 1.0, 1.0, 1.0, 1.0 };
    };
}

#endif //SUNGEARENGINE_TEXT_H
