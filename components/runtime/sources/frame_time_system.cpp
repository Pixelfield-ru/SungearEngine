//
// Created by stuka on 14.10.2026.
//

#include "pixelfield/runtime/system/frame_time_system.h"

std::span<const std::uint32_t> pf::runtime::FrameTimeSystem::reads() const noexcept
{
    return read_ids_;
}

std::span<const std::uint32_t> pf::runtime::FrameTimeSystem::writes() const noexcept
{
    return write_ids_;
}

std::error_code pf::runtime::FrameTimeSystem::execute(SystemContext& context) noexcept
{
    pf_assert(context.resources.hasType<std::chrono::steady_clock::time_point>(), "Cannot find resource in context.");
    pf_assert(context.resources.hasType<FrameTime>(), "Type was not registered. Maybe system initialize() was not called.");

    const auto& now = context.resources.read<std::chrono::steady_clock::time_point>();
    auto& frame_time = context.resources.write<FrameTime>();

    auto delta = now - last_point_;
    if(frame_time.frame_index == 0)
    {
        delta = std::chrono::nanoseconds(0);
    }

    frame_time.delta = delta;
    frame_time.elapsed += delta;
    ++frame_time.frame_index;

    last_point_ = now;

    return {};
}

std::error_code pf::runtime::FrameTimeSystem::initialize(SystemContext& context) noexcept
{
    context.resources.emplace<FrameTime>();

    return {};
}
