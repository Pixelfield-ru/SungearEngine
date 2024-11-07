//
// Created by Ilya on 21.08.2024.
//

#ifndef SUNGEARENGINEEDITOR_LOGSWINDOW_H
#define SUNGEARENGINEEDITOR_LOGSWINDOW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>
#include <SGCore/Memory/AssetRef.h>
#include <SGCore/Logger/Logger.h>

namespace SGE
{
    struct LogsWindow : public SGCore::ImGuiWrap::IView
    {
        LogsWindow();
        LogsWindow(const LogsWindow&) = default;
        LogsWindow(LogsWindow&&) = default;

        bool begin() final;
        void renderBody() final;
        void end() final;

        void setCurrentLevel(SGCore::Logger::Level level) noexcept;

    private:
        SGCore::AssetRef<SGCore::ITexture2D> m_infoIcon;
        SGCore::AssetRef<SGCore::ITexture2D> m_debugIcon;
        SGCore::AssetRef<SGCore::ITexture2D> m_warnIcon;
        SGCore::AssetRef<SGCore::ITexture2D> m_errorIcon;
        SGCore::AssetRef<SGCore::ITexture2D> m_criticalIcon;

        SGCore::AssetRef<SGCore::ITexture2D> m_clearLogsIcon;

        std::string m_currentChosenLevel;

        bool m_enableAutoScroll = false;
        float m_logsCurrentScrollY = 0.0f;

        std::int64_t m_currentSelectedTagIndex = 0;
        std::vector<std::string> m_logTags;
    };
}

#endif //SUNGEARENGINEEDITOR_LOGSWINDOW_H
