#ifndef SUNGEARENGINE_BOXCOMPONENT_H
#define SUNGEARENGINE_BOXCOMPONENT_H

#include "IPrimitiveComponent.h"

namespace Core::ECS
{
    class BoxComponent : public IPrimitiveComponent
    {
    public:
        glm::vec3 m_size { 5.0, 5.0, 5.0 };

        BoxComponent();

        void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept final;

    private:
        glm::vec2 m_lastSize { 0.0, 0.0 };
    };
}

#endif //SUNGEARENGINE_BOXCOMPONENT_H
