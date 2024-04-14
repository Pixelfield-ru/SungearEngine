//
// Created by ilya on 14.04.24.
//

#include "AudioSource.h"

#include "AudioBuffer.h"

SGCore::AudioSource::~AudioSource()
{
    destroy();
}

void SGCore::AudioSource::create() noexcept
{
    destroy();
    
    AL_CALL_E(m_isValid, alGenSources, 1, &m_handler);
    
    // auto attaching to buffer if it is not nullptr
    attachBuffer(m_attachedBuffer.lock());
    
    setState(m_state);
}

void SGCore::AudioSource::destroy() noexcept
{
    if(m_isValid)
    {
        detachBuffer();
        AL_CALL(alDeleteSources, 1, &m_handler);
        m_isValid = false;
    }
}

void SGCore::AudioSource::attachBuffer(const Ref<AudioBuffer>& buffer) noexcept
{
    m_attachedBuffer = buffer;
    
    if(m_isValid && buffer)
    {
        AL_CALL(alSourcei, m_handler, AL_BUFFER, buffer->m_handler);
    }
}

void SGCore::AudioSource::detachBuffer() const noexcept
{
    if(m_isValid)
    {
        AL_CALL(alSourcei, m_handler, AL_BUFFER, 0);
    }
}

void SGCore::AudioSource::setState(const SGCore::AudioSourceState& state) noexcept
{
    m_state = state;
    if(m_isValid)
    {
        switch(state)
        {
            case SOURCE_PLAYING:
                AL_CALL(alSourcei, m_handler, AL_SOURCE_STATE, AL_PLAYING);
                break;
            case SOURCE_PAUSED:
                AL_CALL(alSourcei, m_handler, AL_SOURCE_STATE, AL_PAUSED);
                break;
            case SOURCE_STOPPED:
                AL_CALL(alSourcei, m_handler, AL_SOURCE_STATE, AL_STOPPED);
                break;
        }
    }
}

SGCore::AudioSourceState SGCore::AudioSource::getState() const noexcept
{
    return m_state;
}

SGCore::AudioSource& SGCore::AudioSource::operator=(const SGCore::AudioSource& other) noexcept
{
    if(this == std::addressof(other)) return *this;
    
    if(m_isValid)
    {
        m_attachedBuffer = other.m_attachedBuffer;
        
        attachBuffer(m_attachedBuffer.lock());
    }
    
    return *this;
}

SGCore::AudioSource& SGCore::AudioSource::operator=(SGCore::AudioSource&& other) noexcept
{
    if(this == std::addressof(other)) return *this;
    
    if(m_isValid)
    {
        destroy();
        
        m_attachedBuffer = other.m_attachedBuffer;
        m_handler = other.m_handler;
        m_isValid = other.m_isValid;
        
        attachBuffer(m_attachedBuffer.lock());
        other.detachBuffer();
    }
    
    return *this;
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
