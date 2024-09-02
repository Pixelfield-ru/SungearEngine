//
// Created by stuka on 01.09.2024.
//

#ifndef SUNGEARENGINEEDITOR_DIALOGWINDOWSMANAGER_H
#define SUNGEARENGINEEDITOR_DIALOGWINDOWSMANAGER_H

#include "Window.h"

#include <SGCore/Logger/Logger.h>
#include <SGCore/Graphics/API/ITexture2D.h>

namespace SGE
{
    struct DialogWindow : Window
    {
        friend struct DialogWindowsManager;

        SGCore::Logger::Level m_level = SGCore::Logger::Level::LVL_INFO;
    };

    struct DialogWindowsManager
    {
        static void init() noexcept;

        static void renderWindows() noexcept;

        static DialogWindow createThreeButtonsWindow(const std::string& windowName,
                                                     const std::string& firstButtonText,
                                                     const std::string& secondButtonText,
                                                     const std::string& thirdButtonText) noexcept;

        static DialogWindow createTwoButtonsWindow(const std::string& windowName,
                                                   const std::string& firstButtonText,
                                                   const std::string& secondButtonText) noexcept;

        static DialogWindow createOneButtonWindow(const std::string& windowName,
                                                  const std::string& firstButtonText) noexcept;

        static void addDialogWindow(DialogWindow& dialogWindow) noexcept;
        SG_NOINLINE static const std::vector<DialogWindow>& getDialogWindows() noexcept;

    private:
        static inline SGCore::Ref<SGCore::ITexture2D> m_infoIcon;
        static inline SGCore::Ref<SGCore::ITexture2D> m_debugIcon;
        static inline SGCore::Ref<SGCore::ITexture2D> m_warnIcon;
        static inline SGCore::Ref<SGCore::ITexture2D> m_errorIcon;
        static inline SGCore::Ref<SGCore::ITexture2D> m_criticalIcon;

        static inline std::vector<DialogWindow> m_dialogWindows;

        static inline SGCore::Ref<SGCore::UniqueNamesManager> m_windowsNamesManager;
    };
}

#endif //SUNGEARENGINEEDITOR_DIALOGWINDOWSMANAGER_H
