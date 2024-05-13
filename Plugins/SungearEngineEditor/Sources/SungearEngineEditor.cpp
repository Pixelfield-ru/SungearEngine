#include "SungearEngineEditor.h"
#include "PluginMain.h"
#include "Styles/StylesManager.h"

#include <SGUtils/Singleton.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Input/InputManager.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>

SGE::SungearEngineEditor::~SungearEngineEditor()
{
    SGCore::CoreMain::getRenderTimer().onUpdate.disconnect<&SungearEngineEditor::onUpdate>(*this);
    SGCore::ImGuiWrap::IView::getRoot()->removeChild(m_mainView);
}

std::string SGE::SungearEngineEditor::onConstruct(const std::vector<std::string>& args)
{
	m_name = "SungearEngineEditor";
	m_version = "1.0.0";
    
    std::cout << "SGEDITOR PATH: " << getLocalPath() << std::endl;
    
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&SungearEngineEditor::onUpdate>(*this);

    std::cout << "FIRST" << std::endl;

    StylesManager::init();

    std::cout << "SECOND" << std::endl;
    
    m_mainView = SGCore::MakeRef<MainView>();
    SGCore::ImGuiWrap::IView::getRoot()->addChild(m_mainView);
    
	// No error.
	return "";
}

void SGE::SungearEngineEditor::onUpdate(const double&, const double&) const noexcept
{
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_R))
    {
        /*std::ofstream ofs(std::filesystem::current_path().string() + "/imgui.ini", std::ios::trunc | std::ios::out);
        
        SGCore::ImGuiWrap::ImGuiLayer::destroy();
        SGCore::ImGuiWrap::ImGuiLayer::init();
        ImGui::LoadIniSettingsFromDisk((std::filesystem::current_path().string() + "/imgui.ini").c_str());
        StylesManager::init();*/
    }
}

SGCore::Ref<SGE::MainView> SGE::SungearEngineEditor::getMainView() const noexcept
{
    return m_mainView;
}

SGCore::Ref<SGE::SungearEngineEditor> SGE::SungearEngineEditor::getInstance() noexcept
{
    return s_SungearEngineEditorInstance;
}


