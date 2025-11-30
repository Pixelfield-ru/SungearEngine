//
// Created by stuka on 29.11.2025.
//

#pragma once

#include <optional>
#include <string>

namespace SGCore::UI::Utils
{
    std::optional<std::string> extractVariable(const std::string& text);
    std::optional<std::string> extractTemplateVariable(const std::string& text);
}