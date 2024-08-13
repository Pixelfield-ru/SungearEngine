//
// Created by ilya on 14.04.24.
//

#include "AudioSource.h"

SGCore::AudioSource::~AudioSource()
{
    destroy();
}

void SGCore::AudioSource::create() noexcept
{
    destroy();
    
    AL_CALL_E(m_isValid, alGenSources, 1, &m_handler);
    
    // auto attaching to buffer if it is not nullptr
    attachAudioTrack(m_attachedAudioTrack);
    
    setType(getType());
    setState(getState());
}

void SGCore::AudioSource::destroy() noexcept
{
    if(m_isValid)
    {
        detachAudioTrack();
        AL_CALL(alDeleteSources, 1, &m_handler);
        m_isValid = false;
    }
}

void SGCore::AudioSource::attachAudioTrack(const Ref<AudioTrackAsset>& audioTrackAsset) noexcept
{
    m_attachedAudioTrack = audioTrackAsset;
    
    if(m_isValid && audioTrackAsset)
    {
        AL_CALL(alSourcei, m_handler, AL_BUFFER, audioTrackAsset->getALHandler());
    }
}

void SGCore::AudioSource::detachAudioTrack() const noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcei, m_handler, AL_BUFFER, 0);
    }
}

void SGCore::AudioSource::setState(const SGCore::AudioSourceState& state) noexcept
{
    if(m_isValid)
    {
        switch(state)
        {
            case AudioSourceState::PLAYING:
                AL_CALL(alSourcePlay, m_handler);
                break;
            case AudioSourceState::PAUSED:
                AL_CALL(alSourcePause, m_handler);
                break;
            case AudioSourceState::STOPPED:
                AL_CALL(alSourceStop, m_handler);
                break;
        }
    }
}

SGCore::AudioSourceState SGCore::AudioSource::getState() const noexcept
{
    ALint alState;
    AL_CALL(alGetSourcei, m_handler, AL_SOURCE_STATE, &alState);
    
    switch(alState)
    {
        case AL_PLAYING:
            return AudioSourceState::PLAYING;
        case AL_PAUSED:
            return AudioSourceState::PAUSED;
        case AL_STOPPED:
            return AudioSourceState::STOPPED;
    }
    
    return AudioSourceState::STOPPED;
}

void SGCore::AudioSource::setType(const SGCore::AudioSourceType& type) noexcept
{
    m_type = type;
    
    if(m_isValid)
    {
        switch(type)
        {
            case AudioSourceType::POSITIONAL:
            {
                AL_CALL(alSourcei, m_handler, AL_SOURCE_RELATIVE, AL_FALSE);
                break;
            }
            case AudioSourceType::AMBIENT:
            {
                AL_CALL(alSourcei, m_handler, AL_SOURCE_RELATIVE, AL_TRUE);
                setPosition({ 0, 0, 0 });
                break;
            }
        }
    }
}

SGCore::AudioSourceType SGCore::AudioSource::getType() const noexcept
{
    return m_type;
}

SGCore::AudioSource& SGCore::AudioSource::operator=(const SGCore::AudioSource& other) noexcept
{
    if(this == std::addressof(other)) return *this;
    
    if(m_isValid)
    {
        m_attachedAudioTrack = other.m_attachedAudioTrack;

        attachAudioTrack(m_attachedAudioTrack);
        
        setGain(other.getGain());
        setPitch(other.getPitch());
        setIsLooping(other.isLooping());
        setState(other.getState());
        setType(other.getType());
    }
    
    return *this;
}

SGCore::AudioSource& SGCore::AudioSource::operator=(SGCore::AudioSource&& other) noexcept
{
    if(this == std::addressof(other)) return *this;
    
    if(m_isValid)
    {
        destroy();

        m_attachedAudioTrack = other.m_attachedAudioTrack;
        m_handler = other.m_handler;
        m_isValid = other.m_isValid;
        m_type = other.m_type;

        attachAudioTrack(m_attachedAudioTrack);
        other.detachAudioTrack();
    }
    
    return *this;
}

void SGCore::AudioSource::setPosition(const glm::vec3& position) noexcept
{
    AL_CALL(alSource3f, m_handler, AL_POSITION, position.x, position.y, position.z);
}

glm::vec3 SGCore::AudioSource::getPosition() const noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetSource3f, m_handler, AL_POSITION, &x, &y, &z);
    
    return { x, y, z };
}

void SGCore::AudioSource::setVelocity(const glm::vec3& velocity) noexcept
{
    AL_CALL(alSource3f, m_handler, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

glm::vec3 SGCore::AudioSource::getVelocity() const noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetSource3f, m_handler, AL_VELOCITY, &x, &y, &z);

    return { x, y, z };
}

void SGCore::AudioSource::setDirection(const glm::vec3& direction) noexcept
{
    AL_CALL(alSource3f, m_handler, AL_DIRECTION, direction.x, direction.y, direction.z);
}

glm::vec3 SGCore::AudioSource::getDirection() const noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetSource3f, m_handler, AL_DIRECTION, &x, &y, &z);
    
    return { x, y, z };
}

void SGCore::AudioSource::setGain(const float& gain) noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcef, m_handler, AL_GAIN, gain);
    }
}

float SGCore::AudioSource::getGain() const noexcept
{
    float val = 0;
    if(m_isValid)
    {
        AL_CALL(alGetSourcef, m_handler, AL_GAIN, &val);
    }
    
    return val;
}

void SGCore::AudioSource::setPitch(const float& pitch) noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcef, m_handler, AL_PITCH, pitch);
    }
}

float SGCore::AudioSource::getPitch() const noexcept
{
    float val = 0;
    if(m_isValid)
    {
        AL_CALL(alGetSourcef, m_handler, AL_PITCH, &val);
    }
    
    return val;
}

void SGCore::AudioSource::setIsLooping(bool isLooping) noexcept
{
    m_isLooping = isLooping;
    
    if(m_isValid)
    {
        AL_CALL(alSourcei, m_handler, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
    }
}

bool SGCore::AudioSource::isLooping() const noexcept
{
    return m_isLooping;
}

void SGCore::AudioSource::setRolloffFactor(const float& rolloffFactor) noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcef, m_handler, AL_ROLLOFF_FACTOR, rolloffFactor);
    }
}

float SGCore::AudioSource::getRolloffFactor() const noexcept
{
    float val = 0;
    if(m_isValid)
    {
        AL_CALL(alGetSourcef, m_handler, AL_ROLLOFF_FACTOR, &val);
    }
    
    return 0;
}

