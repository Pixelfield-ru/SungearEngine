//
// Created by stuka on 16.08.2026.
//

#include "SGCore/Serde/Serde.h"

namespace SGCore::Serde
{
    #include "SerializerForward.h"

    ECS::VisitorsRegistry& SerializerImpl<FormatType::JSON>::componentsVisitors() noexcept
    {
        static ECS::VisitorsRegistry registry;

        return registry;
    }
}
