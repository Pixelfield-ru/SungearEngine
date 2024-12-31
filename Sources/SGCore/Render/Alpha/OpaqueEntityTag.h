//
// Created by stuka on 31.12.2024.
//

#ifndef SUNGEARENGINE_OPAQUEENTITYTAG_H
#define SUNGEARENGINE_OPAQUEENTITYTAG_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct OpaqueEntityTag : public ECS::Component<OpaqueEntityTag, const OpaqueEntityTag>
    {
    private:
        bool m_dummy = false;
    };
}

#endif //SUNGEARENGINE_OPAQUEENTITYTAG_H
