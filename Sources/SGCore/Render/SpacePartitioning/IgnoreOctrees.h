//
// Created by ilya on 10.03.24.
//

#ifndef SUNGEARENGINE_IGNOREOCTREES_H
#define SUNGEARENGINE_IGNOREOCTREES_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct IgnoreOctrees : ECS::Component<IgnoreOctrees, const IgnoreOctrees>
    {
    private:
        bool m_dummy = true;
    };
}

#endif //SUNGEARENGINE_IGNOREOCTREES_H
