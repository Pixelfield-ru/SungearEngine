#ifndef SUNGEARENGINE_BOXCOMPONENT_H
#define SUNGEARENGINE_BOXCOMPONENT_H

#include "ComplexPrimitiveComponent.h"

namespace Core::ECS
{
    class BoxComponent : public ComplexPrimitiveComponent
    {
        friend class PrimitivesUpdaterSystem;

    public:
        glm::vec3 m_size { 5.0, 5.0, 5.0 };

        BoxComponent();

        void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept final;

    private:
        glm::vec3 m_lastSize { 5.0, 5.0, 5.0 };

        /**
         * Creates vertices, indexes, and mesh of a cube.
         */
        void build();
    };
}

#endif //SUNGEARENGINE_BOXCOMPONENT_H
