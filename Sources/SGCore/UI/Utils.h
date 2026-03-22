//
// Created by stuka on 29.11.2025.
//

#pragma once

#include <optional>
#include <string>

#include <sgcore_export.h>

namespace SGCore::UI::Utils
{
    SGCORE_EXPORT std::optional<std::string> extractVariable(const std::string& text);
    SGCORE_EXPORT std::optional<std::string> extractTemplateVariable(const std::string& text);
}
