#include "SungearEngineEditor.h"
#include "PluginMain.h"
#include "Styles/StylesManager.h"

#include "SGCore/Utils/Singleton.h"
#include "Views/DialogWindowsManager.h"
#include <SGCore/Main/CoreMain.h>
#include <SGCore/Input/PCInput.h>
#include <SGCore/ImGuiWrap/ImGuiLayer.h>
#include <SGCore/Logger/Logger.h>
#include <SGCore/Utils/StringInterpolation/InterpolationResolver.h>
#include <SGCore/Graphics/API/IShader.h>
#include <SGCore/Motion/IK/IKJoint.h>
#include <SGCore/Motion/IK/IKRootJoint.h>
#include <SGCore/Render/DebugDraw.h>
#include <SGCore/Render/RenderPipelinesManager.h>
#include <SGCore/Render/PBRRP/PBRRenderPipeline.h>

#include "Resources.h"

SGE::SungearEngineEditor::~SungearEngineEditor()
{
    SGCore::ImGuiWrap::IView::getRoot()->removeChild(m_mainView);
}

std::string SGE::SungearEngineEditor::onConstruct(const std::vector<std::string>& args)
{
    auto pbrrpPipeline = SGCore::RenderPipelinesManager::instance().createRenderPipeline<SGCore::PBRRenderPipeline>();
    SGCore::RenderPipelinesManager::instance().registerRenderPipeline(pbrrpPipeline);
    SGCore::RenderPipelinesManager::instance().setCurrentRenderPipeline<SGCore::PBRRenderPipeline>();

    SGCore::PathInterpolationMarkupSpec::setKey("sgEditorPath", getLocalPath());

    ImGui::SetCurrentContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());
    ImGuizmo::SetImGuiContext(SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext());

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

    checkSungearEngineEnvironmentRootPathValidity();

    Resources::loadStandardResources();

    loadDefaultUIConfig();

    // SGCore::Event<void()> onClicked;

    onConstructed();

	// No error.
	return "";
}

void SGE::SungearEngineEditor::update(const double& dt, const double& fixedDt)
{
    const auto currentScene = SGCore::Scene::getCurrentScene();
    const auto renderPipeline = SGCore::RenderPipelinesManager::instance().getCurrentRenderPipeline();
    if(currentScene && renderPipeline)
    {
        const auto debugDraw = renderPipeline->getRenderPass<SGCore::DebugDraw>();
        if(debugDraw)
        {
            const auto ikJointsView = currentScene->getECSRegistry()->view<SGCore::Transform, SGCore::IKJoint>();
            const auto ikRootJointsView = currentScene->getECSRegistry()->view<SGCore::Transform, SGCore::IKRootJoint>();

            ikJointsView.each([&debugDraw](const SGCore::Transform::reg_t& jointTransform, const auto&) {
                debugDraw->drawAABB(jointTransform->m_finalTransform.m_position - 0.01f, jointTransform->m_finalTransform.m_position + 0.01f, { 1.0f, 0.5f, 0.5f, 1.0f });
            });

            ikRootJointsView.each([&debugDraw](const SGCore::Transform::reg_t& jointTransform, const auto&) {
                debugDraw->drawAABB(jointTransform->m_finalTransform.m_position - 0.01f, jointTransform->m_finalTransform.m_position + 0.01f, { 0.3f, 1.0f, 0.1f, 1.0f });
            });
        }
    }

    if(SGCore::Input::PC::keyboardKeyReleased(SGCore::Input::KeyboardKey::KEY_Y) && !ImGui::GetIO().WantTextInput)
    {
        SGCore::AssetManager::getInstance()->createPackage("./", "assets");

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

    if(SGCore::Input::PC::keyboardKeyPressed(SGCore::Input::KeyboardKey::KEY_L) && !ImGui::GetIO().WantTextInput)
    {
        SGCore::AssetManager::getInstance()->loadPackage("./", "assets");
    }

    if(SGCore::Input::PC::keyboardKeyPressed(SGCore::Input::KeyboardKey::KEY_F11))
    {
        SGCore::CoreMain::getWindow().setFullscreen(!SGCore::CoreMain::getWindow().isFullscreen());
    }

    if(SGCore::Input::PC::keyboardKeyPressed(SGCore::Input::KeyboardKey::KEY_N) && !ImGui::GetIO().WantTextInput)
    {
        auto shaders = SGCore::AssetManager::getInstance()->getAssetsWithType<SGCore::IShader>();
        for(const auto& shader : shaders)
        {
            shader->reloadFromDisk();
        }
        // SGCore::CoreMain::getWindow().setFullscreen(!SGCore::CoreMain::getWindow().isFullscreen());
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
        sungearEngineRootErrorDialog->onCustomBodyRenderListener = [errorMsg]() {
            ImGui::SameLine();
            ImGui::TextWrapped(errorMsg.c_str());
        };
        DialogWindowsManager::addDialogWindow(sungearEngineRootErrorDialog);

        LOG_E_UNFORMATTED("SGEditor", errorMsg)

        return false;
    }
    else
    {
        const std::filesystem::path sungearEngineIncludeCMakeFile = SGCore::CoreMain::getSungearEngineRootPath() / "cmake/SungearEngineInclude.cmake";
        if (!std::filesystem::exists(SGCore::CoreMain::getSungearEngineRootPath()) ||
            !std::filesystem::exists(sungearEngineIncludeCMakeFile))
        {
            const std::string errorMsg = "The 'SUNGEAR_SOURCES_ROOT' environment variable contains an invalid value. "
                                         "Make sure that the 'SUNGEAR_SOURCES_ROOT' environment variable contains the correct value and indeed points to the Sungear Engine root folder. "
                                         "Until then, you will not be able to build the project, as well as some other features of the engine.\n" +
                                         additionalWarningMessage;

            auto sungearEngineRootErrorDialog = DialogWindowsManager::createOneButtonWindow("Incorrect environment variable", "OK");
            sungearEngineRootErrorDialog->onCustomBodyRenderListener = [errorMsg]() {
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

void SGE::SungearEngineEditor::loadDefaultUIConfig() noexcept
{
    ImGui::LoadIniSettingsFromDisk(SGCore::Utils::toUTF8((getInstance()->getLocalPath() / "Resources/editor_ui_config.ini").u16string()).c_str());
}


