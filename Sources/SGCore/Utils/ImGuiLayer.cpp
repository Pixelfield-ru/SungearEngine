//
// Created by Ilya on 18.11.2023.
//

#include "ImGuiLayer.h"
#include "SGCore/Main/CoreMain.h"

void Core::Utils::ImGuiLayer::initImGui() noexcept
{
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    auto gapiType = Core::Main::CoreMain::getRenderer().getGAPIType();

    if (gapiType == Core::Graphics::SG_API_TYPE_GL4 ||
        gapiType == Core::Graphics::SG_API_TYPE_GL46)
    {
        ImGui_ImplGlfw_InitForOpenGL(Core::Main::CoreMain::getWindow().m_handler, true);
        ImGui_ImplOpenGL3_Init();
    }
}

void Core::Utils::ImGuiLayer::beginFrame() noexcept
{
    auto gapiType = Core::Main::CoreMain::getRenderer().getGAPIType();

    if (gapiType == Core::Graphics::SG_API_TYPE_GL4 ||
        gapiType == Core::Graphics::SG_API_TYPE_GL46)
    {
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
    }

    ImGui::NewFrame();
}

void Core::Utils::ImGuiLayer::endFrame() noexcept
{
    auto gapiType = Core::Main::CoreMain::getRenderer().getGAPIType();

    ImGui::Render();

    if (gapiType == Core::Graphics::SG_API_TYPE_GL4 ||
        gapiType == Core::Graphics::SG_API_TYPE_GL46)
    {
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
    }
}
