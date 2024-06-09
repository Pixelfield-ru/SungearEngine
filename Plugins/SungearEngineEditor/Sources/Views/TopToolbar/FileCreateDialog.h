//
// Created by ilya on 28.04.24.
//

#ifndef SUNGEARENGINEEDITOR_FILECREATEDIALOG_H
#define SUNGEARENGINEEDITOR_FILECREATEDIALOG_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <glm/vec2.hpp>
#include "FileOpenMode.h"

namespace SGE
{
    struct FileCreateDialog : SGCore::ImGuiWrap::IView
    {
        FileOpenMode m_mode = FileOpenMode::CREATE;
        
        /**
         * Leave empty to use path where current executable file is located
         */
        std::string m_defaultPath;
        std::string m_ext;
        
        bool m_isCreatingDirectory = false;
        
        std::string m_currentChosenDirPath;
        
        std::string m_dialogTitle = "Create File";
        
        bool begin() final;
        void renderBody() final;
        void end() final;
        
        void onActiveChangedListener() override;
        
    private:
        std::string m_fileName;
        
        std::string m_error;
        
        glm::vec2 m_size { 440, 85 };
    };
}

#endif //SUNGEARENGINEEDITOR_FILECREATEDIALOG_H
