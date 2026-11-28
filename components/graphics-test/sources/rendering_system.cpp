//
// Created by stuka on 28.11.2026.
//

#include "rendering_system.h"

#include <array>
#include <print>

#include <sgf/core/errors.h>
#include <sgf/platform/platform.h>
#include <sgf/rhi/physical_device.h>

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

    constexpr std::array k_titles { "red", "blue" };
    constexpr std::array k_clear_colors {
        glm::vec4 { 0.8f, 0.1f, 0.1f, 1.0f },
        glm::vec4 { 0.1f, 0.2f, 0.8f, 1.0f }
    };
}

RenderingSystem::RenderingSystem()
{
    execution_domain_ = sgf::core::ExecutionDomain::kCurrent;
}

RenderingSystem::~RenderingSystem()
{
    if(device_)
    {
        std::ignore = device_->waitIdle();
    }
}

std::span<const std::uint32_t> RenderingSystem::reads() const noexcept
{
    return read_ids_;
}

std::span<const std::uint32_t> RenderingSystem::writes() const noexcept
{
    return {};
}

std::error_code RenderingSystem::initialize(sgf::runtime::SystemContext& context)
{
    const auto allocator = context.get_allocator();

    if(const auto error = sgf::platform::platform().init())
    {
        return error;
    }

    sgf::platform::WindowCreateInfo window_info;
    window_info.client_api = sgf::platform::WindowClientApi::kNoApi;
    window_info.width = 480;
    window_info.height = 360;

    for(std::size_t index = 0; index < k_windows_count; ++index)
    {
        window_info.title = k_titles[index];
        if(const auto error = sgf::platform::Window::create(window_info, windows_[index]))
        {
            return error;
        }
    }

    sgf::rhi::VulkanContextCreateInfo context_info(allocator);
    context_info.app_name = "graphics-test";
    context_info.engine_name = "sungear";
    context_.emplace(allocator);
    if(const auto error = sgf::rhi::VulkanContext::create(std::move(context_info), *context_))
    {
        return error;
    }

    physical_devices_ = context_->physicalDevices();
    if(physical_devices_.empty())
    {
        std::println("No physical device");
        return sgf::core::Error::kInvalidArgument;
    }

    physical_device_ = physical_devices_.front().get();
    for(const auto& device : physical_devices_)
    {
        if(device->type() == sgf::rhi::DeviceType::kDiscreteGpu)
        {
            physical_device_ = device.get();
            break;
        }
    }

    sgf::rhi::LogicalDeviceCreateInfo device_info;
    device_info.context = &*context_;
    device_info.physical_device = physical_device_;
    auto logical_device = context_->createLogicalDevice(device_info, allocator);
    if(!logical_device)
    {
        return logical_device.error();
    }
    device_ = std::move(*logical_device);

    sgf::rhi::Format pass_format = sgf::rhi::Format::kUnknown;
    for(std::size_t index = 0; index < k_windows_count; ++index)
    {
        sgf::rhi::SurfaceCreateInfo surface_info;
        surface_info.context = &*context_;
        surface_info.window = &windows_[index];
        auto surface = context_->createSurface(surface_info, allocator);
        if(!surface)
        {
            return surface.error();
        }

        sgf::rhi::SwapchainCreateInfo swapchain_info;
        swapchain_info.context = &*context_;
        swapchain_info.logical_device = device_.get();
        swapchain_info.physical_device = physical_device_;
        swapchain_info.surface = surface->get();
        swapchain_info.window = &windows_[index];
        swapchain_info.max_frames_in_flight = 1;

        auto swapchain = device_->createSwapchain(swapchain_info, allocator);
        if(!swapchain)
        {
            return swapchain.error();
        }

        if(!pass_)
        {
            sgf::rhi::PassCreateInfo pass_info(allocator);
            pass_info.context = &*context_;
            pass_info.logical_device = device_.get();
            sgf::rhi::ColorAttachmentCreateInfo color;
            color.format = (*swapchain)->format();
            color.final_layout = sgf::rhi::ImageLayout::kPresent;
            pass_info.color_attachments.push_back(color);

            auto created_pass = device_->createPass(pass_info, allocator);
            if(!created_pass)
            {
                return created_pass.error();
            }
            pass_ = std::move(*created_pass);
            pass_format = (*swapchain)->format();

            sgf::rhi::ShaderStage stage(allocator);
            stage.type = sgf::rhi::ShaderStageType::kVertex;
            stage.entry_point_name = "main";

            sgf::rhi::ShaderCreateInfo shader_info(allocator);
            shader_info.context = &*context_;
            shader_info.logical_device = device_.get();
            shader_info.stages_create_info.push_back({
                std::move(stage),
                k_clear_vertex_shader.data(),
                k_clear_vertex_shader.size() * sizeof(std::uint32_t)
            });

            auto created_shader = device_->createShader(shader_info, allocator);
            if(!created_shader)
            {
                return created_shader.error();
            }
            shader_ = std::move(*created_shader);

            sgf::rhi::PipelineStateCreateInfo pipeline_info(allocator);
            pipeline_info.context = &*context_;
            pipeline_info.logical_device = device_.get();
            pipeline_info.pass = pass_.get();
            pipeline_info.shader = shader_.get();
            pipeline_info.enable_rasterizer_discard = true;
            pipeline_info.cull_mode = sgf::rhi::CullMode::kNone;
            pipeline_info.attachments_blend_info.push_back({});

            auto created_pipeline = device_->createPipelineState(pipeline_info, allocator);
            if(!created_pipeline)
            {
                return created_pipeline.error();
            }
            pipeline_ = std::move(*created_pipeline);
        }
        else if((*swapchain)->format() != pass_format)
        {
            std::println("Swapchain formats differ");
            return sgf::core::Error::kInvalidArgument;
        }

        WindowTarget target {
            &windows_[index],
            std::move(*surface),
            std::move(*swapchain),
            {},
            sgf::core::Vector<sgf::core::UniquePtr<sgf::rhi::IFramebuffer>>(allocator),
            k_clear_colors[index]
        };

        sgf::rhi::CommandBufferCreateInfo command_info;
        command_info.context = &*context_;
        command_info.logical_device = device_.get();
        auto command_buffer = device_->createCommandBuffer(command_info, allocator);
        if(!command_buffer)
        {
            return command_buffer.error();
        }
        target.command_buffer = std::move(*command_buffer);

        const auto extent = target.swapchain->extent();
        for(std::uint16_t image_index = 0; image_index < target.swapchain->imagesCount(); ++image_index)
        {
            sgf::rhi::FramebufferCreateInfo framebuffer_info(allocator);
            framebuffer_info.context = &*context_;
            framebuffer_info.logical_device = device_.get();
            framebuffer_info.pass = pass_.get();
            framebuffer_info.images.push_back(target.swapchain->imageView(image_index));
            framebuffer_info.size = {
                static_cast<std::uint32_t>(extent.x),
                static_cast<std::uint32_t>(extent.y)
            };

            auto framebuffer = device_->createFramebuffer(framebuffer_info, allocator);
            if(!framebuffer)
            {
                return framebuffer.error();
            }
            target.framebuffers.push_back(std::move(*framebuffer));
        }

        targets_.push_back(std::move(target));
    }

    return {};
}

std::error_code RenderingSystem::execute(sgf::runtime::SystemContext& context)
{
    std::ignore = context.resources.read<std::chrono::steady_clock::time_point>();

    for(auto& target : targets_)
    {
        if(target.window->closed())
        {
            continue;
        }

        if(target.window->shouldClose())
        {
            closeTarget(target);
            continue;
        }

        if(const auto error = draw(target))
        {
            return error;
        }
    }

    return {};
}

void RenderingSystem::closeTarget(WindowTarget& target)
{
    if(device_)
    {
        std::ignore = device_->waitIdle();
    }

    target.framebuffers.clear();
    target.command_buffer.reset();
    target.swapchain.reset();
    target.surface.reset();
    target.window->close();
}

std::error_code RenderingSystem::draw(WindowTarget& target)
{
    auto frame = target.swapchain->acquireNextFrame();
    if(!frame)
    {
        return frame.error();
    }

    if(const auto error = target.command_buffer->begin())
    {
        return error;
    }

    const auto extent = target.swapchain->extent();
    sgf::rhi::BeginRenderingInfo rendering;
    rendering.pipeline_state = pipeline_.get();
    rendering.framebuffer = target.framebuffers[frame->image_index].get();
    rendering.render_area = { 0, 0, extent.x, extent.y };
    rendering.clear_infos.push_back(sgf::rhi::ClearInfo { target.clear_color });

    if(const auto error = target.command_buffer->beginRendering(rendering))
    {
        return error;
    }

    target.command_buffer->setViewport({
        { 0.0f, 0.0f, static_cast<float>(extent.x), static_cast<float>(extent.y) },
        0.0f,
        1.0f
    });
    target.command_buffer->setScissor({ 0, 0, extent.x, extent.y });

    if(const auto error = target.command_buffer->endRendering())
    {
        return error;
    }
    if(const auto error = target.command_buffer->end())
    {
        return error;
    }

    const sgf::rhi::ICommandBuffer* command_buffers[] { target.command_buffer.get() };
    sgf::rhi::SubmitInfo submit;
    submit.swapchain = target.swapchain.get();
    submit.command_buffers = command_buffers;
    submit.queue_family = sgf::rhi::QueueFamilyType::kGraphics;
    submit.wait_for_frame = &*frame;
    if(const auto error = device_->submit(submit))
    {
        return error;
    }

    sgf::rhi::PresentInfo present;
    present.frame = &*frame;
    present.queue_family = sgf::rhi::QueueFamilyType::kGraphics;
    return target.swapchain->present(present);
}
