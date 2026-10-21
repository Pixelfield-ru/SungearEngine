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

struct MyResource final : std::pmr::memory_resource
{
    size_t memoryUsage() const noexcept
    {
        return mem_usage_;
    }

private:
    void* do_allocate(size_t bytes, size_t alignment) override
    {
        std::println("allocated {} bytes with alignment {}", bytes, alignment);

        mem_usage_ += bytes;

        if(alignment <= alignof(std::max_align_t))
        {
            return ::operator new(bytes);
        }

        return ::operator new(bytes, std::align_val_t{alignment});
    }

    void do_deallocate(void* p, size_t bytes, size_t alignment) override
    {
        std::println("deallocated {} bytes with alignment {}", bytes, alignment);

        if(alignment <= alignof(std::max_align_t))
        {
            ::operator delete(p);
        }
        else
        {
            ::operator delete(p, std::align_val_t{alignment});
        }

        mem_usage_ -= bytes;
    }

    bool do_is_equal(const memory_resource& other) const noexcept override
    {
        return this == &other;
    }

    size_t mem_usage_ {};
};

int main()
{
    MyResource resource;

    sgf::core::LogCallback logger;
    logger.enabled = true;
    logger.function = [](sgf::core::LogLevel level, std::string_view msg) {
        std::println("Level {}: {}", std::to_underlying(level), msg);
    };

    sgf::runtime::SystemContext system_context;
    system_context.resources = sgf::core::TypeMap(&resource);
    system_context.resources.emplace<std::chrono::steady_clock::time_point>(std::chrono::steady_clock::now());

    sgf::core::Vector<sgf::core::UniquePtr<sgf::runtime::ISystem>> systems(&resource);
    systems.push_back(sgf::core::make_unique<sgf::runtime::FrameTimeSystem>(&resource));

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
    system_graph_build_info.memory_resource = &resource;

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
    scheduler_create_info.memory_resource = &resource;
    sgf::runtime::SystemScheduleState schedule;
    schedule.type = sgf::runtime::kFrame;
    // 120 frames per seconds
    schedule.interval = std::chrono::nanoseconds(1'000'000'000 / 120);
    scheduler_create_info.schedules.push_back(schedule);

    sgf::runtime::SystemScheduler system_scheduler;
    if(const auto res = sgf::runtime::SystemScheduler::create(scheduler_create_info, system_scheduler))
    {
        return 1;
    }

    // system_scheduler.run();
    if(const auto res = system_scheduler.runOnce())
    {
        return 1;
    }

    std::println("memory usage: {}", resource.memoryUsage());

    return 0;
}