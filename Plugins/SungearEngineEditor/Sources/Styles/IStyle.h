//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_ISTYLE_H
#define SUNGEARENGINEEDITOR_ISTYLE_H

#include <cstdint>
#include <string>
#include <SGCore/Graphics/API/ITexture2D.h>

namespace SGE
{
    enum IconType
    {
        FOLDER,
        FOLDER_MINUS,
        FOLDER_PLUS,
        FILE,
        FILE_TEXT,
        FILE_MINUS,
        FILE_PLUS,
        EYE,
        EYE_OFF,
        LAYERS,
        COPY,
        GIT_BRANCH,
        GIT_COMMIT,
        GIT_MERGE,
        GIT_PULL_REQUEST,
        EDIT1,
        EDIT2,
        EDIT3,
        DOWNLOAD,
        DOWNLOAD_CLOUD,
        FILTER,
        CPU,
        CODE,
        CLIPBOARD,
        CHECK,
        CHEVRON_DOWN,
        CHEVRON_LEFT,
        CHEVRON_RIGHT,
        CHEVRON_UP,
        CHEVRONS_DOWN,
        CHEVRONS_LEFT,
        CHEVRONS_RIGHT,
        CHEVRONS_UP,
        CLOCK,
        BATTERY,
        ARROW_UP_RIGHT,
        ARROW_UP_LEFT,
        ARROW_RIGHT,
        ARROW_LEFT,
        ARROW_DOWN_RIGHT,
        ARROW_DOWN_LEFT,
        ARROW_DOWN,
        ARROW_UP,
        ALERT_TRIANGLE,
        ALERT_OCTAGON,
        ALERT_CIRCLE,
        ALIGN_LEFT,
        ALIGN_RIGHT,
        ALIGN_CENTER,
        ALIGN_JUSTIFY,
        ANCHOR,
        BOOKMARK,
        LOADER,
        LIST,
        LAYOUT,
        LOCK,
        LOGIN,
        LOGOUT,
        MOVE,
        PACKAGE,
        MINUS,
        PLUS,
        X,
        REFRESH_CCW,
        REFRESH_CW,
        REPEAT,
        ROTATE_CCW,
        ROTATE_CW,
        SEARCH,
        SETTINGS,
        SAVE,
        STAR,
        TRASH1,
        TRASH2,
        ZOOM_IN,
        ZOOM_OUT
    };
    
    struct IStyle
    {
        friend struct StylesManager;
        
        std::string m_name;
        
        virtual SGCore::Ref<SGCore::ITexture2D> getIcon(IconType iconType) noexcept { return nullptr; };
        
    private:
        virtual void apply() = 0;
    };
}

#endif //SUNGEARENGINEEDITOR_ISTYLE_H
