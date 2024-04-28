//
// Created by ilya on 27.04.24.
//

#ifndef SUNGEARENGINEEDITOR_DARK_H
#define SUNGEARENGINEEDITOR_DARK_H

#include <imgui.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Graphics/API/IRenderer.h>

#include "Styles/IStyle.h"
#include "PluginMain.h"
#include "Resources.h"
#include "Utils.h"

#include <lunasvg.h>

namespace SGE::EditorStyles
{
    struct Dark : IStyle
    {
        Dark()
        {
            m_name = "Dark";
        }
        
        SGCore::Ref<SGCore::ITexture2D> getIcon(IconType iconType) noexcept override
        {
            switch(iconType)
            {
                case FOLDER:
                    return nullptr;
                case FOLDER_MINUS:
                    return nullptr;
                case FOLDER_PLUS:
                    return nullptr;
                case FILE:
                    return nullptr;
                case FILE_TEXT:
                    return nullptr;
                case FILE_MINUS:
                    return nullptr;
                case FILE_PLUS:
                    return nullptr;
                case EYE:
                    break;
                case EYE_OFF:
                    break;
                case LAYERS:
                    break;
                case COPY:
                    break;
                case GIT_BRANCH:
                    break;
                case GIT_COMMIT:
                    break;
                case GIT_MERGE:
                    break;
                case GIT_PULL_REQUEST:
                    break;
                case EDIT1:
                    break;
                case EDIT2:
                    break;
                case EDIT3:
                    break;
                case DOWNLOAD:
                    break;
                case DOWNLOAD_CLOUD:
                    break;
                case FILTER:
                    break;
                case CPU:
                    break;
                case CODE:
                    break;
                case CLIPBOARD:
                    break;
                case CHECK:
                    break;
                case CHEVRON_DOWN:
                    break;
                case CHEVRON_LEFT:
                    break;
                case CHEVRON_RIGHT:
                    break;
                case CHEVRON_UP:
                    break;
                case CHEVRONS_DOWN:
                    break;
                case CHEVRONS_LEFT:
                    break;
                case CHEVRONS_RIGHT:
                    break;
                case CHEVRONS_UP:
                    break;
                case CLOCK:
                    break;
                case BATTERY:
                    break;
                case ARROW_UP_RIGHT:
                    break;
                case ARROW_UP_LEFT:
                    break;
                case ARROW_RIGHT:
                    break;
                case ARROW_LEFT:
                    break;
                case ARROW_DOWN_RIGHT:
                    break;
                case ARROW_DOWN_LEFT:
                    break;
                case ARROW_DOWN:
                    break;
                case ARROW_UP:
                    break;
                case ALERT_TRIANGLE:
                    break;
                case ALERT_OCTAGON:
                    break;
                case ALERT_CIRCLE:
                    break;
                case ALIGN_LEFT:
                    break;
                case ALIGN_RIGHT:
                    break;
                case ALIGN_CENTER:
                    break;
                case ALIGN_JUSTIFY:
                    break;
                case ANCHOR:
                    break;
                case BOOKMARK:
                    break;
                case LOADER:
                    break;
                case LIST:
                    break;
                case LAYOUT:
                    break;
                case LOCK:
                    break;
                case LOGIN:
                    break;
                case LOGOUT:
                    break;
                case MOVE:
                    break;
                case PACKAGE:
                    break;
                case MINUS:
                    return nullptr;
                case PLUS:
                    return nullptr;
                case X:
                    return nullptr;
                case REFRESH_CCW:
                    break;
                case REFRESH_CW:
                    break;
                case REPEAT:
                    break;
                case ROTATE_CCW:
                    break;
                case ROTATE_CW:
                    break;
                case SEARCH:
                    break;
                case SETTINGS:
                    break;
                case SAVE:
                    break;
                case STAR:
                    break;
                case TRASH1:
                    break;
                case TRASH2:
                    break;
                case ZOOM_IN:
                    break;
                case ZOOM_OUT:
                    break;
            }
            
            return nullptr;
        }
        
    private:
        void apply() override
        {
            ImVec4* colors = ImGui::GetStyle().Colors;
            
            colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
            colors[ImGuiCol_WindowBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
            colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 0.92f);
            colors[ImGuiCol_Border]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_BorderShadow]           = ImVec4(0.15f, 0.15f, 0.15f, 0.24f);
            colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.15f, 0.54f);
            colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_FrameBgActive]          = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_TitleBg]                = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
            colors[ImGuiCol_TitleBgActive]          = ImVec4(0.06f, 0.06f, 0.06f, 1.00f);
            colors[ImGuiCol_TitleBgCollapsed]       = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_MenuBarBg]              = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_ScrollbarBg]            = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ScrollbarGrab]          = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabHovered]   = ImVec4(0.40f, 0.40f, 0.40f, 0.54f);
            colors[ImGuiCol_ScrollbarGrabActive]    = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_CheckMark]              = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_SliderGrab]             = ImVec4(0.34f, 0.34f, 0.34f, 0.54f);
            colors[ImGuiCol_SliderGrabActive]       = ImVec4(0.56f, 0.56f, 0.56f, 0.54f);
            colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.54f);
            colors[ImGuiCol_ButtonHovered]          = ImVec4(0.19f, 0.19f, 0.19f, 0.54f);
            colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
            colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
            colors[ImGuiCol_Separator]              = ImVec4(0.60f, 0.60f, 0.60f, 0.29f);
            colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
            colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
            colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
            colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
            colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_DockingEmptyBg]         = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
            colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderStrong]      = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
            colors[ImGuiCol_TableBorderLight]       = ImVec4(0.28f, 0.28f, 0.28f, 0.29f);
            colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
            colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
            colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
            colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
            colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
            colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
            colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
            colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(1.00f, 0.00f, 0.00f, 0.35f);
            
            ImGuiStyle& style = ImGui::GetStyle();
            
            style.WindowPadding                     = ImVec2(8.00f, 8.00f);
            style.FramePadding                      = ImVec2(5.00f, 2.00f);
            style.CellPadding                       = ImVec2(6.00f, 6.00f);
            style.ItemSpacing                       = ImVec2(6.00f, 6.00f);
            style.ItemInnerSpacing                  = ImVec2(6.00f, 6.00f);
            style.TouchExtraPadding                 = ImVec2(0.00f, 0.00f);
            style.IndentSpacing                     = 25;
            style.ScrollbarSize                     = 15;
            style.GrabMinSize                       = 10;
            style.WindowBorderSize                  = 1;
            style.ChildBorderSize                   = 1;
            style.PopupBorderSize                   = 1;
            style.FrameBorderSize                   = 1;
            style.TabBorderSize                     = 1;
            style.WindowRounding                    = 7;
            style.ChildRounding                     = 4;
            style.FrameRounding                     = 1;
            style.PopupRounding                     = 4;
            style.ScrollbarRounding                 = 9;
            style.GrabRounding                      = 3;
            style.LogSliderDeadzone                 = 4;
            style.TabRounding                       = 4;
            
            // bebas_kai
            // roboto_flex
            // THE STANDARD OF LOADING FONTS
            if(std::filesystem::exists(SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/roboto_flex.ttf"))
            {
                auto gapiType = SGCore::CoreMain::getRenderer()->getGAPIType();
                
                switch(gapiType)
                {
                    case SGCore::SG_API_TYPE_UNKNOWN:
                        break;
                    case SGCore::SG_API_TYPE_GL4:
                        ImGui_ImplOpenGL3_DestroyDeviceObjects();
                        break;
                    case SGCore::SG_API_TYPE_GL46:
                        ImGui_ImplOpenGL3_DestroyDeviceObjects();
                        break;
                    case SGCore::SG_API_TYPE_GLES2:
                        break;
                    case SGCore::SG_API_TYPE_GLES3:
                        break;
                    case SGCore::SG_API_TYPE_VULKAN:
                        break;
                }
                
                auto& io = ImGui::GetIO();
                
                ImFontConfig fontConfig;
                
                io.Fonts->Clear();
                
                io.Fonts->AddFontFromFileTTF(
                        (SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/roboto_flex.ttf").c_str(),
                        16, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());
                
                /*fontConfig.MergeMode = true;
                const ImWchar iconsRanges[] = { 0xe000, 0xe0fe, 0 };
                io.Fonts->AddFontFromFileTTF(
                        (SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/icons_font0.ttf").c_str(),
                        16, &fontConfig, iconsRanges);*/
                
                io.Fonts->Build();
            }
            
            // loading icons
            {
                static constexpr glm::ivec2 standardSize { 16, 16 };
                
                Utils::loadSVG(SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/entypo/entypo/air.svg", "air", standardSize.x, standardSize.y);
            }
        }
    };
}

#endif //SUNGEARENGINEEDITOR_DARK_H
