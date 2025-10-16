#ifndef SUNGEARENGINEEDITOR_H
#define SUNGEARENGINEEDITOR_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include <SGCore/Memory/AssetManager.h>
#include "Views/MainView.h"
#include "Project/Project.h"

#ifdef SG_PLATFORM_OS_WINDOWS
#include <windows.h>
#include <winnt.h>
#endif

#include <SGCore/PluginsSystem/PluginProject.h>

#define SGEDITOR_TAG "SGEditor"

namespace SGE
{
    struct SungearEngineEditor : public SGCore::IPlugin
    {
        SGCore::Threading::BaseThreadsPool<SGCore::Threading::LeastTasksCount> m_threadsPool { 2, false };

        SGCore::Ref<Project> m_currentProject;

        SGCore::Signal<void()> onConstructed;

        ~SungearEngineEditor() override;

        void update(const double& dt, const double& fixedDt) final;
        void fixedUpdate(const double& dt, const double& fixedDt) final;

        std::string onConstruct(const std::vector<std::string>& args) final;
        
        [[nodiscard]] SGCore::Ref<MainView> getMainView() const noexcept;
        
        SG_NOINLINE static SGCore::Ref<SungearEngineEditor> getInstance() noexcept;

        /**
         * Checks Sungear Engine environment root path validity and logs warning if path is not correct\n
         * or environment variable 'SUNGEAR_SOURCES_ROOT' was not found.
         * @return Is Sungear Engine environment root path valid.
         */
        static bool checkSungearEngineEnvironmentRootPathValidity(const std::string& additionalWarningMessage = "") noexcept;

        static void loadDefaultUIConfig() noexcept;

    private:
        static inline SGCore::Ref<SungearEngineEditor> s_SungearEngineEditorInstance = SGCore::MakeRef<SungearEngineEditor>();
        
        SGCore::Ref<MainView> m_mainView;
    };
}

#endif // SUNGEARENGINEEDITOR_H
