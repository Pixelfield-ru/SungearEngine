//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <cstdint>

namespace pf::core
{
    namespace impl
    {
        static std::uint32_t& counter() noexcept
        {
            static std::uint32_t current {};
            return current;
        }
    }

    /**
     * @return Sequenced index for type.
     */
    template<typename>
    static std::uint32_t typeIndex() noexcept
    {
        static std::uint32_t index = []() {
            auto& current = impl::counter();
            const auto idx = current;
            ++current;
            return idx;
        }();

        return index;
    }
}