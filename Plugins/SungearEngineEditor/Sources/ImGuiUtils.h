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
        bool m_isLMBReleased = false;
        bool m_isLMBDoubleClicked = false;
        
        bool m_isRMBClicked = false;
        bool m_isRMBDoubleClicked = false;
        
        bool m_isHovered = false;
        
        ImVec2 m_elementPosition { };
        ImVec2 m_elementClickableSize { };
    };
    
    enum class DragNDropType
    {
        SOURCE,
        TARGET,
        BOTH,
        UNKNOWN
    };
    
    enum class DragNDropState
    {
        DRAGGING,
        ACCEPTED,
        NONE
    };
    
    struct DragNDropInfo
    {
        bool m_isEnabled = true; // IN PARAM
        
        DragNDropType m_type = DragNDropType::UNKNOWN; // IN PARAM
        
        std::function<void()> m_drawSourceFunction; // IN PARAM
        std::function<void()> m_drawTargetFunction; // IN PARAM
        std::function<void(const ImGuiPayload*)> m_payloadProcessFunction; // IN PARAM
        
        std::string m_name; // IN PARAM
        void* m_data = nullptr; // IN PARAM
        size_t m_dataSize = 0; // IN PARAM
        
        ImGuiDragDropFlags m_flags = 0; // IN PARAM
        
        DragNDropState m_state = DragNDropState::NONE; // OUT PARAM
    };

    struct ImRectangleButtonInfo
    {
        void* m_imageNativeHandler { };
        ImVec2 m_buttonSize { };
        ImVec2 m_imageSize { };
        float m_buttonRounding = 3.0f;
        bool m_useDummy = true;
        ImVec2 m_imageOffset { -1, -1 };                    // if using -1, -1, then auto center image
        ImVec4 m_hoverBgColor { 0.3f, 0.3f, 0.3f, 0.3f };
        DragNDropInfo* m_dragNDropInfo { };
        std::string m_name = "##inv";
    };

    struct ImCheckboxInfo
    {
        std::string m_name;
        bool m_clicked = false;
        ImVec2 m_screenPosition { };
        ImVec2 m_clickableSize { };
        float m_rounding = 3.0f;
    };

    struct ImGuiUtils
    {
        static void BeginCheckboxesGroup(const std::string& name,
                                         const ImVec4& checkedCheckboxesColor = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1.0f),
                                         bool onlyOneCanBeChecked = true) noexcept;
        static void EndCheckboxesGroup() noexcept;

        static ImClickInfo ImageButton(void* imageNativeHandler,
                                       const ImVec2& buttonSize,
                                       const ImVec2& imageSize,
                                       const float& buttonRounding = 3.0f,
                                       bool useDummy = true,
                                       const ImVec2& imageOffset = ImVec2(-1, -1),    // if using -1, -1, then auto center image
                                       const ImVec4& hoverBgColor = ImVec4(0.3, 0.3, 0.3, 0.3),
                                       DragNDropInfo* dragNDropInfo = nullptr,
                                       const std::string& name = "##inv") noexcept;

        static ImClickInfo ImageButton(const ImRectangleButtonInfo& rectangleButtonInfo) noexcept;
        
        static ImClickInfo ImageButton(void* imageNativeHandler,
                                       const float& buttonRadius,
                                       const ImVec2& imageSize,
                                       const ImVec2& imageOffset = ImVec2(-1, -1),    // if using -1, -1, then auto center image
                                       const ImVec4& hoverBgColor = ImVec4(0.3, 0.3, 0.3, 0.3)) noexcept;

        static void TextWithColors(const char* fmt, ...) noexcept;

        static SGCore::AssetRef<SGCore::ITexture2D> getFileIcon(const std::filesystem::path& filePath,
                                                                const SGCore::ivec2_32& iconSize,
                                                                SGCore::Event<void(
                                                                        SGCore::Ref<SGCore::ITexture2D>& iconTexture,
                                                                        const std::string& fileExtension,
                                                                        const std::string& fileName)>* onIconSet = nullptr) noexcept;
        
    private:
        static bool ProcessInlineHexColor(const char* start, const char* end, ImVec4& color) noexcept;
        static inline bool m_isCheckBoxesGroupBegan = false;
        static inline bool m_isLastCheckBoxesGroupOnlyOneCanBeChecked = false;
        static inline std::string m_lastCheckboxesGroupName;
        static inline std::unordered_map<std::string, std::unordered_map<std::string, ImCheckboxInfo>> m_checkedCheckBoxes;
    };
}

#endif //SUNGEARENGINEEDITOR_IMGUIUTILS_H
