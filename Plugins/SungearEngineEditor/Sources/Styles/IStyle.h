//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_ISTYLE_H
#define SUNGEARENGINEEDITOR_ISTYLE_H

#include <cstdint>
#include <string>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/Assets/SVGImage.h>
#include <array>

namespace SGE
{
    struct IStyle
    {
        friend struct StylesManager;
        
        std::string m_name;
        
        SGCore::AssetRef<SGCore::SVGImage> m_chevronRightIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_chevronDownIcon;
        
        SGCore::AssetRef<SGCore::SVGImage> m_headerIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_cppIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_folderIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_cmakeIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_linesIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_moreIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_questionIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_questionCircledIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_libraryIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_trashBinIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_pencilIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_crossIcon;
        
        SGCore::AssetRef<SGCore::SVGImage> m_arrowUpIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_arrowRightIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_arrowDownIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_arrowLeftIcon;
        
        SGCore::AssetRef<SGCore::SVGImage> m_copyIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_pasteIcon;
        
        SGCore::AssetRef<SGCore::SVGImage> m_dummyIcon;
        
        SGCore::AssetRef<SGCore::SVGImage> m_cubesIcon;

        SGCore::AssetRef<SGCore::SVGImage> m_plusIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_minusIcon;

        SGCore::AssetRef<SGCore::SVGImage> m_visualStudioIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_mingwIcon;

        SGCore::AssetRef<SGCore::SVGImage> m_greenCheckmark;
        SGCore::AssetRef<SGCore::SVGImage> m_redCross;

        SGCore::AssetRef<SGCore::SVGImage> m_infoIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_warningIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_errorIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_criticalIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_greenBugIcon;
        SGCore::AssetRef<SGCore::SVGImage> m_greenDebugRun;
        SGCore::AssetRef<SGCore::SVGImage> m_reloadIcon;

        std::unordered_map<std::string, ImFont*> m_fonts;

        std::array<ImVec4, ImGuiCol_COUNT> m_initialColors;

    protected:
        void fillInitialColors() noexcept;

        virtual void apply();
    };
}

#endif //SUNGEARENGINEEDITOR_ISTYLE_H
