#ifndef SUNGEARENGINE_SPHERECOMPONENT_H
#define SUNGEARENGINE_SPHERECOMPONENT_H

#include "IComplexGizmo.h"

namespace SGCore
{
    struct SphereGizmo : public IComplexGizmo
    {
        friend class GizmosMeshesRebuilder;

        float m_radius = 5.0;
        float m_angleIncrement = 18.0;

        SphereGizmo();

    private:
        float m_lastRadius = 0.0;
        float m_lastAngleIncrement = 0.0;
    };
}

#endif //SUNGEARENGINE_SPHERECOMPONENT_H
