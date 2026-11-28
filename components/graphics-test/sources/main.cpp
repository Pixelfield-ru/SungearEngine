//
// Created by stuka on 18.10.2026.
//

#include <print>
#include <ranges>
#include <thread>

#include <sgf/core/execution/thread_pool.h>
#include <sgf/core/log.h>
#include <sgf/runtime/system/system.h>
#include <sgf/runtime/system/system_graph.h>
#include <sgf/runtime/system/system_scheduler.h>

#include "platform_polling_system.h"
#include "rendering_system.h"

int main()
{
    std::pmr::polymorphic_allocator<> allocator;

    sgf::core::LogCallback logger;
    logger.enabled = true;
    logger.function = [](sgf::core::LogLevel level, std::string_view message, std::source_location caller) {
        std::println("Level {}: {} ({}:{})", std::to_underlying(level), message, caller.file_name(), caller.line());
    };

    sgf::core::ThreadPool thread_pool(std::thread::hardware_concurrency() * 2, allocator);
    sgf::runtime::SystemContext system_context(allocator);
    auto& scheduler = system_context.resources.emplace<sgf::runtime::SystemScheduler>(allocator);

    sgf::core::Vector<sgf::core::UniquePtr<sgf::runtime::ISystem>> systems(allocator);
    systems.push_back(sgf::core::makeUnique<PlatformPollingSystem>(allocator));
    systems.push_back(sgf::core::makeUnique<RenderingSystem>(allocator));

    for(const auto& system : systems)
    {
        if(const auto error = system->initialize(system_context))
        {
            std::println("{}", error.message());
            return 1;
        }
    }

    const auto system_pointers = systems
        | std::views::transform([](const auto& system) { return system.get(); })
        | std::ranges::to<std::vector>();

    sgf::runtime::SystemGraphBuildInfo graph_info;
    graph_info.context = &system_context;
    graph_info.debug_output = &logger;
    graph_info.systems = system_pointers;

    sgf::runtime::SystemGraph system_graph(allocator);
    if(const auto error = sgf::runtime::SystemGraph::build(graph_info, system_graph))
    {
        std::println("{}", error.message());
        return 1;
    }

    sgf::runtime::SystemSchedulerCreateInfo scheduler_info(allocator);
    scheduler_info.context = &system_context;
    scheduler_info.debug_output = &logger;
    scheduler_info.graph = &system_graph;
    scheduler_info.thread_pool = &thread_pool;

    sgf::runtime::SystemScheduleState schedule;
    schedule.type = sgf::runtime::kFrame;
    schedule.interval = std::chrono::nanoseconds(1'000'000'000 / 120);
    scheduler_info.schedules.push_back(schedule);

    if(const auto error = sgf::runtime::SystemScheduler::create(std::move(scheduler_info), scheduler))
    {
        std::println("{}", error.message());
        return 1;
    }

    scheduler.run();
    return 0;
}
