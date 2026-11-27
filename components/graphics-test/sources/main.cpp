//
// Created by stuka on 18.10.2026.
//

#include <array>
#include <print>
#include <string_view>

#include <glm/vec4.hpp>

#include <sgf/platform/platform.h>
#include <sgf/platform/window.h>
#include <sgf/rhi/physical_device.h>
#include <sgf/rhi/vulkan/vulkan_context.h>

namespace
{
    // Empty vertex stage. Both windows are cleared by the pass, so the shader is not executed.
    constexpr std::array<std::uint32_t, 42> k_clear_vertex_shader {
        0x07230203, 0x00010000, 0x0008000b, 0x00000006,
        0x00000000, 0x00020011, 0x00000001, 0x0006000b,
        0x00000001, 0x4c534c47, 0x6474732e, 0x3035342e,
        0x00000000, 0x0003000e, 0x00000000, 0x00000001,
        0x0005000f, 0x00000000, 0x00000004, 0x6e69616d,
        0x00000000, 0x00030003, 0x00000002, 0x000001c2,
        0x00040005, 0x00000004, 0x6e69616d, 0x00000000,
        0x00020013, 0x00000002, 0x00030021, 0x00000003,
        0x00000002, 0x00050036, 0x00000002, 0x00000004,
        0x00000000, 0x00000003, 0x000200f8, 0x00000005,
        0x000100fd, 0x00010038
    };

    int fail(std::string_view what, const std::error_code& error)
    {
        std::println("{}: {}", what, error.message());
        return 1;
    }

    struct WindowTarget
    {
        sgf::platform::Window* window {};
        sgf::core::UniquePtr<sgf::rhi::ISwapchain> swapchain;
        sgf::core::UniquePtr<sgf::rhi::ICommandBuffer> command_buffer;
        sgf::core::Vector<sgf::core::UniquePtr<sgf::rhi::IFramebuffer>> framebuffers;
        glm::vec4 clear_color {};
    };
}

int main()
{
    std::pmr::polymorphic_allocator<> allocator;

    if(const auto error = sgf::platform::platform().init())
    {
        return fail("Cannot init platform", error);
    }

    static constexpr auto windows_count = 2;

    sgf::platform::Window windows[windows_count];
    sgf::platform::WindowCreateInfo window_info;
    window_info.client_api = sgf::platform::WindowClientApi::kNoApi;
    window_info.width = 480;
    window_info.height = 360;
    window_info.title = "red";
    if(const auto error = sgf::platform::Window::create(window_info, windows[0]))
    {
        return fail("Cannot create window", error);
    }

    window_info.title = "blue";
    if(const auto error = sgf::platform::Window::create(window_info, windows[1]))
    {
        return fail("Cannot create window", error);
    }

    sgf::rhi::VulkanContextCreateInfo context_info(allocator);
    context_info.app_name = "graphics-test";
    context_info.engine_name = "sungear";

    sgf::rhi::VulkanContext context(allocator);
    if(const auto error = sgf::rhi::VulkanContext::create(std::move(context_info), context))
    {
        return fail("Cannot create context", error);
    }

    // ======================================
    // create surfaces for windows
    sgf::core::UniquePtr<sgf::rhi::ISurface> surfaces[windows_count];

    for(std::int8_t i = 0; i < windows_count; ++i)
    {
        sgf::rhi::SurfaceCreateInfo surface_info;
        surface_info.context = &context;
        surface_info.window = &windows[i];
        auto surface = context.createSurface(surface_info, allocator);
        if(!surface)
        {
            return fail("Cannot create surface", surface.error());
        }

        surfaces[i] = std::move(*surface);
    }

    // =========================================
    // get physical devices
    auto physical_devices = context.physicalDevices();
    if(physical_devices.empty())
    {
        std::println("No physical device");
        return 1;
    }

    // select physical device
    sgf::rhi::IPhysicalDevice* physical_device = physical_devices.front().get();
    for(const auto& device : physical_devices)
    {
        if(device->type() == sgf::rhi::DeviceType::kDiscreteGpu)
        {
            physical_device = device.get();
            break;
        }
    }

    // create logical device
    sgf::rhi::LogicalDeviceCreateInfo device_info;
    device_info.context = &context;
    device_info.physical_device = physical_device;
    auto logical_device = context.createLogicalDevice(device_info, allocator);
    if(!logical_device)
    {
        return fail("Cannot create logical device", logical_device.error());
    }

    // ===================================
    // setup gpu objects

    const glm::vec4 clear_colors[] {
        { 0.8f, 0.1f, 0.1f, 1.0f },
        { 0.1f, 0.2f, 0.8f, 1.0f }
    };

    sgf::core::UniquePtr<sgf::rhi::IShader> shader;
    sgf::core::UniquePtr<sgf::rhi::IPass> pass;
    sgf::core::UniquePtr<sgf::rhi::IPipelineState> pipeline;
    sgf::rhi::Format pass_format = sgf::rhi::Format::kUnknown;
    sgf::core::Vector<WindowTarget> targets(allocator);

    for(std::size_t target_index = 0; target_index < windows_count; ++target_index)
    {
        // create swapchain for a surface
        sgf::rhi::SwapchainCreateInfo swapchain_info;
        swapchain_info.context = &context;
        swapchain_info.logical_device = logical_device->get();
        swapchain_info.physical_device = physical_device;
        swapchain_info.surface = surfaces[target_index].get();
        swapchain_info.window = &windows[target_index];
        swapchain_info.max_frames_in_flight = 1;

        auto swapchain = (*logical_device)->createSwapchain(swapchain_info, allocator);
        if(!swapchain)
        {
            return fail("Cannot create swapchain", swapchain.error());
        }

        // create same pass for all windows
        if(!pass)
        {
            // =================================================================
            // create pass
            sgf::rhi::PassCreateInfo pass_info(allocator);
            pass_info.context = &context;
            pass_info.logical_device = logical_device->get();
            sgf::rhi::ColorAttachmentCreateInfo color;
            color.format = (*swapchain)->format();
            color.final_layout = sgf::rhi::ImageLayout::kPresent;
            pass_info.color_attachments.push_back(color);

            auto created_pass = (*logical_device)->createPass(pass_info, allocator);
            if(!created_pass)
            {
                return fail("Cannot create pass", created_pass.error());
            }
            pass = std::move(*created_pass);
            pass_format = (*swapchain)->format();

            // ===================================================================
            // create shader
            sgf::rhi::ShaderStage stage(allocator);
            stage.type = sgf::rhi::ShaderStageType::kVertex;
            stage.entry_point_name = "main";

            sgf::rhi::ShaderCreateInfo shader_info(allocator);
            shader_info.context = &context;
            shader_info.logical_device = logical_device->get();
            shader_info.stages_create_info.push_back({
                std::move(stage),
                k_clear_vertex_shader.data(),
                k_clear_vertex_shader.size() * sizeof(std::uint32_t)
            });

            auto created_shader = (*logical_device)->createShader(shader_info, allocator);
            if(!created_shader)
            {
                return fail("Cannot create shader", created_shader.error());
            }
            shader = std::move(*created_shader);

            // ===================================================================
            // create pipeline
            sgf::rhi::PipelineStateCreateInfo pipeline_info(allocator);
            pipeline_info.context = &context;
            pipeline_info.logical_device = logical_device->get();
            pipeline_info.pass = pass.get();
            pipeline_info.shader = shader.get();
            pipeline_info.enable_rasterizer_discard = true;
            pipeline_info.cull_mode = sgf::rhi::CullMode::kNone;
            pipeline_info.attachments_blend_info.push_back({});

            auto created_pipeline = (*logical_device)->createPipelineState(pipeline_info, allocator);
            if(!created_pipeline)
            {
                return fail("Cannot create pipeline", created_pipeline.error());
            }
            pipeline = std::move(*created_pipeline);
        }
        else if((*swapchain)->format() != pass_format)
        {
            std::println("Swapchain formats differ");
            return 1;
        }

        WindowTarget target {
            &windows[target_index],
            std::move(*swapchain),
            {},
            sgf::core::Vector<sgf::core::UniquePtr<sgf::rhi::IFramebuffer>>(allocator),
            clear_colors[target_index]
        };

        // =============================================================================
        // create command buffer
        sgf::rhi::CommandBufferCreateInfo command_info;
        command_info.context = &context;
        command_info.logical_device = logical_device->get();
        auto command_buffer = (*logical_device)->createCommandBuffer(command_info, allocator);
        if(!command_buffer)
        {
            return fail("Cannot create command buffer", command_buffer.error());
        }
        target.command_buffer = std::move(*command_buffer);

        // ===========================================================================
        // create framebuffer for every image in swapchain
        const auto extent = target.swapchain->extent();
        for(std::uint16_t image_index = 0; image_index < target.swapchain->imagesCount(); ++image_index)
        {
            sgf::rhi::FramebufferCreateInfo framebuffer_info(allocator);
            framebuffer_info.context = &context;
            framebuffer_info.logical_device = logical_device->get();
            framebuffer_info.pass = pass.get();
            framebuffer_info.images.push_back(target.swapchain->imageView(image_index));
            framebuffer_info.size = {
                static_cast<std::uint32_t>(extent.x),
                static_cast<std::uint32_t>(extent.y)
            };

            auto framebuffer = (*logical_device)->createFramebuffer(framebuffer_info, allocator);
            if(!framebuffer)
            {
                return fail("Cannot create framebuffer", framebuffer.error());
            }
            target.framebuffers.push_back(std::move(*framebuffer));
        }

        targets.push_back(std::move(target));
    }

    const auto windowsClosed = [&]() {
        for(std::int8_t i = 0; i < windows_count; ++i)
        {
            if(!windows[i].closed()) return false;
        }

        return true;
    };

    while(!windowsClosed())
    {
        sgf::platform::platform().poll();

        for(auto& target : targets)
        {
            if(target.window->closed())
            {
                continue;
            }

            auto frame = target.swapchain->acquireNextFrame();
            if(!frame)
            {
                return fail("Cannot acquire frame", frame.error());
            }

            if(const auto error = target.command_buffer->begin())
            {
                return fail("Cannot begin command buffer", error);
            }

            const auto extent = target.swapchain->extent();
            sgf::rhi::BeginRenderingInfo rendering;
            rendering.pipeline_state = pipeline.get();
            rendering.framebuffer = target.framebuffers[frame->image_index].get();
            rendering.render_area = { 0, 0, extent.x, extent.y };
            rendering.clear_infos.push_back(sgf::rhi::ClearInfo { target.clear_color });

            if(const auto error = target.command_buffer->beginRendering(rendering))
            {
                return fail("Cannot begin rendering", error);
            }

            target.command_buffer->setViewport({
                { 0.0f, 0.0f, static_cast<float>(extent.x), static_cast<float>(extent.y) },
                0.0f,
                1.0f
            });
            target.command_buffer->setScissor({ 0, 0, extent.x, extent.y });

            if(const auto error = target.command_buffer->endRendering())
            {
                return fail("Cannot end rendering", error);
            }
            if(const auto error = target.command_buffer->end())
            {
                return fail("Cannot end command buffer", error);
            }

            const sgf::rhi::ICommandBuffer* command_buffers[] { target.command_buffer.get() };
            sgf::rhi::SubmitInfo submit;
            submit.swapchain = target.swapchain.get();
            submit.command_buffers = command_buffers;
            submit.queue_family = sgf::rhi::QueueFamilyType::kGraphics;
            submit.wait_for_frame = &*frame;
            if(const auto error = (*logical_device)->submit(submit))
            {
                return fail("Cannot submit", error);
            }

            sgf::rhi::PresentInfo present;
            present.frame = &*frame;
            present.queue_family = sgf::rhi::QueueFamilyType::kGraphics;
            if(const auto error = target.swapchain->present(present))
            {
                return fail("Cannot present", error);
            }
        }
    }

    std::ignore = (*logical_device)->waitIdle();
    return 0;
}
