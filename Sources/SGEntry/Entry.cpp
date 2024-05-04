#include "Entry.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Audio/AudioBuffer.h"
#include "SGCore/Audio/AudioSource.h"
#include "SGCore/Audio/AudioDevice.h"
#include "SGCore/Audio/AudioListener.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Render/PBRRP/PBRRenderPipeline.h"
#include "SGCore/Render/RenderPipelinesManager.h"
#include "SGCore/Scene/EntityBaseInfo.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Render/Camera3D.h"
#include "SGCore/Transformations/Controllable3D.h"
#include "SGCore/Render/RenderingBase.h"

#include <SGCore/Input/InputManager.h>
#include <SGCore/PluginsSystem/PluginsManager.h>
#include <SGCore/Memory/AssetManager.h>

#ifdef PLATFORM_OS_WINDOWS
#ifdef __cplusplus
extern "C" {
#endif
#include <windows.h>
    __declspec(dllexport) DWORD NvOptimusEnablement = 1;
    __declspec(dllexport) int AmdPowerXpressRequestHighPerformance = 1;
#ifdef __cplusplus
}
#endif
#endif

#include <glm/vec3.hpp>
#include <imgui.h>

#include "SGCore/ImGuiWrap/ImGuiLayer.h"
#include "SGCore/ImGuiWrap/Views/IView.h"
#include "SGCore/Graphics/API/ITexture2D.h"

SGCore::Ref<SGCore::Scene> testScene2;

template<typename RetT>
struct get_address;

template<typename RetT, typename... Args>
struct get_address<RetT(Args...)>
{
    size_t operator()(std::function<RetT(Args...)> f)
    {
        typedef RetT (fnType)(Args...);
        fnType** fnPointer = f.template target<fnType*>();
        return (size_t) *fnPointer;
    }
};

template<typename RetT, typename T>
size_t getAddress(std::function<RetT(T &)> f) {
    typedef void (fnType)(T &);
    fnType** fnPointer = f.template target<fnType*>();
    return (size_t) *fnPointer;
}

SGCore::Threading::ThreadsPool threadsPool(SGCore::Threading::ThreadCreatePolicy::IF_NO_FREE_THREADS);

void onTaskFinish(SGCore::Ref<SGCore::Threading::Thread> inThread)
{
    std::cout << "finished task in thread: " << SGCore::Threading::ThreadsManager::currentThread() << std::endl;
}

void onTaskExecute()
{
    std::cout << "execute task in thread : " << SGCore::Threading::ThreadsManager::currentThread() << std::endl;
}

void coreInit()
{
    testScene2 = SGCore::MakeRef<SGCore::Scene>();
    testScene2->m_name = "TestScene";
    testScene2->createDefaultSystems();
    SGCore::Scene::addScene(testScene2);
    SGCore::Scene::setCurrentScene("TestScene");
    
    std::cout << "MAIN THREAD: " << SGCore::Threading::ThreadsManager::getMainThread() << std::endl;
    
    auto thread = threadsPool.getThread();
    
    auto task = thread->createTask();
    task->setOnExecutedCallback(onTaskFinish, SGCore::Threading::ThreadsManager::currentThread(), thread);
    task->setOnExecuteCallback(onTaskExecute);
    thread->addTask(task);
    thread->start();
    
    SGCore::AssetManager::getInstance()->m_defaultAssetsLoadPolicy = SGCore::AssetsLoadPolicy::PARALLEL_THEN_LAZYLOAD;
    
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust_prev", "../SGResources/textures/spotted_rust/spotted-rust_preview.jpg");
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust0", "../SGResources/textures/spotted_rust/spotted-rust_albedo.png");
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust1", "../SGResources/textures/spotted_rust/spotted-rust_ao.png");
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust2", "../SGResources/textures/spotted_rust/spotted-rust_height.png");
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust3", "../SGResources/textures/spotted_rust/spotted-rust_metallic.png");
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust4", "../SGResources/textures/spotted_rust/spotted-rust_normal-ogl.png");
    SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("rust5", "../SGResources/textures/spotted_rust/spotted-rust_roughness.png");
    
    for(int i = 0; i < 15; ++i)
    {
        SGCore::AssetManager::getInstance()->loadAssetWithAlias<SGCore::ITexture2D>("skybox" + std::to_string(i),
                                                                                    "../SGResources/textures/skyboxes/skybox0/standard_skybox5.png");
    }
    
    std::cout << "thread0: " << thread << std::endl;
    auto thread1 = threadsPool.getThread();
    std::cout << "thread1: " << thread1 << std::endl;

    char* sgSourcesPath = std::getenv("SUNGEAR_SOURCES_ROOT");
    
    if(sgSourcesPath)
    {
        std::string sgEditorPath = std::string(sgSourcesPath) + "/Plugins/SungearEngineEditor";

        // hardcoded sgeditor load
        auto sgEditorPlugin =
                SGCore::PluginsManager::loadPlugin("SungearEngineEditor", "1.0.0",
                                                   sgEditorPath,
                                                   {},
                                                   SGCore::PluginBuildType::PBT_RELEASE);

        std::cout << "plugin: " << sgEditorPlugin  << ", sgeditor path: " << sgEditorPath << std::endl;
    }
    else
    {
        std::cout << "CANNOT LOAD SUNGEAR EDITOR PLUGIN" << std::endl;
    }
    
    SGCore::Threading::ThreadsManager::currentThread()->processFinishedTasks();
}

void onUpdate(const double& dt, const double& fixedDt)
{
    testScene2->update(dt, fixedDt);
    
    SGCore::ImGuiWrap::ImGuiLayer::beginFrame();
    SGCore::ImGuiWrap::IView::getRoot()->render();
    SGCore::ImGuiWrap::ImGuiLayer::endFrame();
    
    if(SGCore::InputManager::getMainInputListener()->keyboardKeyReleased(SGCore::KeyboardKey::KEY_P))
    {
        auto sgEditorPlugin = SGCore::PluginsManager::reloadPlugin("SungearEngineEditor", "1.0.0", { }, SGCore::PluginBuildType::PBT_RELEASE);
        std::cout << sgEditorPlugin << std::endl;
    }
}

int main()
{
    using namespace SGCore;
    
    CoreMain::onInit.connect<&coreInit>();
    CoreMain::getRenderTimer().onUpdate.connect<&onUpdate>();
    
    CoreMain::start();
    
    return 0;
}