#ifndef SUNGEARENGINEEDITOR_V2_H
#define SUNGEARENGINEEDITOR_V2_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>

struct SungearEngineEditor_v2 : public SGCore::IPlugin
{
    SGCore::Event<void()> onConstructed;

    ~SungearEngineEditor_v2() override;

    void update(const double& dt, const double& fixedDt) final;
    void fixedUpdate(const double& dt, const double& fixedDt) final;

    std::string onConstruct(const std::vector<std::string>& args) final;

    SG_NOINLINE static SGCore::Ref<SungearEngineEditor_v2> getInstance() noexcept;

private:
    static inline SGCore::Ref<SungearEngineEditor_v2> s_SungearEngineEditor_v2Instance = SGCore::MakeRef<SungearEngineEditor_v2>();
};

#endif // SUNGEARENGINEEDITOR_V2_H
