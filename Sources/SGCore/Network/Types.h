//
// Created by stuka on 21.07.2026.
//

#pragma once

#include <cstdint>
#include <utility>

namespace SGCore::Net
{
    using session_id_t = std::int64_t;
    using packet_id_t = std::uint32_t;

    template<typename MsgT>
    concept NetMessage = requires { std::declval<MsgT>().getTypeIDStatic(); };
}
