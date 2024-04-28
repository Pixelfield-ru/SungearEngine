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
    ImGui::CreateContext();
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