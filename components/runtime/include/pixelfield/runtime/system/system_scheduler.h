//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <unordered_map>
#include <mutex>

#include "pixelfield/runtime/execution/thread_pool.h"

#include "system.h"
#include "system_graph.h"

namespace pf::runtime
{
    struct SystemSchedulerCreateInfo final
    {
        const SystemGraph* graph {};
        SystemContext* context;
        ThreadPool* thread_pool {};
        core::LogCallback debug_output;
        std::unordered_map<SystemScheduleType, SystemScheduleConfig> schedule_config;
    };

    struct SystemScheduler final
    {
        SystemScheduler(SystemSchedulerCreateInfo create_info) noexcept;
        SystemScheduler(const SystemScheduler&) noexcept = delete;
        SystemScheduler(SystemScheduler&&) noexcept = delete;

        void run() noexcept;
        void stop() noexcept;
        std::error_code runOnce() noexcept;

    private:
        SystemSchedulerCreateInfo create_info_ {};

        std::mutex systems_access_;
        std::atomic<bool> running_;
        std::atomic<std::uint32_t> running_systems_ {};
        std::vector<std::uint32_t> executable_systems_;
        std::error_code last_error_{};

        void executeNode(SystemContext& context, SystemGraphState& graph_state, const SystemGraph::Node& node) noexcept;
    };
}
