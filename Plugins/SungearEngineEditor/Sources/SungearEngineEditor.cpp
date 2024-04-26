#include "SungearEngineEditor.h"
#include "PluginMain.h"

#include <SGUtils/Singleton.h>
#include <SGCore/ImGuiWrap/ViewsInjector.h>
#include <SGCore/Main/CoreMain.h>

SungearEngineEditor::~SungearEngineEditor()
{
    SGCore::CoreMain::getRenderTimer().onUpdate.disconnect<&SungearEngineEditor::onUpdate>(*this);
    std::cout << "sdfsdfssdgjfgh" << std::endl;
}

std::string SungearEngineEditor::load(const std::vector<std::string>& args)
{
	m_name = "SungearEngineEditor";
	m_version = "1.0.0";

    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&SungearEngineEditor::onUpdate>(*this);
    
	// No error.
	return "";
}

void SungearEngineEditor::onUpdate(const double&, const double&) const noexcept
{
    mainViewsInjector.renderViews();
}

