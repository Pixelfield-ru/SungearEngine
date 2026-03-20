//
// Created by Ilya on 08.02.2024.
//

#pragma once

#include <string>
#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT SGSLETranslatorConfiguration
    {
        bool m_useOutputDebug = true;
        std::string m_outputDebugDirectoryPath = "SGSLETranslatorOutputDebug";
        bool m_autoPadding = false;
    };
}
