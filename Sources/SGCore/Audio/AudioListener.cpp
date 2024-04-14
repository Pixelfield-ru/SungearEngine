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

void SGCore::AudioListener::setOrientation(const glm::vec3& orientation) noexcept
{
    AL_CALL(alListener3f, AL_ORIENTATION, orientation.x, orientation.y, orientation.z);
}

glm::vec3 SGCore::AudioListener::getOrientation() noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetListener3f, AL_ORIENTATION, &x, &y, &z);
    
    return { x, y, z };
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
