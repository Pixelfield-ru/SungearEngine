//
// Created by stuka on 28.11.2024.
//

#include "InterpolationResolver.h"

std::unordered_map<std::string, std::filesystem::path>& SGCore::Impl::getInterpolationMarkupGlobalData(std::filesystem::path*) noexcept
{
    static std::unordered_map<std::string, std::filesystem::path> s_globalMarkup;
    return s_globalMarkup;
}

std::unordered_map<std::string, std::string>& SGCore::Impl::getInterpolationMarkupGlobalData(std::string*) noexcept
{
    static std::unordered_map<std::string, std::string> s_globalMarkup;
    return s_globalMarkup;
}
