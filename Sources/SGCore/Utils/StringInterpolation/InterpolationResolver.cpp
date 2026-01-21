//
// Created by stuka on 28.11.2024.
//

#include "InterpolationResolver.h"

SGCore::InterpolationMarkupData<std::filesystem::path>& SGCore::Impl::getInterpolationMarkupDataInstance(std::filesystem::path*) noexcept
{
    static InterpolationMarkupData<std::filesystem::path> s_instance;
    return s_instance;
}

SGCore::InterpolationMarkupData<std::string>& SGCore::Impl::getInterpolationMarkupDataInstance(std::string*) noexcept
{
    static InterpolationMarkupData<std::string> s_instance;
    return s_instance;
}
