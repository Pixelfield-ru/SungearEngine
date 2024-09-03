//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_STYLESMANAGER_H
#define SUNGEARENGINEEDITOR_STYLESMANAGER_H

#include <vector>
#include <SGCore/Main/CoreGlobals.h>
#include "SGCore/Utils/Utils.h"
#include "IStyle.h"

namespace SGE
{
    struct StylesManager
    {
        static void init() noexcept;
        
        SG_NOINLINE static void setCurrentStyle(const SGCore::Ref<IStyle>& style) noexcept;
        
        SG_NOINLINE static SGCore::Ref<IStyle> getCurrentStyle() noexcept;
        
        static SGCore::Ref<IStyle> getStyle(const std::string& name) noexcept;
        
        SG_NOINLINE static auto& getStyles() noexcept;
        
    private:
        static inline SGCore::Ref<IStyle> m_currentStyle;
        static inline std::vector<SGCore::Ref<IStyle>> m_styles;
    };
}

#endif //SUNGEARENGINEEDITOR_STYLESMANAGER_H
