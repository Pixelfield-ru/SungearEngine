//
// Created by ilya on 27.03.24.
//

#ifndef SUNGEARENGINEEDITOR_MAINVIEW_H
#define SUNGEARENGINEEDITOR_MAINVIEW_H

#include <SGCore/Main/CoreGlobals.h>
#include "TopToolbarView.h"

namespace SGE
{
    struct MainView
    {
        MainView();
        ~MainView();
        
    private:
        SGCore::Ref<TopToolbarView> m_topToolbarView;
    };
}

#endif //SUNGEARENGINEEDITOR_MAINVIEW_H
