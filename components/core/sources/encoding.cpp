//
// Created by stuka on 12.10.2026.
//

#include "pixelfield/core/encoding.h"

std::string pf::core::pathToUtf8(const std::filesystem::path& path) noexcept
{
    const auto u8 = path.u8string();
    return { u8.begin(), u8.end() };
}