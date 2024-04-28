//
// Created by ilya on 28.04.24.
//

#ifndef SUNGEARENGINEEDITOR_RESOURCES_H
#define SUNGEARENGINEEDITOR_RESOURCES_H

#include <vector>
#include <imgui.h>
#include <SGUtils/Utils.h>

namespace SGE
{
    struct Resources
    {
        static void releaseResources() noexcept;
        
        SG_NOINLINE static std::vector<ImFont*>& getImGuiFonts() noexcept;
        
    private:
        static inline std::vector<ImFont*> m_imguiFonts;
    };
}

#endif //SUNGEARENGINEEDITOR_RESOURCES_H
