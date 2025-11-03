//
// Created by Ilya on 18.11.2023.
//

#include <imgui_impl_opengl3.h>

#include "ImGuiLayer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/GAPIType.h"
#include "SGCore/Graphics/API/IRenderer.h"
#include "SGCore/Utils/Platform.h"

#if SG_PLATFORM_PC
#include <imgui_impl_glfw.h>
#elif SG_PLATFORM_OS_ANDROID
#include <imgui_impl_android.h>
#endif

void SGCore::ImGuiWrap::ImGuiLayer::init() noexcept
{
    IMGUI_CHECKVERSION();
    ImGuiContext* imGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imGuiContext);
    m_currentContext = imGuiContext;
    std::cout << "ImGui init! imGuiContext: " << ImGui::GetCurrentContext() << std::endl;
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;
    // io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;

    auto gapiType = CoreMain::getRenderer()->getGAPIType();

    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46)
    {
#if SG_PLATFORM_PC
        ImGui_ImplGlfw_InitForOpenGL(CoreMain::getWindow().m_handle, true);
        ImGui_ImplOpenGL3_Init();
#elif SG_PLATFORM_OS_ANDROID
        ImGui_ImplAndroid_Init(CoreMain::getWindow().m_handle);
        ImGui_ImplOpenGL3_Init("#version 320 es");
#endif
    }
}

void SGCore::ImGuiWrap::ImGuiLayer::destroy() noexcept
{
    
    auto gapiType = CoreMain::getRenderer()->getGAPIType();
    
    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46)
    {
        ImGui_ImplOpenGL3_DestroyDeviceObjects();
#if SG_PLATFORM_PC
        ImGui_ImplGlfw_Shutdown();
#elif SG_PLATFORM_OS_ANDROID
        ImGui_ImplAndroid_Shutdown();
#endif
        ImGui_ImplOpenGL3_Shutdown();
    }
    
    ImGui::DestroyContext();
}

void SGCore::ImGuiWrap::ImGuiLayer::reload() noexcept
{
    /*destroy();
    init();*/
}

void SGCore::ImGuiWrap::ImGuiLayer::beginFrame() noexcept
{
    auto gapiType = CoreMain::getRenderer()->getGAPIType();

    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46)
    {
        ImGui_ImplOpenGL3_NewFrame();
#if SG_PLATFORM_PC
        ImGui_ImplGlfw_NewFrame();
#elif SG_PLATFORM_OS_ANDROID
        ImGui_ImplAndroid_NewFrame();
#endif
    }

    ImGui::NewFrame();
}

void SGCore::ImGuiWrap::ImGuiLayer::endFrame() noexcept
{
    auto gapiType = CoreMain::getRenderer()->getGAPIType();

    ImGui::Render();

    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46 || gapiType == GAPIType::SG_API_TYPE_GLES3)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    
    // ImGui::EndFrame();

    /*if (ImGui::GetIO().ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
    {
        ImGui::UpdatePlatformWindows();
        ImGui::RenderPlatformWindowsDefault();
    }*/
}

ImGuiContext* SGCore::ImGuiWrap::ImGuiLayer::getCurrentContext() noexcept
{
    return m_currentContext;
}

void SGCore::ImGuiWrap::ImGuiLayer::destroyDeviceObjects() noexcept
{
    auto gapiType = SGCore::CoreMain::getRenderer()->getGAPIType();

    switch(gapiType)
    {
        case SGCore::SG_API_TYPE_UNKNOWN:
            break;
        case SGCore::SG_API_TYPE_GL4:
            ImGui_ImplOpenGL3_DestroyDeviceObjects();
            break;
        case SGCore::SG_API_TYPE_GL46:
            ImGui_ImplOpenGL3_DestroyDeviceObjects();
            break;
        case SGCore::SG_API_TYPE_GLES2:
            break;
        case SGCore::SG_API_TYPE_GLES3:
            ImGui_ImplOpenGL3_DestroyDeviceObjects();
            break;
        case SGCore::SG_API_TYPE_VULKAN:
            break;
    }
}
