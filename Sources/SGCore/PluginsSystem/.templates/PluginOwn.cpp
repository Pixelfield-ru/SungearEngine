#include "${pluginName}$.h"

${pluginName}$::~${pluginName}$()
{

}

void ${pluginName}$::update(const double& dt, const double& fixedDt)
{

}

void ${pluginName}$::fixedUpdate(const double& dt, const double& fixedDt)
{

}

std::string ${pluginName}$::onConstruct(const std::vector<std::string>& args)
{
    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());
    
    m_name = "${pluginName}$";
    m_version = "1.0.0";
    
    return "";
}

SGCore::Ref<${pluginName}$> ${pluginName}$::getInstance() noexcept
{
    return s_${pluginName}$Instance;
}