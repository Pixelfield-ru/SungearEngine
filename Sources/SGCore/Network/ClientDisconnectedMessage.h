//
// Created by stuka on 19.08.2025.
//

#pragma once

#include <string>

#include "sgcore_export.h"
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore::Net
{
    struct SGCORE_EXPORT ClientDisconnectedMessage
    {
        sg_implement_nonvirtual_type_id(SGCore::Net::ClientDisconnectedMessage);
    };
}