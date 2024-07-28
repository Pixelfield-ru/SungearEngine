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

        void updateSelectedToolchain();

        std::function<void()> onToolchainChanged;

    private:
        enum class ToolchainPathType
        {
            OWN,
            CMAKE,
            BUILD_TOOL
        };

        void setSelectedToolchainPath(const std::filesystem::path& path, ToolchainPathType pathType);

        std::string m_currentToolchainName;

        std::string m_currentToolchainPath;
        std::string m_currentToolchainCMakePath;
        std::string m_currentToolchainBuildToolPath;

        std::string m_toolchainPathError;
        std::string m_cmakePathError;
        std::string m_buildToolPathError;

        // VISUAL STUDIO ONLY ===============================
        std::string m_currentVSToolchainWinSDKVersion;

        static inline const char* m_vsArchTypes[] = { "x86", "amd64", "x86_amd64", "x86_arm", "x86_arm64", "amd64_x86",
                                                      "amd64_arm", "amd64_arm64" };

        static inline const char* m_vsPlatformTypes[] = { "empty", "store", "uwp" };

        std::int32_t m_vsCurrentSelectedArchTypeInCombo = 0;
        std::int32_t m_vsCurrentSelectedPlatformTypeInCombo = 0;
        // ==================================================

        SGCore::Ref<Toolchain> m_selectedToolchain;
    };
}

#endif //SUNGEARENGINEEDITOR_SELECTEDTOOLCHAINDOCKEDWINDOW_H
