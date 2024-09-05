#include "SungearEngineEditor.h"
#include "PluginMain.h"
#include "Styles/StylesManager.h"

#include "SGCore/Utils/Singleton.h"
#include "Views/DialogWindowsManager.h"
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Input/InputManager.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>
#include <SGCore/Logger/Logger.h>

SGE::SungearEngineEditor::~SungearEngineEditor()
{
    SGCore::ImGuiWrap::IView::getRoot()->removeChild(m_mainView);
}

std::string SGE::SungearEngineEditor::onConstruct(const std::vector<std::string>& args)
{
    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

	m_name = "SungearEngineEditor";
	m_version = "1.0.0";

    EngineSettings::getInstance()->load("configs/engine_settings.json");

    StylesManager::init();
    DialogWindowsManager::init();
    
    m_mainView = SGCore::MakeRef<MainView>();
    SGCore::ImGuiWrap::IView::getRoot()->addChild(m_mainView);

    LOG_I(SGEDITOR_TAG, "Info msg")
    LOG_D(SGEDITOR_TAG, "Debug msg")
    LOG_W(SGEDITOR_TAG, "Warning msg")
    LOG_E(SGEDITOR_TAG, "Error msg")
    LOG_C(SGEDITOR_TAG, "Critical msg")

    const char* sungearEngineRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    if(sungearEngineRoot)
    {
        s_sungearEngineRootPath = sungearEngineRoot;
    }

    checkSungearEngineEnvironmentRootPathValidity();

	// No error.
	return "";
}

void SGE::SungearEngineEditor::update(const double& dt, const double& fixedDt)
{
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_R))
    {
        /*try
        {
            // CRASH HERE ======================
            void* ptr = nullptr;
            *((int*) ptr) = 5;
            // ==================================
        }
        catch(const std::exception& e)
        {
            std::printf("Exception: %s\n", e.what());
        }*/
        /*std::ofstream ofs(std::filesystem::current_path().string() + "/imgui.ini", std::ios::trunc | std::ios::out);

        SGCore::ImGuiWrap::ImGuiLayer::destroy();
        SGCore::ImGuiWrap::ImGuiLayer::init();
        ImGui::LoadIniSettingsFromDisk((std::filesystem::current_path().string() + "/imgui.ini").c_str());
        StylesManager::init();*/
    }
}

void SGE::SungearEngineEditor::fixedUpdate(const double& dt, const double& fixedDt)
{

}

SGCore::Ref<SGE::MainView> SGE::SungearEngineEditor::getMainView() const noexcept
{
    return m_mainView;
}

SGCore::Ref<SGE::SungearEngineEditor> SGE::SungearEngineEditor::getInstance() noexcept
{
    return s_SungearEngineEditorInstance;
}

const std::filesystem::path& SGE::SungearEngineEditor::getSungearEngineRootPath() noexcept
{
    return s_sungearEngineRootPath;
}

bool SGE::SungearEngineEditor::checkSungearEngineEnvironmentRootPathValidity(const std::string& additionalWarningMessage) noexcept
{
    const char* sungearEngineRoot = std::getenv("SUNGEAR_SOURCES_ROOT");
    if(!sungearEngineRoot)
    {
        const std::string errorMsg = "The 'SUNGEAR_SOURCES_ROOT' environment variable does not exist. "
                                     "Make sure that you have added 'SUNGEAR_SOURCES_ROOT' to the environment variables. "
                                     "In the variable value, specify the path to the Sungear Engine. "
                                     "Until then, you will not be able to build the project, as well as some other features of the engine.\n" +
                                     additionalWarningMessage;

        auto sungearEngineRootErrorDialog = DialogWindowsManager::createOneButtonWindow("Incorrect environment variable", "OK");
        sungearEngineRootErrorDialog.onCustomBodyRenderListener = [errorMsg]() {
            ImGui::SameLine();
            ImGui::TextWrapped(errorMsg.c_str());
        };
        DialogWindowsManager::addDialogWindow(sungearEngineRootErrorDialog);

        LOG_E_UNFORMATTED("SGEditor", errorMsg)

        return false;
    }
    else
    {
        const std::filesystem::path sungearEngineIncludeCMakeFile = s_sungearEngineRootPath / "cmake/SungearEngineInclude.cmake";
        if (!std::filesystem::exists(s_sungearEngineRootPath) ||
            !std::filesystem::exists(sungearEngineIncludeCMakeFile))
        {
            const std::string errorMsg = "The 'SUNGEAR_SOURCES_ROOT' environment variable contains an invalid value. "
                                         "Make sure that the 'SUNGEAR_SOURCES_ROOT' environment variable contains the correct value and indeed points to the Sungear Engine root folder. "
                                         "Until then, you will not be able to build the project, as well as some other features of the engine.\n" +
                                         additionalWarningMessage;

            auto sungearEngineRootErrorDialog = DialogWindowsManager::createOneButtonWindow("Incorrect environment variable", "OK");
            sungearEngineRootErrorDialog.onCustomBodyRenderListener = [errorMsg]() {
                ImGui::SameLine();
                ImGui::TextWrapped(errorMsg.c_str());
            };
            DialogWindowsManager::addDialogWindow(sungearEngineRootErrorDialog);

            LOG_E_UNFORMATTED("SGEditor", errorMsg)

            return false;
        }
    }

    return true;
}


