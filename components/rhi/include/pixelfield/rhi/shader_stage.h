//
// Created by stuka on 03.09.2026.
//

#pragma once

#include <string>

#include "types.h"

namespace pf::rhi
{
    struct ShaderStage
    {
        ShaderStageType type = ShaderStageType::kUnknown;
        std::string entry_point_name;
    };
}