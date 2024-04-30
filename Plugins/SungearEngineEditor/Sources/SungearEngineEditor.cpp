#include "SungearEngineEditor.h"
#include "PluginMain.h"
#include "Styles/StylesManager.h"

#include <SGUtils/Singleton.h>
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Input/InputManager.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>

SGE::SungearEngineEditor::SungearEngineEditor()
{
    m_mainView = SGCore::MakeRef<MainView>();
    SGCore::ImGuiWrap::IView::getRoot()->addChild(m_mainView);
    
    SGCore::CoreMain::getRenderTimer().onUpdate.connect<&SungearEngineEditor::onUpdate>(*this);
}

SGE::SungearEngineEditor::~SungearEngineEditor()
{
    SGCore::CoreMain::getRenderTimer().onUpdate.disconnect<&SungearEngineEditor::onUpdate>(*this);
    SGCore::ImGuiWrap::IView::getRoot()->removeChild(m_mainView);
}

std::string SGE::SungearEngineEditor::load(const std::vector<std::string>& args)
{
	m_name = "SungearEngineEditor";
	m_version = "1.0.0";
    
    std::cout << "SGEDITOR PATH: " << getLocalPath() << std::endl;
    
    StylesManager::init();
    
	// No error.
	return "";
}

void SGE::SungearEngineEditor::onUpdate(const double&, const double&) const noexcept
{
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_R))
    {
        std::cout << std::filesystem::current_path().string() + "/imgui.ini" << std::endl;
        std::filesystem::remove(std::filesystem::current_path().string() + "/imgui.ini");
        SGCore::ImGuiWrap::ImGuiLayer::reload();
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

SGCore::AssetManager& SGE::SungearEngineEditor::getAssetManager() noexcept
{
    return m_assetManager;
}

