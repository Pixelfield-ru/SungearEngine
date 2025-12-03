//
// Created by stuka on 22.02.2025.
//

#ifndef BORDERRADIUSALTERNATIVEVALUE_H
#define BORDERRADIUSALTERNATIVEVALUE_H

#include "SGCore/UI/Math/DynValueNode.h"

namespace SGCore::UI
{
    struct BorderRadiusAlternativeValue final
    {
        Ref<DynValueNode> m_radiusX;
        Ref<DynValueNode> m_radiusY;
    };
}

#endif //BORDERRADIUSALTERNATIVEVALUE_H
