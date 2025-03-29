//
// Created by stuka on 01.09.2024.
//

#ifndef SUNGEARENGINEEDITOR_DIALOGWINDOWSMANAGER_H
#define SUNGEARENGINEEDITOR_DIALOGWINDOWSMANAGER_H

#include "Window.h"

#include <queue>

#include <SGCore/Logger/Logger.h>
#include <SGCore/Graphics/API/ITexture2D.h>

namespace SGE
{
    struct DialogWindow : Window
    {
        friend struct DialogWindowsManager;

        SGCore::Logger::Level m_level = SGCore::Logger::Level::LVL_INFO;

        SGCore::Slot<void()> onCustomBodyRenderListener;

    private:
        SGCore::Slot<void()> onIconRenderListener;
    };

    struct DialogWindowsManager
    {
        static void init() noexcept;

        static void renderWindows() noexcept;

        static SGCore::Ref<DialogWindow> createThreeButtonsWindow(const std::string& windowName,
                                                                  const std::string& firstButtonText,
                                                                  const std::string& secondButtonText,
                                                                  const std::string& thirdButtonText) noexcept;

        static SGCore::Ref<DialogWindow> createTwoButtonsWindow(const std::string& windowName,
                                                                const std::string& firstButtonText,
                                                                const std::string& secondButtonText) noexcept;

        static SGCore::Ref<DialogWindow> createOneButtonWindow(const std::string& windowName,
                                                               const std::string& firstButtonText) noexcept;

        static void addDialogWindow(const SGCore::Ref<DialogWindow>& dialogWindow) noexcept;
        SG_NOINLINE static const std::deque<SGCore::Ref<DialogWindow>>& getDialogWindows() noexcept;

    private:
        static inline SGCore::AssetRef<SGCore::ITexture2D> m_infoIcon;
        static inline SGCore::AssetRef<SGCore::ITexture2D> m_debugIcon;
        static inline SGCore::AssetRef<SGCore::ITexture2D> m_warnIcon;
        static inline SGCore::AssetRef<SGCore::ITexture2D> m_errorIcon;
        static inline SGCore::AssetRef<SGCore::ITexture2D> m_criticalIcon;

        static inline std::deque<SGCore::Ref<DialogWindow>> m_dialogWindows;

        static inline SGCore::Ref<SGCore::UniqueNamesManager> m_windowsNamesManager;
    };
}

#endif //SUNGEARENGINEEDITOR_DIALOGWINDOWSMANAGER_H
