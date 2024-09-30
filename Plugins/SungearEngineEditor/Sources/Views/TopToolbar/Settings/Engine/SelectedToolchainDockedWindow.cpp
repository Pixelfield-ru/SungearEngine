//
// Created by Ilya on 24.07.2024.
//

#include "SelectedToolchainDockedWindow.h"
#include "Styles/StylesManager.h"
#include "ImGuiUtils.h"
#include "nfd.h"
#include "Toolchains/VisualStudioToolchain.h"
#include "SungearEngineEditor.h"
#include <SGCore/Exceptions/FileNotFoundException.h>

#include <imgui_stdlib.h>

void SGE::SelectedToolchainDockedWindow::renderBody()
{
    auto lockedSelectedToolchain = m_selectedToolchain.lock();

    if (!lockedSelectedToolchain) return;

    m_folderTexture = StylesManager::getCurrentStyle()->m_folderIcon
            ->getSpecialization(20, 20)
            ->getTexture();

    m_greenCheckMarkTexture = StylesManager::getCurrentStyle()->m_greenCheckmark
            ->getSpecialization(16, 16)
            ->getTexture();

    m_redCrossTexture = StylesManager::getCurrentStyle()->m_redCross
            ->getSpecialization(22, 22)
            ->getTexture();

    m_questionCircledTexture = StylesManager::getCurrentStyle()->m_questionCircledIcon
            ->getSpecialization(16, 16)
            ->getTexture();

    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(1, 5));
    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
    if (ImGui::BeginTable((m_name.getName() + "_Table").c_str(), 3, ImGuiTableFlags_SizingStretchProp))
    {
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
            ImGui::Text("Name");

            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::InputText("##ToolchainNameInputText", &m_currentToolchainName, ImGuiInputTextFlags_EnterReturnsTrue);

            if (ImGui::IsItemEdited())
            {
                lockedSelectedToolchain->m_name = m_currentToolchainName;
                m_currentToolchainName = lockedSelectedToolchain->m_name.getName();

                if (onToolchainChanged)
                {
                    onToolchainChanged();
                }
            }

            ImGui::TableNextColumn();
        }

        // DUMMY
        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::TableNextColumn();
            ImGui::TableNextColumn();
        }

        ImGui::TableNextRow();
        {
            ImGui::TableNextColumn();
            ImGui::GetForegroundDrawList()->AddLine(
                    { ImGui::GetCursorScreenPos().x, ImGui::GetCursorScreenPos().y - 10 },
                    ImVec2(ImGui::GetCursorScreenPos().x + ImGui::GetWindowSize().x - 17,
                           ImGui::GetCursorScreenPos().y - 10),
                    ImGui::ColorConvertFloat4ToU32({ 0.60f, 0.60f, 0.60f, 0.29f }));
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
            ImGui::Text("Path");

            ImGui::TableNextColumn();
            ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::InputText("##ToolchainPathInputText", &m_currentToolchainPath, ImGuiInputTextFlags_EnterReturnsTrue);

            if (ImGui::IsItemEdited())
            {
                setSelectedToolchainPath(m_currentToolchainPath, ToolchainPathType::OWN);

                if (onToolchainChanged)
                {
                    onToolchainChanged();
                }
            }

            if (!m_toolchainPathError.empty())
            {
                if (!m_toolchainPathError.contains("Correct toolchain"))
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                    ImGui::Image(m_redCrossTexture->getTextureNativeHandler(),
                                 { (float) m_redCrossTexture->getWidth(),
                                   (float) m_redCrossTexture->getHeight() });
                    ImGui::SameLine();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
                    ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, m_toolchainPathError.c_str());
                } else
                {
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                    ImGui::Image(m_greenCheckMarkTexture->getTextureNativeHandler(),
                                 { (float) m_greenCheckMarkTexture->getWidth(),
                                   (float) m_greenCheckMarkTexture->getHeight() });
                    ImGui::SameLine();
                    ImGui::TextColored({ 1.0, 1.0, 1.0, 1.0 }, m_toolchainPathError.c_str());
                }
            }

            ImGui::TableNextColumn();

            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
            if (ImGuiUtils::ImageButton(m_folderTexture->getTextureNativeHandler(),
                                        ImVec2(m_folderTexture->getWidth() + 6, m_folderTexture->getHeight() + 6),
                                        ImVec2(m_folderTexture->getWidth(),
                                               m_folderTexture->getHeight())).m_isLMBClicked)
            {
                char* dat = m_currentToolchainPath.data();
                nfdresult_t result = NFD_PickFolder("", &dat);
                if (result == NFD_OKAY)
                {
                    m_currentToolchainPath = dat;
                    setSelectedToolchainPath(m_currentToolchainPath, ToolchainPathType::OWN);
                }
            }
        }

        switch (lockedSelectedToolchain->getType())
        {
            case ToolchainType::VISUAL_STUDIO:
            {
                auto vsToolchain = std::static_pointer_cast<VisualStudioToolchain>(lockedSelectedToolchain);

                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();

                    ImGui::TableNextColumn();

                    ImGui::PushStyleVar(ImGuiStyleVar_CellPadding, ImVec2(0, 0));
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x);
                    if (ImGui::BeginTable((m_name.getName() + "_VSTable").c_str(), 2,
                                          ImGuiTableFlags_SizingStretchProp))
                    {
                        ImGui::TableNextRow();
                        {
                            ImGui::TableNextColumn();
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                            ImGui::Text("Architecture");
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                            ImGui::SetNextItemWidth(130);
                            if (ImGui::Combo("##VSToolchainArchType", &m_vsCurrentSelectedArchTypeInCombo,
                                             m_vsArchTypes,
                                             m_vsArchTypesCount))
                            {
                                switch (m_vsCurrentSelectedArchTypeInCombo)
                                {
                                    case 0:
                                        vsToolchain->m_archType = VCArchType::X86;
                                        break;

                                    case 1:
                                        vsToolchain->m_archType = VCArchType::AMD64;
                                        break;

                                    case 2:
                                        vsToolchain->m_archType = VCArchType::X86_AMD64;
                                        break;

                                    case 3:
                                        vsToolchain->m_archType = VCArchType::X86_ARM;
                                        break;

                                    case 4:
                                        vsToolchain->m_archType = VCArchType::X86_ARM64;
                                        break;

                                    case 5:
                                        vsToolchain->m_archType = VCArchType::AMD64_X86;
                                        break;

                                    case 6:
                                        vsToolchain->m_archType = VCArchType::AMD64_ARM;
                                        break;

                                    case 7:
                                        vsToolchain->m_archType = VCArchType::AMD64_ARM64;
                                        break;
                                }
                            }

                            ImGui::TableNextColumn();
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                            ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 2);
                            ImGui::Text("Platform");
                            ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                            ImGui::SetNextItemWidth(80);
                            if (ImGui::Combo("##VSToolchainPlatformType", &m_vsCurrentSelectedPlatformTypeInCombo,
                                             m_vsPlatformTypes, m_vsPlatformTypesCount))
                            {
                                switch (m_vsCurrentSelectedPlatformTypeInCombo)
                                {
                                    case 0:
                                        vsToolchain->m_platformType = VCPlatformType::EMPTY;
                                        break;

                                    case 1:
                                        vsToolchain->m_platformType = VCPlatformType::STORE;
                                        break;

                                    case 2:
                                        vsToolchain->m_platformType = VCPlatformType::UWP;
                                        break;
                                }
                            }
                        }

                        ImGui::EndTable();
                    }
                    ImGui::PopStyleVar();

                    ImGui::TableNextColumn();
                }

                ImGui::TableNextRow();
                {
                    ImGui::TableNextColumn();
                    ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
                    ImGui::Text("SDK Version");
                    ImGui::SameLine();
                    ImGui::Image(m_questionCircledTexture->getTextureNativeHandler(), {
                            (float) m_questionCircledTexture->getWidth(),
                            (float) m_questionCircledTexture->getHeight()
                    });
                    if (ImGui::IsItemHovered())
                    {
                        ImGui::SetTooltip("Leave empty to use the latest installed version of SDK");
                    }

                    ImGui::TableNextColumn();
                    ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
                    ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                    ImGui::InputText("##VSToolchainWinSDKVersionInputText", &m_currentVSToolchainWinSDKVersion,
                                     ImGuiInputTextFlags_EnterReturnsTrue);

                    if (ImGui::IsItemEdited())
                    {
                        vsToolchain->m_winSDKVersion = m_currentVSToolchainWinSDKVersion;

                        std::string vcvarsallSDKTestResult;

                        if (std::filesystem::exists(vsToolchain->m_vcvarsallPath))
                        {
                            vcvarsallSDKTestResult = SGCore::Utils::consoleExecute(fmt::format("\"{0}\" {1} {2} {3}",
                                                                                               SGCore::Utils::toUTF8(
                                                                                                       vsToolchain->m_vcvarsallPath.u16string()),
                                                                                               VCArchTypeToString(
                                                                                                       vsToolchain->m_archType),
                                                                                               VCPlatformTypeToString(
                                                                                                       vsToolchain->m_platformType),
                                                                                               vsToolchain->m_winSDKVersion));

                            if (vcvarsallSDKTestResult.contains("ERROR"))
                            {
                                m_toolchainPathError = "Invalid SDK Version";
                            }
                            else
                            {
                                m_toolchainPathError = "Correct toolchain";
                            }
                        }

                        if (onToolchainChanged)
                        {
                            onToolchainChanged();
                        }
                    }
                }
            }
                break;
        }

        drawCMakeChooseRow();

        drawBuildToolChooseRow();

        ImGui::EndTable();
    }
    ImGui::PopStyleVar();

    // ImGui::Separator();
}

void SGE::SelectedToolchainDockedWindow::setSelectedToolchain(const SGCore::Ref<Toolchain>& toolchain) noexcept
{
    m_selectedToolchain = toolchain;

    if(toolchain)
    {
        // if visual studio toolchain
        if(toolchain->getType() == ToolchainType::VISUAL_STUDIO)
        {
            auto* vsToolchain = static_cast<VisualStudioToolchain*>(toolchain.get());

            const std::string vcArchTypeStr = VCArchTypeToString(vsToolchain->m_archType);
            const std::string vcPlatformTypeStr = VCPlatformTypeToString(vsToolchain->m_platformType);

            // selecting necessary arch type that equals to toolchain->m_archType
            for(std::uint8_t i = 0; i < m_vsArchTypesCount; ++i)
            {
                if(vcArchTypeStr == m_vsArchTypes[i])
                {
                    m_vsCurrentSelectedArchTypeInCombo = i;
                    break;
                }
            }

            // selecting necessary platform type that equals to toolchain->m_platformType
            for(std::uint8_t i = 0; i < m_vsPlatformTypesCount; ++i)
            {
                if(vcPlatformTypeStr == m_vsPlatformTypes[i])
                {
                    m_vsCurrentSelectedPlatformTypeInCombo = i;
                    break;
                }
            }
        }
    }

    updateSelectedToolchain();
}

SGCore::Ref<SGE::Toolchain> SGE::SelectedToolchainDockedWindow::getSelectedToolchain() const noexcept
{
    return m_selectedToolchain.lock();
}

void SGE::SelectedToolchainDockedWindow::setSelectedToolchainPath(const std::filesystem::path& path,
                                                                  ToolchainPathType pathType)
{
    if(auto l = m_selectedToolchain.lock(); !l) return;

    auto worker = SungearEngineEditor::getInstance()->m_threadsPool.getThread();

    auto task = SGCore::MakeRef<SGCore::Threading::Task>();
    task->setOnExecuteCallback([this, pathType, path]() {
        auto lockedSelectedToolchain = m_selectedToolchain.lock();

        try
        {
            switch(pathType)
            {
                case ToolchainPathType::OWN:
                {
                    lockedSelectedToolchain->setPath(path);
                    m_toolchainPathError = "Correct toolchain";
                    if(m_currentToolchainCMakePath.empty())
                    {
                        setSelectedToolchainPath(lockedSelectedToolchain->getCMakePath(), ToolchainPathType::CMAKE);
                        m_currentToolchainCMakePath = SGCore::Utils::toUTF8(lockedSelectedToolchain->getCMakePath().u16string());

                        std::printf("selected cmake %s\n", m_currentToolchainCMakePath.c_str());
                    }
                    if(m_currentToolchainBuildToolPath.empty())
                    {
                        setSelectedToolchainPath(lockedSelectedToolchain->getBuildToolPath(),
                                                 ToolchainPathType::BUILD_TOOL);
                        m_currentToolchainBuildToolPath = SGCore::Utils::toUTF8(
                                lockedSelectedToolchain->getBuildToolPath().u16string());

                        std::printf("selected build tool %s\n", m_currentToolchainBuildToolPath.c_str());
                    }
                    break;
                }
                case ToolchainPathType::CMAKE:
                {
                    // AUTO DETECTING CMAKE
                    if(path.empty())
                    {
                        lockedSelectedToolchain->setCMakePath("");
                        setSelectedToolchainPath(lockedSelectedToolchain->getPath(), ToolchainPathType::OWN);
                        break;
                    }
                    lockedSelectedToolchain->setCMakePath(path);
                    m_cmakePathError = "Version: " + lockedSelectedToolchain->getCMakeVersion();
                    std::printf("correct cmake\n");
                    break;
                }
                case ToolchainPathType::BUILD_TOOL:
                {
                    // AUTO DETECTING BUILD TOOL
                    if(path.empty())
                    {
                        lockedSelectedToolchain->setBuildToolPath("");
                        setSelectedToolchainPath(lockedSelectedToolchain->getPath(), ToolchainPathType::OWN);
                        break;
                    }
                    lockedSelectedToolchain->setBuildToolPath(path);
                    m_buildToolPathError = "Version: " + lockedSelectedToolchain->getBuildToolVersion();
                    break;
                }
            }
        }
        catch(const SGCore::FileNotFoundException& e)
        {
            std::string errWhat = e.what();
            if(errWhat.contains("cmake") || errWhat.contains("CMake"))
            {
                m_cmakePathError = errWhat;
            }
            else if(errWhat.contains("build tool"))
            {
                m_buildToolPathError = errWhat;
            }
            else
            {
                m_toolchainPathError = errWhat;
            }
        }
        catch(const std::exception& e)
        {
            std::string errWhat = e.what();
            if(errWhat.contains("cmake") || errWhat.contains("CMake"))
            {
                m_cmakePathError = errWhat;
            }
            else if(errWhat.contains("build tool"))
            {
                m_buildToolPathError = errWhat;
            }
            else
            {
                m_toolchainPathError = errWhat;
            }
        }
    });

    worker->addTask(task);
    worker->start();
}

void SGE::SelectedToolchainDockedWindow::updateSelectedToolchain()
{
    auto lockedSelectedToolchain = m_selectedToolchain.lock();
    if (!lockedSelectedToolchain) return;

    m_currentToolchainName = lockedSelectedToolchain->m_name.getName();
    m_currentToolchainPath = SGCore::Utils::toUTF8(lockedSelectedToolchain->getPath().u16string());
    m_currentToolchainCMakePath = SGCore::Utils::toUTF8(lockedSelectedToolchain->getCMakePath().u16string());
    m_currentToolchainBuildToolPath = SGCore::Utils::toUTF8(lockedSelectedToolchain->getBuildToolPath().u16string());

    setSelectedToolchainPath(m_currentToolchainPath, ToolchainPathType::OWN);
    setSelectedToolchainPath(m_currentToolchainCMakePath, ToolchainPathType::CMAKE);
    setSelectedToolchainPath(m_currentToolchainBuildToolPath, ToolchainPathType::BUILD_TOOL);

    // TOOLCHAIN TEST
    switch (lockedSelectedToolchain->getType())
    {
        case ToolchainType::VISUAL_STUDIO:
        {
            auto vsToolchain = std::static_pointer_cast<VisualStudioToolchain>(lockedSelectedToolchain);

            if (std::filesystem::exists(vsToolchain->m_vcvarsallPath))
            {
                std::string vcvarsallSDKTestResult = SGCore::Utils::consoleExecute(
                        fmt::format("\"{0}\" {1} {2} {3}",
                                    SGCore::Utils::toUTF8(
                                            vsToolchain->m_vcvarsallPath.u16string()),
                                    VCArchTypeToString(
                                            vsToolchain->m_archType),
                                    VCPlatformTypeToString(
                                            vsToolchain->m_platformType),
                                    vsToolchain->m_winSDKVersion));

                if (vcvarsallSDKTestResult.contains("ERROR"))
                {
                    m_toolchainPathError = "Invalid SDK Version";
                } else
                {
                    m_toolchainPathError = "Correct toolchain";
                }
            }
            break;
        }
    }
}

void SGE::SelectedToolchainDockedWindow::drawCMakeChooseRow()
{
    ImGui::TableNextRow();
    {
        ImGui::TableNextColumn();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
        ImGui::Text("CMake");
        ImGui::SameLine();
        ImGui::Image(m_questionCircledTexture->getTextureNativeHandler(), {
                (float) m_questionCircledTexture->getWidth(),
                (float) m_questionCircledTexture->getHeight()
        });
        if(ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Leave empty to auto-detect");
        }

        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::InputText("##ToolchainCMakePathInputText", &m_currentToolchainCMakePath,
                         ImGuiInputTextFlags_EnterReturnsTrue);

        // m_selectedToolchain->set

        if(ImGui::IsItemEdited())
        {
            setSelectedToolchainPath(m_currentToolchainCMakePath, ToolchainPathType::CMAKE);

            if(onToolchainChanged)
            {
                onToolchainChanged();
            }
        }

        if (!m_cmakePathError.empty())
        {
            if(!m_cmakePathError.contains("Version"))
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                ImGui::Image(m_redCrossTexture->getTextureNativeHandler(),
                             { (float) m_redCrossTexture->getWidth(),
                               (float) m_redCrossTexture->getHeight() });
                ImGui::SameLine();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
                ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, m_cmakePathError.c_str());
            }
            else
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::Image(m_greenCheckMarkTexture->getTextureNativeHandler(),
                             { (float) m_greenCheckMarkTexture->getWidth(),
                               (float) m_greenCheckMarkTexture->getHeight() });
                ImGui::SameLine();
                ImGui::TextColored({ 1.0, 1.0, 1.0, 1.0 }, m_cmakePathError.c_str());
            }
        }

        ImGui::TableNextColumn();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
        if (ImGuiUtils::ImageButton(m_folderTexture->getTextureNativeHandler(),
                                    ImVec2(m_folderTexture->getWidth() + 6, m_folderTexture->getHeight() + 6),
                                    ImVec2(m_folderTexture->getWidth(), m_folderTexture->getHeight())).m_isLMBClicked)
        {
            char* dat = m_currentToolchainCMakePath.data();
            nfdresult_t result = NFD_OpenDialog({}, "", &dat);
            if (result == NFD_OKAY)
            {
                m_currentToolchainCMakePath = dat;
                setSelectedToolchainPath(m_currentToolchainCMakePath, ToolchainPathType::CMAKE);

                // std::printf("selected cmake %s\n", SGCore::Utils::toUTF8(m_selectedToolchain->getCMakePath().u16string()).c_str());
            }
        }
    }
}

void SGE::SelectedToolchainDockedWindow::drawBuildToolChooseRow()
{
    ImGui::TableNextRow();
    {
        ImGui::TableNextColumn();
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
        ImGui::Text("Build Tool");
        ImGui::SameLine();
        ImGui::Image(m_questionCircledTexture->getTextureNativeHandler(), {
                (float) m_questionCircledTexture->getWidth(),
                (float) m_questionCircledTexture->getHeight()
        });
        if(ImGui::IsItemHovered())
        {
            ImGui::SetTooltip("Leave empty to auto-detect");
        }

        ImGui::TableNextColumn();
        ImGui::SetNextItemWidth(ImGui::GetContentRegionAvail().x - 7);
        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::InputText("##ToolchainBuildToolPathInputText", &m_currentToolchainBuildToolPath,
                         ImGuiInputTextFlags_EnterReturnsTrue);

        // m_selectedToolchain->set

        if(ImGui::IsItemEdited())
        {
            setSelectedToolchainPath(m_currentToolchainBuildToolPath, ToolchainPathType::BUILD_TOOL);

            if(onToolchainChanged)
            {
                onToolchainChanged();
            }
        }

        if (!m_buildToolPathError.empty())
        {
            if(!m_buildToolPathError.contains("Version"))
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
                ImGui::Image(m_redCrossTexture->getTextureNativeHandler(),
                             { (float) m_redCrossTexture->getWidth(),
                               (float) m_redCrossTexture->getHeight() });
                ImGui::SameLine();
                ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 3);
                ImGui::TextColored({ 1.0, 0.0, 0.0, 1.0 }, m_buildToolPathError.c_str());
            }
            else
            {
                ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
                ImGui::Image(m_greenCheckMarkTexture->getTextureNativeHandler(),
                             { (float) m_greenCheckMarkTexture->getWidth(),
                               (float) m_greenCheckMarkTexture->getHeight() });
                ImGui::SameLine();
                ImGui::TextColored({ 1.0, 1.0, 1.0, 1.0 }, m_buildToolPathError.c_str());
            }
        }

        ImGui::TableNextColumn();

        ImGui::SetCursorPosX(ImGui::GetCursorPosX() + 7);
        ImGui::SetCursorPosY(ImGui::GetCursorPosY() - 3);
        if (ImGuiUtils::ImageButton(m_folderTexture->getTextureNativeHandler(),
                                    ImVec2(m_folderTexture->getWidth() + 6, m_folderTexture->getHeight() + 6),
                                    ImVec2(m_folderTexture->getWidth(), m_folderTexture->getHeight())).m_isLMBClicked)
        {
            char* dat = m_currentToolchainBuildToolPath.data();
            nfdresult_t result = NFD_OpenDialog({}, "", &dat);
            if (result == NFD_OKAY)
            {
                m_currentToolchainBuildToolPath = dat;
                setSelectedToolchainPath(m_currentToolchainBuildToolPath, ToolchainPathType::BUILD_TOOL);

                // std::printf("selected cmake %s\n", SGCore::Utils::toUTF8(m_selectedToolchain->getCMakePath().u16string()).c_str());
            }
        }
    }
}
