#ifndef SUNGEARENGINE_LINECOMPONENT_H
#define SUNGEARENGINE_LINECOMPONENT_H

#include "IPrimitiveComponent.h"

namespace Core::ECS
{
    class LineComponent : public IPrimitiveComponent
    {
    public:
        LineComponent() noexcept;

        void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept final;
    };
}

#endif //SUNGEARENGINE_LINECOMPONENT_H
