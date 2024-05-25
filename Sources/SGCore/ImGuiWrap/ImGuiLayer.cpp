//
// Created by Ilya on 18.11.2023.
//

#include "ImGuiLayer.h"
#include "SGCore/Main/CoreMain.h"
#include "SGCore/Graphics/API/GAPIType.h"
#include "SGCore/Graphics/API/IRenderer.h"

void SGCore::ImGuiWrap::ImGuiLayer::init() noexcept
{
    IMGUI_CHECKVERSION();
    ImGuiContext* imGuiContext = ImGui::CreateContext();
    ImGui::SetCurrentContext(imGuiContext);
    m_currentContext = imGuiContext;
    std::cout << "ImGui init! imGuiContext: " << m_currentContext << std::endl;
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;
    io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;

    auto gapiType = CoreMain::getRenderer()->getGAPIType();

    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46)
    {
        ImGui_ImplGlfw_InitForOpenGL(CoreMain::getWindow().m_handler, true);
        ImGui_ImplOpenGL3_Init();
    }
}

void SGCore::ImGuiWrap::ImGuiLayer::destroy() noexcept
{
    
    auto gapiType = CoreMain::getRenderer()->getGAPIType();
    
    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46)
    {
        ImGui_ImplOpenGL3_DestroyDeviceObjects();
        ImGui_ImplGlfw_Shutdown();
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
        ImGui_ImplGlfw_NewFrame();
    }

    ImGui::NewFrame();
}

void SGCore::ImGuiWrap::ImGuiLayer::endFrame() noexcept
{
    auto gapiType = CoreMain::getRenderer()->getGAPIType();

    ImGui::Render();

    if (gapiType == GAPIType::SG_API_TYPE_GL4 ||
        gapiType == GAPIType::SG_API_TYPE_GL46)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
    
    ImGui::EndFrame();
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
            break;
        case SGCore::SG_API_TYPE_VULKAN:
            break;
    }
}
