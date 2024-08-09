//
// Created by stuka on 28.12.2023.
//

#ifndef SGSLINTERPRETER_SGSLBRACKETSLISTENER_H
#define SGSLINTERPRETER_SGSLBRACKETSLISTENER_H

#include <SGCore/pch.h>

namespace SGCore
{
    struct SGSLEBracketsListener
    {
        size_t m_openedBracketsCount = 0;

        std::function<void()> m_onBlockEnd;
    };
}

#endif //SGSLINTERPRETER_SGSLBRACKETSLISTENER_H
