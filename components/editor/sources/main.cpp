//
// Created by stuka on 18.10.2026.
//

#include <memory>
#include <iostream>
#include <print>
#include <ranges>

#include <sgf/runtime/system/system.h>
#include <sgf/runtime/system/system_graph.h>
#include <sgf/runtime/system/system_scheduler.h>
#include <sgf/runtime/system/frame_time_system.h>
#include <sgf/runtime/execution/thread_pool.h>

int main()
{
    sgf::core::LogCallback logger;
    logger.enabled = true;
    logger.function = [](sgf::core::LogLevel level, std::string_view msg) {
        std::println("Level {}: {}", std::to_underlying(level), msg);
    };

    sgf::runtime::SystemContext system_context;
    system_context.resources.emplace<std::chrono::steady_clock::time_point>(std::chrono::steady_clock::now());

    std::vector<std::unique_ptr<sgf::runtime::ISystem>> systems;
    systems.push_back(std::make_unique<sgf::runtime::FrameTimeSystem>());

    for(const auto& system : systems)
    {
        if(const auto res = system->initialize(system_context))
        {
            return 1;
        }
    }

    const auto systems_pointer = systems
    | std::views::transform([](const auto& system) { return system.get(); })
    | std::ranges::to<std::vector>();

    sgf::runtime::SystemGraphBuildInfo system_graph_build_info;
    system_graph_build_info.context = &system_context;
    system_graph_build_info.debug_output = &logger;
    system_graph_build_info.systems = systems_pointer;

    sgf::runtime::SystemGraph system_graph;
    if(auto res = sgf::runtime::SystemGraph::build(system_graph_build_info))
    {
        system_graph = std::move(*res);
    }
    else
    {
        return 1;
    }

    sgf::runtime::ThreadPool thread_pool(std::thread::hardware_concurrency() * 2);

    sgf::runtime::SystemSchedulerCreateInfo scheduler_create_info;
    scheduler_create_info.context = &system_context;
    scheduler_create_info.debug_output = &logger;
    scheduler_create_info.graph = &system_graph;
    scheduler_create_info.thread_pool = &thread_pool;
    sgf::runtime::SystemScheduler system_scheduler(scheduler_create_info);

    // system_scheduler.run();
    system_scheduler.runOnce();

    return 0;
}