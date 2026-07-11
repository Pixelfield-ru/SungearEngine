//
// Created by stuka on 08.07.2026.
//

#pragma once

#include <string>

#include "sgcore_export.h"
#include "SGCore/Utils/StaticTypeID.h"

namespace SGCore::Net
{
    struct SGCORE_EXPORT ClientConnectedMessage
    {
        sg_implement_nonvirtual_type_id(SGCore::Net::ClientConnectedMessage);
    private:
        bool m_dummy = false;
    };
}