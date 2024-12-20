//
// Created by Ilya on 27.07.2024.
//
#include "IStyle.h"
#include "Resources.h"
#include "SungearEngineEditor.h"

void SGE::IStyle::apply()
{
    m_visualStudioIcon = Resources::getEditorOnlyAssetManager()->loadAssetWithAlias<SGCore::SVGImage>(
            "visual_studio_icon",
            SungearEngineEditor::getInstance()->getLocalPath() /
            "Resources/icons/common/visual_studio.svg");

    m_greenCheckmark = Resources::getEditorOnlyAssetManager()->loadAssetWithAlias<SGCore::SVGImage>(
            "green_checkmark",
            SungearEngineEditor::getInstance()->getLocalPath() /
            "Resources/icons/common/green_checkmark.svg");

    m_redCross = Resources::getEditorOnlyAssetManager()->loadAssetWithAlias<SGCore::SVGImage>(
            "red_cross",
            SungearEngineEditor::getInstance()->getLocalPath() /
            "Resources/icons/common/red_cross.svg");

    m_greenDebugRun = Resources::getEditorOnlyAssetManager()->loadAssetWithAlias<SGCore::SVGImage>(
            "green_debug_run",
            SungearEngineEditor::getInstance()->getLocalPath() /
            "Resources/icons/common/green_debug_run.svg");
}

void SGE::IStyle::fillInitialColors() noexcept
{
    for(std::int64_t i = 0; i < m_initialColors.size(); ++i)
    {
        m_initialColors[i] = ImGui::GetStyle().Colors[i];
    }
}

