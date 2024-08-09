//
// Created by ilya on 14.04.24.
//

#include "AudioListener.h"
#include "AudioUtils.h"

void SGCore::AudioListener::setPosition(const glm::vec3& position) noexcept
{
    AL_CALL(alListener3f, AL_POSITION, position.x, position.y, position.z);
}

glm::vec3 SGCore::AudioListener::getPosition() noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetListener3f, AL_POSITION, &x, &y, &z);
    
    return { x, y, z };
}

void SGCore::AudioListener::setVelocity(const glm::vec3& velocity) noexcept
{
    AL_CALL(alListener3f, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

glm::vec3 SGCore::AudioListener::getVelocity() noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetListener3f, AL_VELOCITY, &x, &y, &z);
    
    return { x, y, z };
}

void SGCore::AudioListener::setOrientation(const glm::vec3& forward, const glm::vec3& up) noexcept
{
    ALfloat floats[] = { forward.x, forward.y, forward.z, up.x, up.y, up.z };
    AL_CALL(alListenerfv, AL_ORIENTATION, floats);
}

void SGCore::AudioListener::getOrientation(glm::vec3& forward, glm::vec3& up) noexcept
{
    float values[6];
    
    AL_CALL(alGetListenerfv, AL_ORIENTATION, values);
    
    forward.x = values[0];
    forward.y = values[1];
    forward.z = values[2];
    
    up.x = values[3];
    up.y = values[4];
    up.z = values[5];
}

void SGCore::AudioListener::setGain(const float& gain) noexcept
{
    AL_CALL(alListenerf, AL_GAIN, gain);
}

float SGCore::AudioListener::getGain() noexcept
{
    float gain;
    AL_CALL(alGetListenerf, AL_GAIN, &gain);
    return gain;
}
