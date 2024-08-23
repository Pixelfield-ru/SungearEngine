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
            auto infoMsgsButtonClickInfo = ImGuiUtils::ImageButton(m_infoIcon->getTextureNativeHandler(),
                                                                   ImVec2(m_infoIcon->getWidth() + 6,
                                                                          m_infoIcon->getHeight() + 6),
                                                                   ImVec2(m_infoIcon->getWidth(),
                                                                          m_infoIcon->getHeight()));
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

            auto debugMsgsButtonClickInfo = ImGuiUtils::ImageButton(m_debugIcon->getTextureNativeHandler(),
                                                                    ImVec2(m_debugIcon->getWidth() + 6,
                                                                           m_debugIcon->getHeight() + 6),
                                                                    ImVec2(m_debugIcon->getWidth(),
                                                                           m_debugIcon->getHeight()));

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

            auto warningMsgsButtonClickInfo = ImGuiUtils::ImageButton(m_warnIcon->getTextureNativeHandler(),
                                                                      ImVec2(m_warnIcon->getWidth() + 6,
                                                                             m_warnIcon->getHeight() + 6),
                                                                      ImVec2(m_warnIcon->getWidth(),
                                                                             m_warnIcon->getHeight()));

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

            auto errorsMsgsButtonClickInfo = ImGuiUtils::ImageButton(m_errorIcon->getTextureNativeHandler(),
                                                                     ImVec2(m_errorIcon->getWidth() + 6,
                                                                            m_errorIcon->getHeight() + 6),
                                                                     ImVec2(m_errorIcon->getWidth(),
                                                                            m_errorIcon->getHeight()));

            if (errorsMsgsButtonClickInfo.m_isLMBClicked)
            {
                setCurrentLevel(SGCore::Logger::Level::LVL_ERROR);
            }

            if(errorsMsgsButtonClickInfo.m_isHovered)
            {
                ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(5, 5));
                ImGui::SetTooltip("Error messages");
                ImGui::PopStyleVar();
            }

            ImGui::SameLine();

            // ==================================================================

            auto criticalMsgsButtonClickInfo = ImGuiUtils::ImageButton(m_criticalIcon->getTextureNativeHandler(),
                                                                       ImVec2(m_criticalIcon->getWidth() + 6,
                                                                              m_criticalIcon->getHeight() + 6),
                                                                       ImVec2(m_criticalIcon->getWidth(),
                                                                              m_criticalIcon->getHeight()));

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
        }
        ImGui::EndChildFrame();
        ImGui::PopStyleVar(2);

        // ==================================================================
        // ==================================================================
        // ==================================================================

        std::vector<SGCore::Logger::LogMessage> messages;
        if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_INFO))
        {
            messages = SGCore::Logger::getDefaultLogger()->getMessagesWithLevel(SGCore::Logger::Level::LVL_INFO);
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_DEBUG))
        {
            messages = SGCore::Logger::getDefaultLogger()->getMessagesWithLevel(SGCore::Logger::Level::LVL_DEBUG);
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_WARN))
        {
            messages = SGCore::Logger::getDefaultLogger()->getMessagesWithLevel(SGCore::Logger::Level::LVL_WARN);
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_ERROR))
        {
            messages = SGCore::Logger::getDefaultLogger()->getMessagesWithLevel(SGCore::Logger::Level::LVL_ERROR);
        }
        else if(m_currentChosenLevel == SGCore::Logger::levelToString(SGCore::Logger::Level::LVL_CRITICAL))
        {
            messages = SGCore::Logger::getDefaultLogger()->getMessagesWithLevel(SGCore::Logger::Level::LVL_CRITICAL);
        }
        else
        {
            messages = SGCore::Logger::getDefaultLogger()->getAllMessages();
        }

        ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(12, 0));
        if (ImGui::BeginChildFrame(ImGui::GetID("LogMessages"), ImGui::GetContentRegionAvail()))
        {
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
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 0.0, 1.0, 0.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    case SGCore::Logger::Level::LVL_WARN:
                        ImGui::PushStyleColor(ImGuiCol_Text,
                                              ImGui::ColorConvertFloat4ToU32(
                                                      { 233.0f / 255.0f, 213.0f / 255.0f, 2.0f / 255.0f, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    case SGCore::Logger::Level::LVL_ERROR:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0, 0.0, 0.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                    case SGCore::Logger::Level::LVL_CRITICAL:
                        ImGui::PushStyleColor(ImGuiCol_Text, ImGui::ColorConvertFloat4ToU32({ 1.0, 0.0, 0.0, 1.0 }));
                        ImGui::TextUnformatted(logMessage.m_message.c_str());
                        break;
                }

                ImGui::PopStyleColor();
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
