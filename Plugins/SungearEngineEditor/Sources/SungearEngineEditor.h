#ifndef SUNGEARENGINEEDITOR_H
#define SUNGEARENGINEEDITOR_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include "Views/MainView.h"

namespace SGE
{
    struct SungearEngineEditor : public SGCore::IPlugin
    {
        SungearEngineEditor();
        ~SungearEngineEditor() override;
        
        std::string load(const std::vector<std::string>& args) override;
        
        [[nodiscard]] SGCore::Ref<MainView> getMainView() const noexcept;
        
        SG_NOINLINE static SGCore::Ref<SungearEngineEditor> getInstance() noexcept;
        
    private:
        static inline SGCore::Ref<SungearEngineEditor> s_SungearEngineEditorInstance = SGCore::MakeRef<SungearEngineEditor>();
        
        void onUpdate(const double&, const double&) const noexcept;
        
        SGCore::Ref<MainView> m_mainView;
    };
}

#endif // SUNGEARENGINEEDITOR_H
