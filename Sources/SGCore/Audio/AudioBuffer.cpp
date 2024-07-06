//
// Created by ilya on 14.04.24.
//

#include "AudioBuffer.h"

SGCore::AudioBuffer::~AudioBuffer()
{
    destroy();
}

void SGCore::AudioBuffer::destroy() noexcept
{
    if(m_isValid)
    {
        AL_CALL(alDeleteBuffers, 1, &m_handler);
        
        m_isValid = false;
    }
}

void SGCore::AudioBuffer::create() noexcept
{
    destroy();
    
    AL_CALL_E(m_isValid, alGenBuffers, 1, &m_handler);
}

void SGCore::AudioBuffer::putData(const uint16_t& numChannels,
                                  const std::uint16_t& bitsPerSample,
                                  const void* data,
                                  const size_t& dataSize,
                                  const std::uint16_t& sampleRate) noexcept
{
    if(m_isValid)
    {
        ALenum alFormat = 0;
        if(numChannels == 1 && bitsPerSample == 8)
        {
            alFormat = AL_FORMAT_MONO8;
        }
        else if(numChannels == 1 && bitsPerSample == 16)
        {
            alFormat = AL_FORMAT_MONO16;
        }
        else if(numChannels == 2 && bitsPerSample == 8)
        {
            alFormat = AL_FORMAT_STEREO8;
        }
        else if(numChannels == 2 && bitsPerSample == 16)
        {
            alFormat = AL_FORMAT_STEREO16;
        }
        
        AL_CALL(alBufferData, m_handler, alFormat, data, dataSize, sampleRate);

        for(std::uint64_t i = 0; i < dataSize; ++i)
        {
            m_data.push_back(*((char*) data + i));
        }

        setFrequency(m_frequency);
        setBitsPerSample(bitsPerSample);
        setNumChannels(numChannels);
    }
}

void SGCore::AudioBuffer::setFrequency(const std::uint32_t& frequency) noexcept
{
    m_frequency = frequency;
    if(m_isValid)
    {
        AL_CALL(alBufferf, m_handler, AL_FREQUENCY, frequency);
    }
}

std::uint32_t SGCore::AudioBuffer::getFrequency() const noexcept
{
    return m_frequency;
}

void SGCore::AudioBuffer::setBitsPerSample(const uint16_t& bitsPerSample) noexcept
{
    m_bitsPerSample = bitsPerSample;
    if(m_isValid)
    {
        AL_CALL(alBufferi, m_handler, AL_BITS, bitsPerSample);
    }
}

std::uint16_t SGCore::AudioBuffer::getBitsPerSample() const noexcept
{
    return m_bitsPerSample;
}

void SGCore::AudioBuffer::setNumChannels(const uint16_t& numChannels) noexcept
{
    m_numChannels = numChannels;
    if(m_isValid)
    {
        AL_CALL(alBufferi, m_handler, AL_CHANNELS, numChannels);
    }
}

std::uint16_t SGCore::AudioBuffer::getNumChannels() const noexcept
{
    return m_numChannels;
}
