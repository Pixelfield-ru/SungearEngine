//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_PROJECTCREATEDIALOG_H
#define SUNGEARENGINEEDITOR_PROJECTCREATEDIALOG_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <glm/vec2.hpp>
#include "FileOpenMode.h"

namespace SGE
{
    struct ProjectCreateDialog : SGCore::ImGuiWrap::IView
    {
        FileOpenMode m_mode = FileOpenMode::CREATE;
        
        void renderBody() override;
        
    private:
        int m_currentSelectedCPPStandard = 0;
        
        std::string m_dirPath;
        std::string m_projectName;
        
        std::string m_error;

        ImVec2 m_size { };
    };
}

#endif //SUNGEARENGINEEDITOR_PROJECTCREATEDIALOG_H
