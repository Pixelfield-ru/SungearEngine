//
// Created by stuka on 14.10.2026.
//

#include "pixelfield/runtime/system_graph.h"

#include <algorithm>
#include <ranges>
#include <unordered_set>

#include "pixelfield/runtime/system.h"
#include "pixelfield/runtime/errors.h"

std::expected<pf::runtime::SystemGraph, std::error_code> pf::runtime::SystemGraph::build(
    const SystemGraphBuildInfo& build_info) noexcept
{
    // todo: maybe needs optimization

    SystemGraph graph;

    const auto& context = build_info.context;

    // adding nodes
    for(size_t i = 0; i < build_info.systems.size(); ++i)
    {
        auto* system = build_info.systems[i];

        // validation
        const auto system_reads = system->reads();
        if(!std::ranges::all_of(system_reads, [&](std::uint32_t read_id) {
            return context->resources.hasType(read_id);
        })) return std::unexpected(Error::kIncompleteSystemContext);

        const auto system_writes = system->writes();
        if(!std::ranges::all_of(system_writes, [&](std::uint32_t write_id) {
            return context->resources.hasType(write_id);
        })) return std::unexpected(Error::kIncompleteSystemContext);

        Node node;
        node.system = system;
        node.index = i;

        graph.nodes.push_back(std::move(node));
    }

    std::unordered_set<std::uint32_t> successors_set;

    // building dependencies
    for(auto& node : graph.nodes)
    {
        const auto system_writes = node.system->writes();

        for(const auto write_id : system_writes)
        {
            for(auto& other : graph.nodes)
            {
                if(other.index == node.index) continue;

                const auto other_reads = other.system->reads();
                const auto other_writes = other.system->writes();

                if(std::ranges::any_of(other_reads, [&](std::uint32_t other_read_id) {
                    return write_id == other_read_id && std::ranges::count(other_writes, other_read_id) == 0;
                }) ||
                std::ranges::any_of(other_writes, [&](std::uint32_t other_write_id) {
                    // prevent data race (two systems are writing one type)
                    return write_id == other_write_id && node.index < other.index;
                }))
                {
                    if(successors_set.contains(other.index)) continue;

                    ++other.dependency_count;
                    node.successors.push_back(other.index);
                    successors_set.insert(other.index);
                }
            }
        }

        successors_set.clear();
    }

    // getting entries
    for(auto& node : graph.nodes)
    {
        if(node.dependency_count > 0) continue;

        graph.entries.push_back(node.index);
    }

    // initializing state to check cycles
    SystemGraphState state;
    state.remaining.resize(graph.nodes.size());
    for(const auto& node : graph.nodes)
    {
        state.remaining[node.index] = node.dependency_count;
    }

    // check for cycles
    auto executable_systems = graph.entries;
    while(!executable_systems.empty())
    {
        const auto& node = graph.nodes[executable_systems.back()];
        executable_systems.pop_back();

        for(const auto successor_index : node.successors)
        {
            auto& successor_remaining = state.remaining[successor_index];
            --successor_remaining;
            if(successor_remaining > 0) continue;

            executable_systems.push_back(successor_index);
        }
    }

    if(std::ranges::any_of(state.remaining, [](std::uint32_t remaining) {
        return remaining != 0;
    }))
    {
        return std::unexpected(Error::kCyclicSystemGraph);
    }

    return graph;
}
