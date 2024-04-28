//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
#define SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include "FileCreateDialog.h"

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
    };
}

#endif //SUNGEARENGINEEDITOR_TOPTOOLBARVIEW_H
