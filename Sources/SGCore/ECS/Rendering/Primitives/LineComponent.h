#ifndef SUNGEARENGINE_LINECOMPONENT_H
#define SUNGEARENGINE_LINECOMPONENT_H

#include "IPrimitiveComponent.h"

namespace Core::ECS
{
    class LineComponent : public IPrimitiveComponent
    {
    public:
        glm::vec3 m_start;
        glm::vec3 m_end;
    };
}

#endif //SUNGEARENGINE_LINECOMPONENT_H
