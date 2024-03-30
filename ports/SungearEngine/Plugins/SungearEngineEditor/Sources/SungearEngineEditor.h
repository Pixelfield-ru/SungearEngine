#ifndef SUNGEARENGINEEDITOR_H
#define SUNGEARENGINEEDITOR_H

#include <SGCore/PluginsSystem/IPlugin.h>
#include "Views/MainView.h"

struct SungearEngineEditor : public SGCore::IPlugin
{
	~SungearEngineEditor() override;

	std::string load(const std::vector<std::string>& args) override;
 
private:
    void onUpdate(const double&, const double&) const noexcept;
    
    SGE::MainView m_mainView;
};

#endif // SUNGEARENGINEEDITOR_H
