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

#include <lunasvg.h>
#include <SGCore/Memory/Assets/SVGImage.h>

namespace SGE::EditorStyles
{
    struct Dark : IStyle
    {
        Dark()
        {
            m_name = "Dark";
        }
        
    private:
        void apply() override;
    };
}

#endif //SUNGEARENGINEEDITOR_DARK_H
