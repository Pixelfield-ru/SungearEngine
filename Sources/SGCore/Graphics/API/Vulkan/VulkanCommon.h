//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <vulkan/vulkan.h>

#include <source_location>
#include <string_view>

#include <sgcore_export.h>

namespace SGCore
{
    /// Human-readable name of a VkResult for logs.
    [[nodiscard]] SGCORE_EXPORT const char* vulkanResultToString(VkResult result) noexcept;

    /// Logs an error with the call site when result is not VK_SUCCESS. Returns true on success.
    SGCORE_EXPORT bool vulkanCheck(VkResult result, std::string_view what,
                                   const std::source_location& location = std::source_location::current()) noexcept;
}

/// Wraps a Vulkan call: logs on failure, evaluates to true on VK_SUCCESS.
#define SG_VK_CHECK(expr) ::SGCore::vulkanCheck((expr), #expr)
