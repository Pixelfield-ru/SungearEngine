#ifndef SUNGEARENGINE_SPHERECOMPONENT_H
#define SUNGEARENGINE_SPHERECOMPONENT_H

#include "ComplexPrimitiveComponent.h"

namespace Core::ECS
{
    class SphereComponent : public ComplexPrimitiveComponent
    {
        friend class PrimitivesUpdaterSystem;

    public:
        float m_radius = 5.0;
        float m_angleIncrement = 18.0;

        SphereComponent();

        void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept final;

    private:
        float m_lastRadius = 0.0;
        float m_lastAngleIncrement = 0.0;
    };
}

#endif //SUNGEARENGINE_SPHERECOMPONENT_H
