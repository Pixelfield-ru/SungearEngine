//
// Created by Ilya on 24.07.2024.
//

#ifndef SUNGEARENGINEEDITOR_SELECTEDTOOLCHAINDOCKEDWINDOW_H
#define SUNGEARENGINEEDITOR_SELECTEDTOOLCHAINDOCKEDWINDOW_H

#include "Views/DockedWindow.h"
#include "Toolchains/Toolchain.h"

namespace SGE
{
    struct SelectedToolchainDockedWindow : DockedWindow
    {
        void renderBody() final;

        SGCore::Ref<Toolchain> m_selectedToolchain;
    };
}

#endif //SUNGEARENGINEEDITOR_SELECTEDTOOLCHAINDOCKEDWINDOW_H
