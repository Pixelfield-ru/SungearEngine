//
// Created by ilya on 23.02.24.
//

#ifndef SUNGEARENGINE_FONTSMANAGER_H
#define SUNGEARENGINE_FONTSMANAGER_H

#include <SGCore/pch.h>

#include <sgcore_export.h>

namespace SGCore
{
    struct SGCORE_EXPORT FontsManager
    {
        static void init() noexcept;
        
        static FT_Library getFTLibrary() noexcept;
    private:
        static inline FT_Library m_FTLib = nullptr;
    };
}

#endif //SUNGEARENGINE_FONTSMANAGER_H
