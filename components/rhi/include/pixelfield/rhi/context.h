//
// Created by stuka on 10.09.2026.
//

#pragma once

#include <expected>
#include <string>
#include <functional>

#include "errors.h"
#include "logical_device.h"

namespace pf::rhi
{
    struct ContextCreateInfo
    {
        /// Required.
        std::string app_name;
        /// Required.
        std::string engine_name;
        /// Optional.
        bool enable_api_debug_output {};
        /// Optional.
        bool enable_rhi_debug_output {};
        //// Required.
        GLFWwindow* window {};
        /// Optional.
        std::function<void(LogLevel level, std::string_view msg)> rhi_debug_output_func;
    };

    struct IPhysicalDevice;

    struct IContext
    {
        virtual ~IContext() = default;

        [[nodiscard]] virtual bool valid() const noexcept = 0;

        [[nodiscard]] virtual std::vector<std::shared_ptr<IPhysicalDevice>> physicalDevices() noexcept = 0;

        [[nodiscard]] virtual std::expected<std::unique_ptr<ILogicalDevice>, std::error_code> createLogicalDevice(const LogicalDeviceCreateInfo& create_info) noexcept = 0;

        void printDebugMessage(LogLevel level, std::string_view msg) const noexcept
        {
            if(enable_rhi_debug_output_ && rhi_debug_output_)
            {
                rhi_debug_output_(level, msg);
            }
        }

    protected:
        bool enable_rhi_debug_output_ {};
        std::function<void(LogLevel level, std::string_view msg)> rhi_debug_output_;
    };
}
