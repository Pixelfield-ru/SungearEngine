//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_ISTYLE_H
#define SUNGEARENGINEEDITOR_ISTYLE_H

#include <cstdint>
#include <string>
#include <SGCore/Graphics/API/ITexture2D.h>

namespace SGE
{
    struct IStyle
    {
        friend struct StylesManager;
        
        std::string m_name;
        
    private:
        virtual void apply() = 0;
    };
}

#endif //SUNGEARENGINEEDITOR_ISTYLE_H
