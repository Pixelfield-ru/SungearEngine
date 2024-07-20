//
// Created by ilya on 28.04.24.
//

#ifndef SUNGEARENGINEEDITOR_FILECREATEDIALOG_H
#define SUNGEARENGINEEDITOR_FILECREATEDIALOG_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <glm/vec2.hpp>
#include "FileOpenMode.h"
#include "Views/Window.h"

namespace SGE
{
    struct FileCreateDialog : Window
    {
        FileCreateDialog();
        FileCreateDialog(const FileCreateDialog&) = default;
        FileCreateDialog(FileCreateDialog&&) = default;

        FileOpenMode m_mode = FileOpenMode::CREATE;
        
        SGCore::Event<void(const std::filesystem::path& byPath, bool canceled)> onOperationEnd;
        
        /**
         * Leave empty to use path where current executable file is located
         */
        std::string m_defaultPath;
        std::string m_ext;
        
        bool m_isCreatingDirectory = false;
        
        std::string m_currentChosenDirPath;
        
        std::string m_dialogTitle = "Create File";

        void renderBody() final;
        void footerRender() override;

        void onActiveChangedListener() override;
        
    private:
        void submit();
        void cancel();

        std::string m_fileName;
        
        std::string m_error;
        
        glm::vec2 m_size { 440, 85 };
    };
}

#endif //SUNGEARENGINEEDITOR_FILECREATEDIALOG_H
