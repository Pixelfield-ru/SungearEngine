//
// Created by stuka on 31.12.2024.
//

#ifndef SUNGEARENGINE_TRANSPARENTENTITYTAG_H
#define SUNGEARENGINE_TRANSPARENTENTITYTAG_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct TransparentEntityTag : public ECS::Component<TransparentEntityTag, const TransparentEntityTag>
    {
    private:
        bool m_dummy = false;
    };
}

#endif //SUNGEARENGINE_TRANSPARENTENTITYTAG_H
