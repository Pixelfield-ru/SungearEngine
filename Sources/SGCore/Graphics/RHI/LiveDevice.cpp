//
// Created by 8bitniksis on 20.08.2026.
//

#include "LiveDevice.h"

void SGCore::LiveDevice::set(IDevice* device) noexcept
{
    s_device = device;
}

SGCore::IDevice* SGCore::LiveDevice::get() noexcept
{
    return s_device;
}
