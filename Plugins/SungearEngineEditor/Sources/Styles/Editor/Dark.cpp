//
// Created by Ilya on 19.07.2024.
//
#include "Dark.h"

void SGE::EditorStyles::Dark::apply()
{
    ImVec4* colors = ImGui::GetStyle().Colors;

    colors[ImGuiCol_Text]                   = ImVec4(1.00f, 1.00f, 1.00f, 1.00f);
    colors[ImGuiCol_TextDisabled]           = ImVec4(0.50f, 0.50f, 0.50f, 1.00f);
    colors[ImGuiCol_WindowBg]               = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_ChildBg]                = ImVec4(0.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PopupBg]                = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
    colors[ImGuiCol_Border]                 = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_BorderShadow]           = ImVec4(0.15f, 0.15f, 0.15f, 0.24f);
    colors[ImGuiCol_FrameBg]                = ImVec4(0.15f, 0.15f, 0.15f, 1.00f);
    colors[ImGuiCol_FrameBgHovered]         = ImVec4(0.19f, 0.19f, 0.19f, 1.00f);
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
    colors[ImGuiCol_Button]                 = ImVec4(0.05f, 0.05f, 0.05f, 0.00f);
    colors[ImGuiCol_ButtonHovered]          = ImVec4(0.15f, 0.15f, 0.15f, 0.54f);
    colors[ImGuiCol_ButtonActive]           = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_Header]                 = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_HeaderHovered]          = ImVec4(0.00f, 0.00f, 0.00f, 0.36f);
    colors[ImGuiCol_HeaderActive]           = ImVec4(0.20f, 0.22f, 0.23f, 0.33f);
    colors[ImGuiCol_Separator]              = ImVec4(0.60f, 0.60f, 0.60f, 0.29f);
    colors[ImGuiCol_SeparatorHovered]       = ImVec4(0.44f, 0.44f, 0.44f, 0.29f);
    colors[ImGuiCol_SeparatorActive]        = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_ResizeGrip]             = ImVec4(0.28f, 0.28f, 0.28f, 0.00f);
    colors[ImGuiCol_ResizeGripHovered]      = ImVec4(0.44f, 0.44f, 0.44f, 1.00f);
    colors[ImGuiCol_ResizeGripActive]       = ImVec4(0.40f, 0.44f, 0.47f, 1.00f);
    colors[ImGuiCol_Tab]                    = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabHovered]             = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_TabActive]              = ImVec4(0.20f, 0.20f, 0.20f, 0.36f);
    colors[ImGuiCol_TabUnfocused]           = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TabUnfocusedActive]     = ImVec4(0.14f, 0.14f, 0.14f, 1.00f);
    colors[ImGuiCol_DockingPreview]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_DockingEmptyBg]         = ImVec4(0.30f, 0.30f, 0.30f, 1.00f);
    colors[ImGuiCol_PlotLines]              = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotLinesHovered]       = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogram]          = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_PlotHistogramHovered]   = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableHeaderBg]          = ImVec4(0.00f, 0.00f, 0.00f, 0.52f);
    colors[ImGuiCol_TableBorderStrong]      = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableBorderLight]       = ImVec4(1.00f, 0.28f, 0.28f, 1.00f);
    colors[ImGuiCol_TableRowBg]             = ImVec4(0.00f, 1.00f, 0.00f, 1.00f);
    colors[ImGuiCol_TableRowBgAlt]          = ImVec4(1.00f, 1.00f, 1.00f, 0.06f);
    colors[ImGuiCol_TextSelectedBg]         = ImVec4(0.20f, 0.22f, 0.23f, 1.00f);
    colors[ImGuiCol_DragDropTarget]         = ImVec4(0.33f, 0.67f, 0.86f, 1.00f);
    colors[ImGuiCol_NavHighlight]           = ImVec4(1.00f, 0.00f, 0.00f, 1.00f);
    colors[ImGuiCol_NavWindowingHighlight]  = ImVec4(1.00f, 0.00f, 0.00f, 0.70f);
    colors[ImGuiCol_NavWindowingDimBg]      = ImVec4(1.00f, 0.00f, 0.00f, 0.20f);
    colors[ImGuiCol_ModalWindowDimBg]       = ImVec4(0.50f, 0.50f, 0.50f, 0.2f);

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
    style.WindowRounding                    = 0;
    style.ChildRounding                     = 0;
    style.FrameRounding                     = 0;
    style.PopupRounding                     = 0;
    style.ScrollbarRounding                 = 0;
    style.GrabRounding                      = 0;
    style.LogSliderDeadzone                 = 4;
    style.TabRounding                       = 0;

    // bebas_kai
    // roboto_flex
    // THE STANDARD OF LOADING FONTS
    if(std::filesystem::exists(SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/roboto_flex.ttf"))
    {
        SGCore::ImGuiWrap::ImGuiLayer::destroyDeviceObjects();

        auto& io = ImGui::GetIO();

        ImFontConfig fontConfig;

        io.Fonts->Clear();

        m_fonts["default_16"] = io.Fonts->AddFontFromFileTTF(
                (SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/roboto_flex.ttf").c_str(),
                16, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

        m_fonts["default_18"] = io.Fonts->AddFontFromFileTTF(
                (SungearEngineEditor::getInstance()->getLocalPath() + "/Resources/fonts/roboto_flex.ttf").c_str(),
                18, &fontConfig, io.Fonts->GetGlyphRangesCyrillic());

        io.Fonts->Build();
    }

    // loading icons
    {
        m_folderIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_folder_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/folder.svg");

        m_chevronRightIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_chevron_right_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/chevron_right.svg");

        m_chevronDownIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_chevron_down_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/chevron_down.svg");

        m_headerIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_header_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/header.svg");

        m_cppIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_cpp_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/cpp.svg");

        m_cmakeIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_cmake_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/cmake.svg");

        m_linesIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_lines_",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/lines.svg");

        m_moreIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_more_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/more.svg");

        m_questionIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_question_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/question.svg");

        m_libraryIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_library_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/library.svg");

        m_trashBinIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_trash_bin_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/trash_bin.svg");

        m_pencilIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_pencil_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/pencil.svg");

        m_dummyIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_dummy_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/dummy.svg");

        m_crossIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_cross_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/cross.svg");

        m_arrowUpIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_arrow_up_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/arrow_up.svg");

        m_arrowRightIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_arrow_right_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/arrow_right.svg");

        m_arrowDownIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_arrow_down_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/arrow_down.svg");

        m_arrowLeftIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_arrow_left_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/arrow_left.svg");

        m_copyIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_copy_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/copy.svg");

        m_pasteIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_paste_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/paste.svg");

        m_cubesIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_cubes_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/cubes.svg");

        m_plusIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_plus_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/plus.svg");

        m_minusIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "white_minus_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/minus.svg");

        m_visualStudioIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "visual_studio_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/common/visual_studio.svg");

        m_mingwIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
                "mingw_icon",
                SungearEngineEditor::getInstance()->getLocalPath() +
                "/Resources/icons/dark_style/mingw.svg");
    }
}

