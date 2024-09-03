//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DARK_H
#define SUNGEARENGINEEDITOR_DARK_H

#include "Styles/IStyle.h"

namespace SGE::EditorStyles
{
    struct Dark : IStyle
    {
        Dark()
        {
            m_name = "Dark";
        }

    protected:
        void apply() override;
    };
}

#endif //SUNGEARENGINEEDITOR_DARK_H
