//
// Created by stuka on 14.10.2026.
//

#include "pixelfield/runtime/system/system_scheduler.h"

#include "pixelfield/runtime/system/system_graph.h"

// todo: review

pf::runtime::SystemScheduler::SystemScheduler(SystemSchedulerCreateInfo create_info) noexcept
{
    create_info_ = std::move(create_info);
}

void pf::runtime::SystemScheduler::run() noexcept
{
    running_ = true;

    while(running_)
    {
        if(const auto execution_res = runOnce())
        {
            create_info_.debug_output(core::LogLevel::kError,
                                      std::format("Unexpected error while running scheduler: {}",
                                                  execution_res.message()));
        }
    }
}

void pf::runtime::SystemScheduler::stop() noexcept
{
    running_ = false;
}

std::error_code pf::runtime::SystemScheduler::runOnce() noexcept
{
    const auto& graph = *create_info_.graph;
    auto& context = *create_info_.context;

    SystemGraphState state;
    state.remaining.resize(graph.nodes.size());
    for(const auto& node : graph.nodes)
    {
        state.remaining[node.index] = node.dependency_count;
    }

    executable_systems_ = graph.entries;
    while(!executable_systems_.empty() || running_systems_ > 0)
    {
        const SystemGraph::Node* node {};
        {
            std::lock_guard guard(systems_access_);
            node = &graph.nodes[executable_systems_.back()];
            executable_systems_.pop_back();
        }

        executeNode(context, state, *node);

        std::lock_guard guard(systems_access_);
        if(last_error_)
        {
            return last_error_;
        }
    }

    return {};
}

void pf::runtime::SystemScheduler::executeNode(SystemContext& context, SystemGraphState& graph_state,
                                               const SystemGraph::Node& node) noexcept
{
    ++running_systems_;

    auto* system = node.system;

    auto execution = [this, system, &context, &node, &graph_state]() {
        const auto res = system->execute(context);

        std::lock_guard guard(systems_access_);
        last_error_ = res;
        for(const auto successor_index : node.successors)
        {
            auto& successor_remaining = graph_state.remaining[successor_index];
            --successor_remaining;
            if(successor_remaining > 0) continue;

            executable_systems_.push_back(successor_index);
        }

        --running_systems_;
    };

    if(system->executionDomain() == ExecutionDomain::kAny && create_info_.thread_pool)
    {
        create_info_.thread_pool->post(std::move(execution));
    }
    else
    {
        execution();
    }
}
