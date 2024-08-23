//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_ISTYLE_H
#define SUNGEARENGINEEDITOR_ISTYLE_H

#include <cstdint>
#include <string>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/Assets/SVGImage.h>

namespace SGE
{
    struct IStyle
    {
        friend struct StylesManager;
        
        std::string m_name;
        
        SGCore::Ref<SGCore::SVGImage> m_chevronRightIcon;
        SGCore::Ref<SGCore::SVGImage> m_chevronDownIcon;
        
        SGCore::Ref<SGCore::SVGImage> m_headerIcon;
        SGCore::Ref<SGCore::SVGImage> m_cppIcon;
        SGCore::Ref<SGCore::SVGImage> m_folderIcon;
        SGCore::Ref<SGCore::SVGImage> m_cmakeIcon;
        SGCore::Ref<SGCore::SVGImage> m_linesIcon;
        SGCore::Ref<SGCore::SVGImage> m_moreIcon;
        SGCore::Ref<SGCore::SVGImage> m_questionIcon;
        SGCore::Ref<SGCore::SVGImage> m_questionCircledIcon;
        SGCore::Ref<SGCore::SVGImage> m_libraryIcon;
        SGCore::Ref<SGCore::SVGImage> m_trashBinIcon;
        SGCore::Ref<SGCore::SVGImage> m_pencilIcon;
        SGCore::Ref<SGCore::SVGImage> m_crossIcon;
        
        SGCore::Ref<SGCore::SVGImage> m_arrowUpIcon;
        SGCore::Ref<SGCore::SVGImage> m_arrowRightIcon;
        SGCore::Ref<SGCore::SVGImage> m_arrowDownIcon;
        SGCore::Ref<SGCore::SVGImage> m_arrowLeftIcon;
        
        SGCore::Ref<SGCore::SVGImage> m_copyIcon;
        SGCore::Ref<SGCore::SVGImage> m_pasteIcon;
        
        SGCore::Ref<SGCore::SVGImage> m_dummyIcon;
        
        SGCore::Ref<SGCore::SVGImage> m_cubesIcon;

        SGCore::Ref<SGCore::SVGImage> m_plusIcon;
        SGCore::Ref<SGCore::SVGImage> m_minusIcon;

        SGCore::Ref<SGCore::SVGImage> m_visualStudioIcon;
        SGCore::Ref<SGCore::SVGImage> m_mingwIcon;

        SGCore::Ref<SGCore::SVGImage> m_greenCheckmark;
        SGCore::Ref<SGCore::SVGImage> m_redCross;

        SGCore::Ref<SGCore::SVGImage> m_infoIcon;
        SGCore::Ref<SGCore::SVGImage> m_warningIcon;
        SGCore::Ref<SGCore::SVGImage> m_errorIcon;
        SGCore::Ref<SGCore::SVGImage> m_criticalIcon;
        SGCore::Ref<SGCore::SVGImage> m_greenBugIcon;
        SGCore::Ref<SGCore::SVGImage> m_greenDebugRun;

        std::unordered_map<std::string, ImFont*> m_fonts;

    protected:
        virtual void apply();
    };
}

#endif //SUNGEARENGINEEDITOR_ISTYLE_H
