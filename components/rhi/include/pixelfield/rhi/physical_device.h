//
// Created by stuka on 10.09.2026.
//

#pragma once

#include <string>

#include "types.h"

namespace pf::rhi
{
    struct IPhysicalDevice
    {
        virtual ~IPhysicalDevice() = default;

        [[nodiscard]] virtual bool valid() const noexcept = 0;

        [[nodiscard]] std::string name() const noexcept
        {
            return name_;
        }

        [[nodiscard]] DeviceType type() const noexcept
        {
            return type_;
        }

    protected:
        std::string name_;
        DeviceType type_ = DeviceType::kIntegratedGpu;
    };
}
