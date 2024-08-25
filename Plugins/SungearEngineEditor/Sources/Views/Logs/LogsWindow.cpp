//
// Created by Ilya on 21.08.2024.
//

#include "LogsWindow.h"
#include "imgui_internal.h"

#include "ImGuiUtils.h"
#include "Styles/StylesManager.h"

SGE::LogsWindow::LogsWindow()
{
    m_infoIcon = StylesManager::getCurrentStyle()->m_infoIcon
            ->getSpecialization(24, 24)
            ->getTexture();

    m_debugIcon = StylesManager::getCurrentStyle()->m_greenBugIcon
            ->getSpecialization(24, 24)
            ->getTexture();

    m_warnIcon = StylesManager::getCurrentStyle()->m_warningIcon
            ->getSpecialization(24, 24)
            ->getTexture();

    m_errorIcon = StylesManager::getCurrentStyle()->m_errorIcon
            ->getSpecialization(24, 24)
            ->getTexture();

    m_criticalIcon = StylesManager::getCurrentStyle()->m_criticalIcon
            ->getSpecialization(24, 24)
            ->getTexture();

    m_clearLogsIcon = StylesManager::getCurrentStyle()->m_trashBinIcon
            ->getSpecialization(20, 20)
            ->getTexture();
}

bool SGE::LogsWindow::begin()
{
    return IView::begin();
}

void SGE::LogsWindow::renderBody()
{
    ImGui::PushStyleVar(ImGuiStyleVar_WindowTitleAlign, ImVec2(0.5, 0.5));
    ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));
    ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));

    ImGuiWindowClass windowClass;
    windowClass.DockNodeFlagsOverrideSet = ImGuiDockNodeFlags_AutoHideTabBar;
    ImGui::SetNextWindowClass(&windowClass);

    if(ImGui::Begin("LogView"))
    {
        ImGui::PushStyleVar(ImGuiStyleVar_FramePadding, ImVec2(5, 5));
        ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(10, 0));
        if (ImGui::BeginChildFrame(ImGui::GetID("LogLevelTypeButtonsView"), { ImGui::GetContentRegionAvail().x, 41 }))
        {
            ImGuiUtils::BeginCheckboxesGroup("LogLevelTypeCheckboxes");

            ImRectangleButtonInfo infoMsgsButtonInfo {
                .m_imageNativeHandler = m_infoIcon->getTextureNativeHandler(),
                .m_buttonSize = ImVec2(m_infoIcon->getWidth() + 6, m_infoIcon->getHeight() + 6),
                .m_imageSize = ImVec2(m_infoIcon->getWidth(), m_infoIcon->getHeight()),
                .m_name = "InfoLevelButton"
            };
            auto infoMsgsButtonClickInfo = ImGuiUtils::ImageButton(infoMsgsButtonInfo);
            if (infoMsgsButtonClickInfo.m_isLMBClicked)
            {
                setCurrentLevel(SGCore::Logger::Level::LVL_INFO);
            }

            if(infoMsgsButtonClickInfo.m_isHovered)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                ImGui::SetTooltip("Info messages");
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();

            // ==================================================================

            ImRectangleButtonInfo debugMsgsButtonInfo {
                    .m_imageNativeHandler = m_debugIcon->getTextureNativeHandler(),
                    .m_buttonSize = ImVec2(m_debugIcon->getWidth() + 6, m_debugIcon->getHeight() + 6),
                    .m_imageSize = ImVec2(m_debugIcon->getWidth(), m_debugIcon->getHeight()),
                    .m_name = "DebugLevelButton"
            };
            auto debugMsgsButtonClickInfo = ImGuiUtils::ImageButton(debugMsgsButtonInfo);
            if (debugMsgsButtonClickInfo.m_isLMBClicked)
            {
                setCurrentLevel(SGCore::Logger::Level::LVL_DEBUG);
            }

            if(debugMsgsButtonClickInfo.m_isHovered)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                ImGui::SetTooltip("Debug messages");
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();

            // ==================================================================

            ImRectangleButtonInfo warningMsgsButtonInfo {
                    .m_imageNativeHandler = m_warnIcon->getTextureNativeHandler(),
                    .m_buttonSize = ImVec2(m_warnIcon->getWidth() + 6, m_warnIcon->getHeight() + 6),
                    .m_imageSize = ImVec2(m_warnIcon->getWidth(), m_warnIcon->getHeight()),
                    .m_name = "WarnLevelButton"
            };
            auto warningMsgsButtonClickInfo = ImGuiUtils::ImageButton(warningMsgsButtonInfo);
            if (warningMsgsButtonClickInfo.m_isLMBClicked)
            {
                setCurrentLevel(SGCore::Logger::Level::LVL_WARN);
            }

            if(warningMsgsButtonClickInfo.m_isHovered)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                ImGui::SetTooltip("Warning messages");
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();

            // ==================================================================

            ImRectangleButtonInfo errorMsgsButtonInfo {
                    .m_imageNativeHandler = m_errorIcon->getTextureNativeHandler(),
                    .m_buttonSize = ImVec2(m_errorIcon->getWidth() + 6, m_errorIcon->getHeight() + 6),
                    .m_imageSize = ImVec2(m_errorIcon->getWidth(), m_errorIcon->getHeight()),
                    .m_name = "ErrorLevelButton"
            };
            auto errorMsgsButtonClickInfo = ImGuiUtils::ImageButton(errorMsgsButtonInfo);

            if (errorMsgsButtonClickInfo.m_isLMBClicked)
            {
                setCurrentLevel(SGCore::Logger::Level::LVL_ERROR);
            }

            if(errorMsgsButtonClickInfo.m_isHovered)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                ImGui::SetTooltip("Error messages");
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();

            // ==================================================================

            ImRectangleButtonInfo criticalMsgsButtonInfo {
                    .m_imageNativeHandler = m_criticalIcon->getTextureNativeHandler(),
                    .m_buttonSize = ImVec2(m_criticalIcon->getWidth() + 6, m_criticalIcon->getHeight() + 6),
                    .m_imageSize = ImVec2(m_criticalIcon->getWidth(), m_criticalIcon->getHeight()),
                    .m_name = "CriticalLevelButton"
            };
            auto criticalMsgsButtonClickInfo = ImGuiUtils::ImageButton(criticalMsgsButtonInfo);
            if (criticalMsgsButtonClickInfo.m_isLMBClicked)
            {
                setCurrentLevel(SGCore::Logger::Level::LVL_CRITICAL);
            }

            if(criticalMsgsButtonClickInfo.m_isHovered)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                ImGui::SetTooltip("Critical messages");
                ImGui::PopStyleVar();
            }

            ImGuiUtils::EndCheckboxesGroup();

            // ===============================================================================
            // ===============================================================================
            // ===============================================================================

            m_logTags = SGCore::Logger::getDefaultLogger()->getAllTags();
            m_logTags.insert(m_logTags.begin(), "all");

            m_currentSelectedTagIndex = (m_currentSelectedTagIndex >= m_logTags.size() || m_currentSelectedTagIndex < 0)
                                         ? 0 : m_currentSelectedTagIndex;

            const float rightContentWidth = 300.0f;

            ImGui::SameLine();
            ImGui::SetCursorPosX(ImGui::GetWindowSize().x - rightContentWidth);
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 6);
            ImGui::Text("Tag: ");
            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
            ImGui::SetNextItemWidth(rightContentWidth - ImGui::CalcTextSize("Tag: ").x - 15.0f -
                                            (m_clearLogsIcon->getWidth() + 6) - 10.0f);
            if(ImGui::BeginCombo("##LogTags", m_logTags[m_currentSelectedTagIndex].c_str()))
            {
                for(std::int64_t i = 0; i < m_logTags.size(); ++i)
                {
                    bool isTagSelected = (m_logTags[m_currentSelectedTagIndex] == m_logTags[i]);
                    if (ImGui::Selectable(m_logTags[i].c_str(), isTagSelected))
                    {
                        m_currentSelectedTagIndex = i;
                    }
                    if (isTagSelected)
                    {
                        // You may set the initial focus when opening the combo (scrolling + for keyboard navigation support)
                        ImGui::SetItemDefaultFocus();
                    }
                }

                ImGui::EndCombo();
            }

            ImGui::SameLine();
            ImGui::SetCursorPosY(ImGui::GetCursorPosY() + 2);
            if(ImGuiUtils::ImageButton(m_clearLogsIcon->getTextureNativeHandler(),
                                       ImVec2(m_clearLogsIcon->getWidth() + 6, m_clearLogsIcon->getHeight() + 6),
                                       ImVec2(m_clearLogsIcon->getWidth(), m_clearLogsIcon->getHeight())).m_isLMBClicked)
            {
                SGCore::Logger::Level currentLevel = SGCore::Logger::Level::LVL_INFO;

                if (m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_INFO))
                {
                    currentLevel = SGCore::Logger::Level::LVL_INFO;
                }
                else if (m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_DEBUG))
                {
                    currentLevel = SGCore::Logger::Level::LVL_DEBUG;
                }
                else if (m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_WARN))
                {
                    currentLevel = SGCore::Logger::Level::LVL_WARN;
                }
                else if (m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_ERROR))
                {
                    currentLevel = SGCore::Logger::Level::LVL_ERROR;
                }
                else if (m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_CRITICAL))
                {
                    currentLevel = SGCore::Logger::Level::LVL_CRITICAL;
                }
                else
                {
                    if(m_logTags[m_currentSelectedTagIndex] == "all")
                    {
                        SGCore::Logger::getDefaultLogger()->clearAllMessages();
                    }
                    else
                    {
                        SGCore::Logger::getDefaultLogger()->clearMessagesWithTag(m_logTags[m_currentSelectedTagIndex]);
                    }
                }

                if(!m_currentChosenLevel.empty())
                {
                    if(m_logTags[m_currentSelectedTagIndex] == "all")
                    {
                        SGCore::Logger::getDefaultLogger()->clearMessagesWithLevel(currentLevel);
                    }
                    else
                    {
                        SGCore::Logger::getDefaultLogger()->clearMessagesWithLevelAndTag(currentLevel, m_logTags[m_currentSelectedTagIndex]);
                    }
                }
            }
        }
        ImGui::EndChildFrame();
        ImGui::PopStyleVar(2);

        // ==================================================================
        // ==================================================================
        // ==================================================================

        const std::string& currentTag = m_logTags[m_currentSelectedTagIndex];
        SGCore::Logger::Level currentLevel = SGCore::Logger::Level::LVL_INFO;

        std::vector<SGCore::Logger::LogMessage> messages;
        if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_INFO))
        {
            currentLevel = SGCore::Logger::Level::LVL_INFO;
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_DEBUG))
        {
            currentLevel = SGCore::Logger::Level::LVL_DEBUG;
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_WARN))
        {
            currentLevel = SGCore::Logger::Level::LVL_WARN;
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_ERROR))
        {
            currentLevel = SGCore::Logger::Level::LVL_ERROR;
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_CRITICAL))
        {
            currentLevel = SGCore::Logger::Level::LVL_CRITICAL;
        }
        else
        {
            if(currentTag == "all")
            {
                messages = SGCore::Logger::getDefaultLogger()->getAllMessages();
            }
            else
            {
                messages =
                        SGCore::Logger::getDefaultLogger()->getMessagesWithTag(currentTag);
            }
        }

        if(messages.empty())
        {
            if(currentTag == "all")
            {
                messages = SGCore::Logger::getDefaultLogger()->getMessagesWithLevel(currentLevel);
            }
            else
            {
                messages =
                        SGCore::Logger::getDefaultLogger()->getMessagesWithLevelAndTag(currentLevel, currentTag);
            }
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
        if (ImGui::BeginChildFrame(ImGui::GetID("LogMessages"), ImGui::GetContentRegionAvail()))
        {
            if(ImGui::GetScrollY() == m_logsCurrentScrollY)
            {
                m_enableAutoScroll = true;
            }
            else
            {
                m_enableAutoScroll = false;
            }

            for (const SGCore::Logger::LogMessage& logMessage : messages)
            {
                switch (logMessage.m_level)
                {
                    case SGCore::Logger::Level::LVL_INFO:
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0, 1.0, 1.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    }
                    case SGCore::Logger::Level::LVL_DEBUG:
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 0.0, 1.0, 0.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    }
                    case SGCore::Logger::Level::LVL_WARN:
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text,
                                              ImGui::ColorConvertFloat4ToU32(
                                                      { 233.0f / 255.0f, 213.0f / 255.0f, 2.0f / 255.0f, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    }
                    case SGCore::Logger::Level::LVL_ERROR:
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0, 0.0, 0.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    }
                    case SGCore::Logger::Level::LVL_CRITICAL:
                    {
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0, 0.0, 0.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    }
                }

                ImGui::PopStyleColor();
            }

            m_logsCurrentScrollY = ImGui::GetScrollMaxY();

            if(m_enableAutoScroll)
            {
                ImGui::SetScrollY(m_logsCurrentScrollY);
            }
        }
        ImGui::EndChildFrame();
        ImGui::PopStyleVar();
    }
    ImGui::End();

    ImGui::PopStyleVar(3);
}

void SGE::LogsWindow::end()
{
    IView::end();
}

void SGE::LogsWindow::setCurrentLevel(SGCore::Logger::Level level) noexcept
{
    const auto lvlStr = SGCore::Logger::levelToString(level);
    if(m_currentChosenLevel == lvlStr)
    {
        m_currentChosenLevel = "";
        return;
    }

    m_currentChosenLevel = lvlStr;
}
