//
// Created by Ilya on 27.07.2024.
//
#include "IStyle.h"
#include "Resources.h"
#include "SungearEngineEditor.h"

void SGE::IStyle::apply()
{
    m_visualStudioIcon = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
            "visual_studio_icon",
            SungearEngineEditor::getInstance()->getLocalPath() +
            "/Resources/icons/common/visual_studio.svg");

    m_greenCheckmark = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
            "green_checkmark",
            SungearEngineEditor::getInstance()->getLocalPath() +
            "/Resources/icons/common/green_checkmark.svg");

    m_redCross = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
            "red_cross",
            SungearEngineEditor::getInstance()->getLocalPath() +
            "/Resources/icons/common/red_cross.svg");

    m_greenDebugRun = Resources::getMainAssetManager().loadAssetWithAlias<SGCore::SVGImage>(
            "green_debug_run",
            SungearEngineEditor::getInstance()->getLocalPath() +
            "/Resources/icons/common/green_debug_run.svg");
}

