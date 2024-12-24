//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_PROJECTCREATEDIALOG_H
#define SUNGEARENGINEEDITOR_PROJECTCREATEDIALOG_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <glm/vec2.hpp>
#include "FileOpenMode.h"
#include "Views/Window.h"

namespace SGE
{
    struct ProjectCreateDialog : Window
    {
        ProjectCreateDialog() noexcept;
        ProjectCreateDialog(const ProjectCreateDialog&) = default;
        ProjectCreateDialog(ProjectCreateDialog&&) = default;

        FileOpenMode m_mode = FileOpenMode::CREATE;
        
        void renderBody() override;
        void footerRender() override;

        void submit();
        std::string m_dirPath;
    private:
        void cancel();

        static inline const char* m_cppStandards[] = { /*"C++98", "C++03", "C++11", "C++14", "C++17",*/ "C++20", "C++23" };
        static inline size_t m_cppStandardsCount = 2;

        int m_currentSelectedCPPStandard = 0;

        std::string m_projectName;
        
        std::string m_error;

        ImVec2 m_size { };
    };
}

#endif //SUNGEARENGINEEDITOR_PROJECTCREATEDIALOG_H
