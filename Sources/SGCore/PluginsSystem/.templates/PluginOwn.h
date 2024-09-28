#ifndef ${upperPluginName}$_H
#define ${upperPluginName}$_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include <SGCore/Memory/AssetManager.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>

struct ${pluginName}$ : public SGCore::IPlugin
{
    ~${pluginName}$() override;

    void update(const double& dt, const double& fixedDt) final;
    void fixedUpdate(const double& dt, const double& fixedDt) final;

    std::string onConstruct(const std::vector<std::string>& args) final;

    SG_NOINLINE static SGCore::Ref<${pluginName}$> getInstance() noexcept;

private:
    static inline SGCore::Ref<${pluginName}$> s_${pluginName}$Instance = SGCore::MakeRef<${pluginName}$>();
};

#endif // ${upperPluginName}$_H
