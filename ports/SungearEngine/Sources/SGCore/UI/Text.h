//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_TEXT_H
#define SUNGEARENGINE_TEXT_H

#include <string>
#include <vector>
#include <glm/vec4.hpp>
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Memory/Assets/FontSpecialization.h"

namespace SGCore
{
    struct Font;
    
    struct Text
    {
        std::u16string m_text;
        
        // todo: move in style
        glm::vec4 m_color = { 1.0, 1.0, 1.0, 1.0 };
        
        Weak<Font> m_usedFont;
        FontSpecializationSettings m_fontSettings;
    };
}

#endif //SUNGEARENGINE_TEXT_H
