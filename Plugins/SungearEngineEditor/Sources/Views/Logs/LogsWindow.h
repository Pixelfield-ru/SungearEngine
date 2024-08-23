//
// Created by Ilya on 21.08.2024.
//

#ifndef SUNGEARENGINEEDITOR_LOGSWINDOW_H
#define SUNGEARENGINEEDITOR_LOGSWINDOW_H

#include <SGCore/ImGuiWrap/Views/IView.h>
#include <SGCore/Graphics/API/ITexture2D.h>
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
        SGCore::Ref<SGCore::ITexture2D> m_infoIcon;
        SGCore::Ref<SGCore::ITexture2D> m_debugIcon;
        SGCore::Ref<SGCore::ITexture2D> m_warnIcon;
        SGCore::Ref<SGCore::ITexture2D> m_errorIcon;
        SGCore::Ref<SGCore::ITexture2D> m_criticalIcon;

        std::string m_currentChosenLevel;
    };
}

#endif //SUNGEARENGINEEDITOR_LOGSWINDOW_H
