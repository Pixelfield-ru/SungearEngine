//
// Created by stuka on 08.07.2025.
//

#ifndef SUNGEARENGINE_CSMTARGET_H
#define SUNGEARENGINE_CSMTARGET_H

#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct CSMTarget : ECS::Component<CSMTarget, const CSMTarget>
    {
        const std::vector<float>& getLevels() const noexcept;

    private:
        std::vector<float> m_levels { 50.0f, 25.0f, 10.0f, 2.0f };
    };
}

#endif // SUNGEARENGINE_CSMTARGET_H
