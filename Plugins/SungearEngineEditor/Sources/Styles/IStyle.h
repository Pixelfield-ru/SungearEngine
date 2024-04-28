//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_ISTYLE_H
#define SUNGEARENGINEEDITOR_ISTYLE_H

#include <string>

namespace SGE
{
    struct IStyle
    {
        std::string m_name;
        virtual void apply() = 0;
    };
}

#endif //SUNGEARENGINEEDITOR_ISTYLE_H
