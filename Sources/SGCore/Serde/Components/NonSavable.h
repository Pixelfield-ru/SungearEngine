//
// Created by stuka on 10.09.2024.
//

#ifndef SUNGEARENGINE_NONSAVABLE_H
#define SUNGEARENGINE_NONSAVABLE_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct NonSavable : ECS::Component<NonSavable, const NonSavable>
    {
    private:
        bool m_dummy { };
    };
}

#endif //SUNGEARENGINE_NONSAVABLE_H
