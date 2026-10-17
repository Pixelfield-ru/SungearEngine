//
// Created by stuka on 17.10.2026.
//

#pragma once

#include <cstdint>

namespace pf::runtime
{
    enum class ExecutionDomain : std::uint8_t
    {
        kMain,
        kAny
    };
}