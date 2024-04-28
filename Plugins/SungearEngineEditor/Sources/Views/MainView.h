//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_MAINVIEW_H
#define SUNGEARENGINEEDITOR_MAINVIEW_H

#include <SGCore/Main/CoreGlobals.h>
#include <imgui.h>
#include <SGCore/ImGuiWrap/Views/IView.h>

#include "TopToolbar/TopToolbarView.h"

namespace SGE
{
    struct MainView : SGCore::ImGuiWrap::IView
    {
        MainView();
        ~MainView();
        
        bool begin() override;
        void renderBody() override;
        void end() override;
        
    private:
        SGCore::Ref<TopToolbarView> m_topToolbarView;
    };
}

#endif //SUNGEARENGINEEDITOR_MAINVIEW_H
