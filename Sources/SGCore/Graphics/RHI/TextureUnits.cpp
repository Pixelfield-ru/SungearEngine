//
// Created by 8bitniksis on 17.08.2026.
//

#include "TextureUnits.h"

void SGCore::TextureUnits::set(std::uint8_t unit, Ref<IGPUObject> texture) noexcept
{
    if(unit >= max_units) return;
    s_units[unit] = std::move(texture);
}

const SGCore::Ref<SGCore::IGPUObject>& SGCore::TextureUnits::get(std::uint8_t unit) noexcept
{
    static const Ref<IGPUObject> empty;
    return unit < max_units ? s_units[unit] : empty;
}

void SGCore::TextureUnits::clear() noexcept
{
    for(auto& unit : s_units) unit.reset();
}
