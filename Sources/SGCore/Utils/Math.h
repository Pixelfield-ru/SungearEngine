#ifndef SUNGEARENGINE_MATH_H
#define SUNGEARENGINE_MATH_H

#include "glm/vec3.hpp"

namespace Core::Utils
{
    class MathUtils
    {
    public:
        constexpr const static glm::vec3 left3 = glm::vec3(-1, 0, 0);
        constexpr const static glm::vec3 forward3 = glm::vec3(0, 0, 1);
        constexpr const static glm::vec3 up3 = glm::vec3(0, 1, 0);
    };
}

#endif //SUNGEARENGINE_MATH_H
