#ifndef SUNGEARENGINEEDITOR_H
#define SUNGEARENGINEEDITOR_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include <SGCore/Memory/AssetManager.h>
#include "Views/MainView.h"
#include "Project/Project.h"

#ifdef PLATFORM_OS_WINDOWS
#include <windows.h>
#include <winnt.h>
#endif

#include <SGCore/PluginsSystem/PluginProject.h>

namespace SGE
{
    struct SungearEngineEditor : public SGCore::IPlugin
    {
        SGCore::Threading::BaseThreadsPool<SGCore::Threading::LeastTasksCount> m_threadsPool { 2, false };

        Project m_currentProject;

        ~SungearEngineEditor() override;

        void update(const double& dt, const double& fixedDt) final;
        void fixedUpdate(const double& dt, const double& fixedDt) final;

        std::string onConstruct(const std::vector<std::string>& args) final;
        
        [[nodiscard]] SGCore::Ref<MainView> getMainView() const noexcept;
        
        SG_NOINLINE static SGCore::Ref<SungearEngineEditor> getInstance() noexcept;
        
    private:
        static inline SGCore::Ref<SungearEngineEditor> s_SungearEngineEditorInstance = SGCore::MakeRef<SungearEngineEditor>();
        
        SGCore::Ref<MainView> m_mainView;
    };
}

#endif // SUNGEARENGINEEDITOR_H
