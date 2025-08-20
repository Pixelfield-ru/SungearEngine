//
// Created by stuka on 01.09.2024.
//

#include "DialogWindowsManager.h"
#include "Styles/StylesManager.h"
#include <SGCore/Input/InputManager.h>

void SGE::DialogWindowsManager::init() noexcept
{
    m_infoIcon = StylesManager::getCurrentStyle()->m_infoIcon
            ->getSpecialization(30, 30)
            ->getTexture();

    m_debugIcon = StylesManager::getCurrentStyle()->m_greenBugIcon
            ->getSpecialization(30, 30)
            ->getTexture();

    m_warnIcon = StylesManager::getCurrentStyle()->m_warningIcon
            ->getSpecialization(30, 30)
            ->getTexture();

    m_errorIcon = StylesManager::getCurrentStyle()->m_errorIcon
            ->getSpecialization(30, 30)
            ->getTexture();

    m_criticalIcon = StylesManager::getCurrentStyle()->m_criticalIcon
            ->getSpecialization(30, 30)
            ->getTexture();

    m_windowsNamesManager = SGCore::MakeRef<SGCore::UniqueNamesManager>();
}

void SGE::DialogWindowsManager::renderWindows() noexcept
{
    if(m_dialogWindows.empty()) return;

    auto firstWindow = *m_dialogWindows.begin();
    if(!firstWindow->isActive())
    {
        m_dialogWindows.pop_front();
    }
    else
    {
        firstWindow->render();
    }
}

void SGE::DialogWindowsManager::addDialogWindow(const SGCore::Ref<DialogWindow>& dialogWindow) noexcept
{
    m_dialogWindows.push_front(dialogWindow);
    auto lastDialogWnd = *m_dialogWindows.begin();
    // adding listener to onRenderBody for rendering image
    lastDialogWnd->onIconRenderListener = [lastDialogWndPtr = lastDialogWnd.get()]() {
        switch(lastDialogWndPtr->m_level)
        {
            case SGCore::Logger::Level::LVL_INFO:
            {
                ImGui::Image((ImTextureID) m_infoIcon->getTextureNativeHandler(),
                             ImVec2(m_infoIcon->getWidth(), m_infoIcon->getHeight()));

                break;
            }
            case SGCore::Logger::Level::LVL_DEBUG:
            {
                ImGui::Image((ImTextureID) m_debugIcon->getTextureNativeHandler(),
                             ImVec2(m_debugIcon->getWidth(), m_debugIcon->getHeight()));

                break;
            }
            case SGCore::Logger::Level::LVL_WARN:
            {
                ImGui::Image((ImTextureID) m_warnIcon->getTextureNativeHandler(),
                             ImVec2(m_warnIcon->getWidth(), m_warnIcon->getHeight()));

                break;
            }
            case SGCore::Logger::Level::LVL_ERROR:
            {
                ImGui::Image((ImTextureID) m_errorIcon->getTextureNativeHandler(),
                             ImVec2(m_errorIcon->getWidth(), m_errorIcon->getHeight()));

                break;
            }
            case SGCore::Logger::Level::LVL_CRITICAL:
            {
                ImGui::Image((ImTextureID) m_criticalIcon->getTextureNativeHandler(),
                             ImVec2(m_criticalIcon->getWidth(), m_criticalIcon->getHeight()));

                break;
            }
        }
    };

    lastDialogWnd->onRenderBody += lastDialogWnd->onIconRenderListener;
    lastDialogWnd->onRenderBody += lastDialogWnd->onCustomBodyRenderListener;
}

const std::deque<SGCore::Ref<SGE::DialogWindow>>& SGE::DialogWindowsManager::getDialogWindows() noexcept
{
    return m_dialogWindows;
}

SGCore::Ref<SGE::DialogWindow>
SGE::DialogWindowsManager::createThreeButtonsWindow(const std::string& windowName,
                                                    const std::string& firstButtonText,
                                                    const std::string& secondButtonText,
                                                    const std::string& thirdButtonText) noexcept
{
    SGCore::Ref<DialogWindow> window = SGCore::MakeRef<DialogWindow>();

    window->m_name.attachToManager(m_windowsNamesManager);
    window->m_name = windowName;
    window->m_isPopupWindow = true;
    window->m_bodyMinSize = { 400, 100 };

    window->addButton({
                             .m_text = firstButtonText,
                             .m_name = firstButtonText,
                             .isFastClicked = [](auto& self) -> bool
                             {
                                 return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER);
                             },
                             .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView)
                             {
                                 parentView->setActive(false);
                             },
                             .m_color = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1),
                             .m_hoveredColor = ImVec4(0 / 255.0f, 70 / 255.0f, 110 / 255.0f, 1),
                             .m_borderColor = {0, 0, 0, 0},
                             .m_borderShadowColor = {0, 0, 0, 0},
                             .m_size = {75, 0}
                     }
    );

    window->addButton({
                             .m_text = secondButtonText,
                             .m_name = secondButtonText,
                             .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView)
                             {
                                 parentView->setActive(false);
                             },
                             .m_size = {75, 0}
                     }
    );

    window->addButton({
                             .m_text = thirdButtonText,
                             .m_name = thirdButtonText,
                             .isFastClicked = [](auto& self) -> bool
                             {
                                 return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE);
                             },
                             .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView)
                             {
                                 parentView->setActive(false);
                             },
                             .m_size = {75, 0}
                     }
    );

    return window;
}

SGCore::Ref<SGE::DialogWindow>
SGE::DialogWindowsManager::createTwoButtonsWindow(const std::string& windowName,
                                                  const std::string& firstButtonText,
                                                  const std::string& secondButtonText) noexcept
{
    SGCore::Ref<DialogWindow> window = SGCore::MakeRef<DialogWindow>();

    window->m_name.attachToManager(m_windowsNamesManager);
    window->m_name = windowName;
    window->m_isPopupWindow = true;
    window->m_bodyMinSize = { 400, 100 };

    window->addButton({
                             .m_text = firstButtonText,
                             .m_name = firstButtonText,
                             .isFastClicked = [](auto& self) -> bool
                             {
                                 return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER);
                             },
                             .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView)
                             {
                                 parentView->setActive(false);
                             },
                             .m_color = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1),
                             .m_hoveredColor = ImVec4(0 / 255.0f, 70 / 255.0f, 110 / 255.0f, 1),
                             .m_borderColor = {0, 0, 0, 0},
                             .m_borderShadowColor = {0, 0, 0, 0},
                             .m_size = {150, 0}
                     }
    );

    window->addButton({
                             .m_text = secondButtonText,
                             .m_name = secondButtonText,
                             .isFastClicked = [](auto& self) -> bool
                             {
                                 return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ESCAPE);
                             },
                             .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView)
                             {
                                 parentView->setActive(false);
                             },
                             .m_size = {75, 0}
                     }
    );

    return window;
}

SGCore::Ref<SGE::DialogWindow> SGE::DialogWindowsManager::createOneButtonWindow(const std::string& windowName,
                                                                                const std::string& firstButtonText) noexcept
{
    SGCore::Ref<DialogWindow> window = SGCore::MakeRef<DialogWindow>();

    window->m_name.attachToManager(m_windowsNamesManager);
    window->m_name = windowName;
    window->m_isPopupWindow = true;
    window->m_bodyMinSize = { 400, 100 };

    window->addButton({
                             .m_text = firstButtonText,
                             .m_name = firstButtonText,
                             .isFastClicked = [](auto& self) -> bool
                             {
                                 return SGCore::InputManager::getMainInputListener()->keyboardKeyPressed(SGCore::KeyboardKey::KEY_ENTER);
                             },
                             .onClicked = [](auto& self, SGCore::ImGuiWrap::IView* parentView)
                             {
                                 parentView->setActive(false);
                             },
                             .m_color = ImVec4(10 / 255.0f, 80 / 255.0f, 120 / 255.0f, 1),
                             .m_hoveredColor = ImVec4(0 / 255.0f, 70 / 255.0f, 110 / 255.0f, 1),
                             .m_borderColor = {0, 0, 0, 0},
                             .m_borderShadowColor = {0, 0, 0, 0},
                             .m_size = {75, 0}
                     }
    );

    return window;
}
