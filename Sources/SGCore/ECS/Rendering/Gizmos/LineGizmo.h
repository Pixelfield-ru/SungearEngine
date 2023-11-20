#ifndef SUNGEARENGINE_LINECOMPONENT_H
#define SUNGEARENGINE_LINECOMPONENT_H

#include "IGizmo.h"

namespace SGCore
{
    struct LineGizmo : public IGizmo
    {
        LineGizmo() noexcept;

        void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) noexcept final;
    };
}

#endif //SUNGEARENGINE_LINECOMPONENT_H
