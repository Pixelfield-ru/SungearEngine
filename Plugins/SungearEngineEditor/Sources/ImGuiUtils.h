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
        bool m_isLMBClicked = false;
        bool m_isLMBDoubleClicked = false;
        bool m_isRMBClicked = false;
        bool m_isRMBDoubleClicked = false;
        bool m_isHovered = false;
        ImVec2 m_elementPosition { };
        ImVec2 m_elementClickableSize { };
    };

    struct ImGuiUtils
    {
        static ImClickInfo ImageButton(void* imageNativeHandler,
                                       const ImVec2& buttonSize,
                                       const ImVec2& imageSize,
                                       const ImVec2& imageOffset = ImVec2(-1, -1),    // if using -1, -1, then auto center image
                                       const ImVec4& hoverBgColor = ImVec4(0.3, 0.3, 0.3, 0.3)) noexcept;
        
        static ImClickInfo ImageButton(void* imageNativeHandler,
                                       const float& buttonRadius,
                                       const ImVec2& imageSize,
                                       const ImVec2& imageOffset = ImVec2(-1, -1),    // if using -1, -1, then auto center image
                                       const ImVec4& hoverBgColor = ImVec4(0.3, 0.3, 0.3, 0.3)) noexcept;

        static void TextWithColors(const char* fmt, ...) noexcept;
        
        static SGCore::Ref<SGCore::ITexture2D> getFileIcon(const std::filesystem::path& filePath,
                                                           const SGCore::ivec2_32& iconSize,
                                                           SGCore::Event<void(SGCore::Ref<SGCore::ITexture2D>& iconTexture,
                                                                              const std::string& fileExtension,
                                                                              const std::string& fileName)>* onIconSet = nullptr) noexcept;
        
    private:
        static bool ProcessInlineHexColor(const char* start, const char* end, ImVec4& color) noexcept;
    };
}

#endif //SUNGEARENGINEEDITOR_IMGUIUTILS_H
