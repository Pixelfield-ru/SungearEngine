//
// Created by stuka on 29.11.2025.
//

#include "Utils.h"

#include <regex>

std::optional<std::string> SGCore::UI::UITreeUtils::extractVariable(const std::string& text)
{
    static std::regex pattern("\\{\\{\\s*(\\w+)\\s*\\}\\}");
    std::smatch matches;

    if (std::regex_search(text, matches, pattern) && matches.size() > 1)
    {
        return matches[1].str();
    }

    return std::nullopt;
}
