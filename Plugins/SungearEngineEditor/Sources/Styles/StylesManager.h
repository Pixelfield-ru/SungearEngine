//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_STYLESMANAGER_H
#define SUNGEARENGINEEDITOR_STYLESMANAGER_H

#include <vector>
#include <SGCore/Main/CoreGlobals.h>
#include <SGUtils/Utils.h>

#include "IStyle.h"
#include "Styles/Editor/Dark.h"

namespace SGE
{
    struct StylesManager
    {
        static void init() noexcept
        {
            SGCore::Ref<EditorStyles::Dark> darkStyle = SGCore::MakeRef<EditorStyles::Dark>();
            darkStyle->apply();
            
            m_styles.push_back(darkStyle);
        }
        
        static SGCore::Ref<IStyle> getStyle(const std::string& name) noexcept
        {
            auto foundIt = std::find_if(m_styles.begin(), m_styles.end(), [&name](const SGCore::Ref<IStyle>& style) {
                return style->m_name == name;
            });
            
            return foundIt != m_styles.end() ? *foundIt : nullptr;
        }
        
        SG_NOINLINE static auto& getStyles() noexcept
        {
            return m_styles;
        }
        
    private:
        static inline std::vector<SGCore::Ref<IStyle>> m_styles;
    };
}

#endif //SUNGEARENGINEEDITOR_STYLESMANAGER_H
