//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <system_error>
#include <span>

#include "pixelfield/core/structures/type_map.h"

namespace pf::runtime
{
    struct SystemContext final
    {
        core::TypeMap resources;
    };

    struct ISystem
    {
        virtual ~ISystem() = default;

        /**
         * @return Indices of types that the system reads.
         */
        virtual std::span<const std::uint32_t> reads() const noexcept = 0;
        /**
        * @return Indices of types that the system writes.
        */
        virtual std::span<const std::uint32_t> writes() const noexcept = 0;

        /**
        * Execute system once.
        * @param context System context.
        * @return Result code.
        */
        virtual std::error_code execute(SystemContext& context) = 0;

        /**
        * Initialize system.
        * @param context System context.
        * @return Result code.
        */
        virtual std::error_code start(SystemContext& context) = 0;
    };
}