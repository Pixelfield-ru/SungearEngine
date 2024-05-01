//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
#define SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "FileCreateDialog.h"
#include "ProjectCreateDialog.h"

namespace SGE
{
    struct TopToolbarView : public SGCore::ImGuiWrap::IView
    {
        TopToolbarView();
        
        bool begin() final;
        void renderBody() final;
        void end() final;
        
    private:
        SGCore::Ref<FileCreateDialog> m_fileCreateDialog = SGCore::MakeRef<FileCreateDialog>();
        SGCore::Ref<ProjectCreateDialog> m_projectCreateDialog = SGCore::MakeRef<ProjectCreateDialog>();
    };
}

#endif //SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
