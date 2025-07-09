//
// Created by stuka on 08.07.2025.
//

#ifndef SUNGEARENGINE_CSMTARGET_H
#define SUNGEARENGINE_CSMTARGET_H

#include "CSMCascade.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    struct CSMTarget : ECS::Component<CSMTarget, const CSMTarget>
    {
        CSMTarget();

        const std::vector<CSMCascade>& getCascades() const noexcept;

    private:
        std::vector<CSMCascade> m_levels; //{ 50.0f, 25.0f, 10.0f, 2.0f };
    };
}

#endif // SUNGEARENGINE_CSMTARGET_H
