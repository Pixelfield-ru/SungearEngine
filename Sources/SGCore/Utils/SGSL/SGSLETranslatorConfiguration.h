//
// Created by Ilya on 08.02.2024.
//

#ifndef SGSLETRANSLATOR_SGSLETRANSLATORCONFIGURATION_H
#define SGSLETRANSLATOR_SGSLETRANSLATORCONFIGURATION_H

#include <SGCore/pch.h>

namespace SGCore
{
    struct SGSLETranslatorConfiguration
    {
        bool m_useOutputDebug = true;
        std::string m_outputDebugDirectoryPath = "SGSLETranslatorOutputDebug";
        bool m_autoPadding = false;
    };
}

#endif //SGSLETRANSLATOR_SGSLETRANSLATORCONFIGURATION_H
