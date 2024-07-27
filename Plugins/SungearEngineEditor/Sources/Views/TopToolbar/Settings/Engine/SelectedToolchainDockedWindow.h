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

        void setSelectedToolchain(const SGCore::Ref<Toolchain>& toolchain) noexcept;
        SGCore::Ref<Toolchain> getSelectedToolchain() const noexcept;

        std::function<void()> onToolchainChanged;

    private:
        void setSelectedToolchainPath(const std::filesystem::path& path);

        std::string m_currentToolchainName;
        std::string m_currentToolchainPath;

        std::string m_toolchainPathError;
        std::string m_cmakePathError;
        std::string m_buildToolPathError;

        SGCore::Ref<Toolchain> m_selectedToolchain;
    };
}

#endif //SUNGEARENGINEEDITOR_SELECTEDTOOLCHAINDOCKEDWINDOW_H
