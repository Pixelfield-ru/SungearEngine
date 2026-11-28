//
// Created by stuka on 28.11.2026.
//

#pragma once

#include <array>
#include <optional>

#include <glm/vec4.hpp>

#include <sgf/platform/window.h>
#include <sgf/rhi/vulkan/vulkan_context.h>
#include <sgf/runtime/system/system.h>

struct RenderingSystem final : sgf::runtime::ISystem
{
    RenderingSystem();
    ~RenderingSystem() override;

    [[nodiscard]] std::span<const std::uint32_t> reads() const noexcept override;
    [[nodiscard]] std::span<const std::uint32_t> writes() const noexcept override;

    std::error_code execute(sgf::runtime::SystemContext& context) override;
    std::error_code initialize(sgf::runtime::SystemContext& context) override;

private:
    struct WindowTarget
    {
        sgf::platform::Window* window {};
        sgf::core::UniquePtr<sgf::rhi::ISurface> surface;
        sgf::core::UniquePtr<sgf::rhi::ISwapchain> swapchain;
        sgf::core::UniquePtr<sgf::rhi::ICommandBuffer> command_buffer;
        sgf::core::Vector<sgf::core::UniquePtr<sgf::rhi::IFramebuffer>> framebuffers;
        glm::vec4 clear_color {};
    };

    void closeTarget(WindowTarget& target);
    [[nodiscard]] std::error_code draw(WindowTarget& target);

    static inline std::array read_ids_ { sgf::core::typeIndex<std::chrono::steady_clock::time_point>() };

    static constexpr std::size_t k_windows_count = 2;

    sgf::platform::Window windows_[k_windows_count];
    std::optional<sgf::rhi::VulkanContext> context_;
    sgf::core::Vector<sgf::core::SharedPtr<sgf::rhi::IPhysicalDevice>> physical_devices_;
    sgf::rhi::IPhysicalDevice* physical_device_ {};
    sgf::core::UniquePtr<sgf::rhi::ILogicalDevice> device_;
    sgf::core::UniquePtr<sgf::rhi::IShader> shader_;
    sgf::core::UniquePtr<sgf::rhi::IPass> pass_;
    sgf::core::UniquePtr<sgf::rhi::IPipelineState> pipeline_;
    sgf::core::Vector<WindowTarget> targets_;
};
