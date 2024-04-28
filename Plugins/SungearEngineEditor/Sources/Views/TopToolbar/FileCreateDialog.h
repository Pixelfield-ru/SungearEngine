//
// Created by ilya on 28.04.24.
//

#ifndef SUNGEARENGINEEDITOR_FILECREATEDIALOG_H
#define SUNGEARENGINEEDITOR_FILECREATEDIALOG_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <glm/vec2.hpp>

namespace SGE
{
    struct FileCreateDialog : SGCore::ImGuiWrap::IView
    {
        FileCreateDialog();
        
        bool begin() final;
        void renderBody() final;
        void end() final;
        
        void onActiveChangedListener() override;
        
    private:
        std::string m_dirPath;
        std::string m_fileName;
        
        glm::vec2 m_size { 350, 85 };
    };
}

#endif //SUNGEARENGINEEDITOR_FILECREATEDIALOG_H
