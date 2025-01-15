#include "SungearEngineEditor-v2.h"

SungearEngineEditor_v2::~SungearEngineEditor_v2()
{

}

void SungearEngineEditor_v2::update(const double& dt, const double& fixedDt)
{

}

void SungearEngineEditor_v2::fixedUpdate(const double& dt, const double& fixedDt)
{

}

std::string SungearEngineEditor_v2::onConstruct(const std::vector<std::string>& args)
{
    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());
    
    m_name = "SungearEngineEditor-v2";
    m_version = "1.0.0";

    onConstructed();
    
    return "";
}

SGCore::Ref<SungearEngineEditor_v2> SungearEngineEditor_v2::getInstance() noexcept
{
    return s_SungearEngineEditor_v2Instance;
}