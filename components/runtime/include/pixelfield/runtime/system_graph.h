//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <expected>
#include <system_error>
#include <vector>
#include <span>

#include "pixelfield/core/log.h"

namespace pf::runtime
{
    struct ISystem;
    struct SystemContext;

    struct SystemGraphBuildInfo final
    {
        std::span<ISystem *const> systems;
        const SystemContext* context {};
        core::LogCallback debug_output;
    };

    struct SystemGraphState final
    {
        /// Indicates count of unfulfilled links for each node.
        std::vector<std::uint32_t> remaining;
    };

    struct SystemGraph final
    {
        struct Node final
        {
            /// Executable system.
            ISystem* system {};
            /// Index in array.
            std::uint32_t index {};
            /// Indices of successors in array.
            std::vector<std::uint32_t> successors;
            /// Count of links.
            std::uint32_t dependency_count {};
        };

        /// All nodes in graph.
        std::vector<Node> nodes;
        /// Entry nodes.
        std::vector<std::uint32_t> entries;

        [[nodiscard]] static std::expected<SystemGraph, std::error_code> build(const SystemGraphBuildInfo& build_info) noexcept;
    };
}