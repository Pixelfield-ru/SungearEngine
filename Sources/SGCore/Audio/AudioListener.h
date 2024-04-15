//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOLISTENER_H
#define SUNGEARENGINE_AUDIOLISTENER_H

#include <glm/vec3.hpp>

namespace SGCore
{
    struct AudioListener
    {
        static void setPosition(const glm::vec3& position) noexcept;
        static glm::vec3 getPosition() noexcept;
        
        static void setVelocity(const glm::vec3& velocity) noexcept;
        static glm::vec3 getVelocity() noexcept;
        
        static void setOrientation(const glm::vec3& forward, const glm::vec3& up) noexcept;
        static void getOrientation(glm::vec3& forward, glm::vec3& up) noexcept;
        
        static void setGain(const float& gain) noexcept;
        static float getGain() noexcept;
    };
}

#endif //SUNGEARENGINE_AUDIOLISTENER_H
