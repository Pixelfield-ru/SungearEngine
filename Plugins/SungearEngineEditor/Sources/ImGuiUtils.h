//
// Created by ilya on 30.04.24.
//

#ifndef SUNGEARENGINEEDITOR_IMGUIUTILS_H
#define SUNGEARENGINEEDITOR_IMGUIUTILS_H

#include <imgui.h>
#include <SGCore/Main/CoreGlobals.h>
#include <SGCore/Graphics/API/ITexture2D.h>

namespace SGE
{
    struct ImClickInfo
    {
        bool m_isClicked = false;
        bool m_isDoubleClicked = false;
        bool m_isHovered = false;
    };
    
    struct ImGuiUtils
    {
        static ImClickInfo ImageButton(void* imageNativeHandler, const ImVec2& imageSize,
                                       const ImVec2& hoverMinOffset = ImVec2(-3, -3),
                                       const ImVec2& hoverMaxOffset = ImVec2(3, 3),
                                       const ImVec4& hoverBgColor = ImVec4(0.3, 0.3, 0.3, 0.3)) noexcept;
        
        static SGCore::Ref<SGCore::ITexture2D> getFileIcon(const std::filesystem::path& filePath,
                                                           const SGCore::ivec2_32& iconSize,
                                                           SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>& iconTexture,
                                                                              const std::string& fileExtension,
                                                                              const std::string& fileName)>* onIconSet = nullptr) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_IMGUIUTILS_H
