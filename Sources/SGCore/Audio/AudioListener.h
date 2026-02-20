//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOLISTENER_H
#define SUNGEARENGINE_AUDIOLISTENER_H

#include <glm/vec3.hpp>

namespace SGCore
{
    /**
     * Struct to manage parameters of audio listener. Listener can be only one.
     */
    struct AudioListener
    {
        /**
         * Sets 3D world position of audio listener.
         * @param position Position of listener.
         */
        static void setPosition(const glm::vec3& position) noexcept;

        /**
         * @return 3D world position of audio listener.
         */
        static glm::vec3 getPosition() noexcept;

        /**
         * Sets 3D world velocity of audio listener.
         * @param velocity 3D world velocity of listener.
         */
        static void setVelocity(const glm::vec3& velocity) noexcept;

        /**
         * @return 3D world velocity of audio listener.
         */
        static glm::vec3 getVelocity() noexcept;

        /**
         * Sets 3D orientation of audio listener.
         * @param forward Normalized audio listener forward vector (where the listener is looking).
         * @param up Normalized audio listener up vector.
         */
        static void setOrientation(const glm::vec3& forward, const glm::vec3& up) noexcept;

        /**
         * Writes forward and up vectors of audio listeners in forward and up params.
         * @param forward Parameter to save normalized forward vector of audio listener.
         * @param up Parameter to save normalized up vector of audio listener.
         */
        static void getOrientation(glm::vec3& forward, glm::vec3& up) noexcept;

        /**
         * Sets audio listener`s gain.
         * @param gain Gain value.
         */
        static void setGain(const float& gain) noexcept;

        /**
         * @return Audio listener`s gain.
         */
        static float getGain() noexcept;
    };
}

#endif //SUNGEARENGINE_AUDIOLISTENER_H
