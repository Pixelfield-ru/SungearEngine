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
    
    AL_CALL_E(m_isValid, alGenSources, 1, &m_handle);
    
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
        AL_CALL(alDeleteSources, 1, &m_handle);
        m_isValid = false;
    }
}

void SGCore::AudioSource::attachAudioTrack(const AssetRef<AudioTrackAsset>& audioTrackAsset) noexcept
{
    m_attachedAudioTrack = audioTrackAsset;
    
    if(m_isValid && audioTrackAsset)
    {
        std::cout << "m_handler: " << m_handle << ", audio track handler: " << audioTrackAsset->getALHandler() << std::endl;
        AL_CALL(alSourcei, m_handle, AL_BUFFER, audioTrackAsset->getALHandler());
    }
}

void SGCore::AudioSource::detachAudioTrack() const noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcei, m_handle, AL_BUFFER, 0);
    }
}

void SGCore::AudioSource::doSetState(PlayableState state) noexcept
{
    if(m_isValid)
    {
        switch(state)
        {
            case PlayableState::SG_PLAYING:
                AL_CALL(alSourcePlay, m_handle);
                break;
            case PlayableState::SG_PAUSED:
                AL_CALL(alSourcePause, m_handle);
                break;
            case PlayableState::SG_STOPPED:
                AL_CALL(alSourceStop, m_handle);
                break;
        }
    }
}

void SGCore::AudioSource::resetTimelineTime() noexcept
{
    setCurrentTime(0.0f);
}

SGCore::PlayableState SGCore::AudioSource::getState() const noexcept
{
    ALint alState;
    AL_CALL(alGetSourcei, m_handle, AL_SOURCE_STATE, &alState);
    
    switch(alState)
    {
        case AL_PLAYING:
            return PlayableState::SG_PLAYING;
        case AL_PAUSED:
            return PlayableState::SG_PAUSED;
        case AL_STOPPED:
            return PlayableState::SG_STOPPED;
    }
    
    return PlayableState::SG_STOPPED;
}

void SGCore::AudioSource::setType(const SGCore::AudioSourceType& type) noexcept
{
    m_type = type;
    
    if(m_isValid)
    {
        switch(type)
        {
            case AudioSourceType::SG_WORLD:
            {
                AL_CALL(alSourcei, m_handle, AL_SOURCE_RELATIVE, AL_FALSE);
                break;
            }
            case AudioSourceType::SG_AMBIENT:
            {
                AL_CALL(alSourcei, m_handle, AL_SOURCE_RELATIVE, AL_TRUE);
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
        m_handle = other.m_handle;
        m_isValid = other.m_isValid;
        m_type = other.m_type;

        attachAudioTrack(m_attachedAudioTrack);
        other.detachAudioTrack();
    }
    
    return *this;
}

void SGCore::AudioSource::setPosition(const glm::vec3& position) noexcept
{
    AL_CALL(alSource3f, m_handle, AL_POSITION, position.x, position.y, position.z);
}

glm::vec3 SGCore::AudioSource::getPosition() const noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetSource3f, m_handle, AL_POSITION, &x, &y, &z);
    
    return { x, y, z };
}

void SGCore::AudioSource::setVelocity(const glm::vec3& velocity) noexcept
{
    AL_CALL(alSource3f, m_handle, AL_VELOCITY, velocity.x, velocity.y, velocity.z);
}

glm::vec3 SGCore::AudioSource::getVelocity() const noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetSource3f, m_handle, AL_VELOCITY, &x, &y, &z);

    return { x, y, z };
}

void SGCore::AudioSource::setDirection(const glm::vec3& direction) noexcept
{
    AL_CALL(alSource3f, m_handle, AL_DIRECTION, direction.x, direction.y, direction.z);
}

glm::vec3 SGCore::AudioSource::getDirection() const noexcept
{
    float x;
    float y;
    float z;
    
    AL_CALL(alGetSource3f, m_handle, AL_DIRECTION, &x, &y, &z);
    
    return { x, y, z };
}

void SGCore::AudioSource::setGain(const float& gain) noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcef, m_handle, AL_GAIN, gain);
    }
}

float SGCore::AudioSource::getGain() const noexcept
{
    float val = 0;
    if(m_isValid)
    {
        AL_CALL(alGetSourcef, m_handle, AL_GAIN, &val);
    }
    
    return val;
}

void SGCore::AudioSource::setPitch(const float& pitch) noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcef, m_handle, AL_PITCH, pitch);
    }
}

float SGCore::AudioSource::getPitch() const noexcept
{
    float val = 0;
    if(m_isValid)
    {
        AL_CALL(alGetSourcef, m_handle, AL_PITCH, &val);
    }
    
    return val;
}

void SGCore::AudioSource::setIsLooping(bool isLooping) noexcept
{
    m_isLooping = isLooping;
    
    if(m_isValid)
    {
        AL_CALL(alSourcei, m_handle, AL_LOOPING, isLooping ? AL_TRUE : AL_FALSE);
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
        AL_CALL(alSourcef, m_handle, AL_ROLLOFF_FACTOR, rolloffFactor);
    }
}

float SGCore::AudioSource::getRolloffFactor() const noexcept
{
    float val = 0;
    if(m_isValid)
    {
        AL_CALL(alGetSourcef, m_handle, AL_ROLLOFF_FACTOR, &val);
    }
    
    return 0;
}

void SGCore::AudioSource::setCurrentTime(float time) const noexcept
{
    if(!m_isValid) return;

    AL_CALL(alSourcef, m_handle, AL_SEC_OFFSET, time);
}

float SGCore::AudioSource::getCurrentTime() const noexcept
{
    if(!m_isValid) return 0.0f;

    float time {};
    AL_CALL(alGetSourcef, m_handle, AL_SEC_OFFSET, &time);

    return time;
}

