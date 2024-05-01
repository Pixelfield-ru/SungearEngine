#ifndef SUNGEARENGINEEDITOR_H
#define SUNGEARENGINEEDITOR_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include <SGCore/Memory/AssetManager.h>
#include "Views/MainView.h"

namespace SGE
{
    struct SungearEngineEditor : public SGCore::IPlugin
    {
        ~SungearEngineEditor() override;
        
        std::string onConstruct(const std::vector<std::string>& args) override;
        
        [[nodiscard]] SGCore::Ref<MainView> getMainView() const noexcept;
        
        SG_NOINLINE static SGCore::Ref<SungearEngineEditor> getInstance() noexcept;
        SG_NOINLINE static SGCore::AssetManager& getAssetManager() noexcept;
        
    private:
        static inline SGCore::Ref<SungearEngineEditor> s_SungearEngineEditorInstance = SGCore::MakeRef<SungearEngineEditor>();
        static inline SGCore::AssetManager m_assetManager;
        
        void onUpdate(const double&, const double&) const noexcept;
        
        SGCore::Ref<MainView> m_mainView;
    };
}

#endif // SUNGEARENGINEEDITOR_H
