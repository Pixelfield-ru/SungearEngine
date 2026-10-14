//
// Created by stuka on 12.10.2026.
//

#pragma once

#include <string>
#include <filesystem>

namespace pf::core
{
    std::string pathToUtf8(const std::filesystem::path& p) noexcept;
}