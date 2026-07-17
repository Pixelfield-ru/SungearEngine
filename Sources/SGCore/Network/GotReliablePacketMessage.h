//
// Created by stuka on 17.07.2026.
//

#pragma once

#include "sgcore_export.h"
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore::Net
{
    struct SGCORE_EXPORT GotReliablePacketMessage
    {
        sg_implement_nonvirtual_type_id(SGCore::Net::GotReliablePacketMessage);

        static constexpr bool use_rudp = true;
    };
}