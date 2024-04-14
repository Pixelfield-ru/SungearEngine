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

void c(const std::source_location& srcLoc, void(*Func)(int, unsigned int*), int n, unsigned int* buf)
{
    Func(n, buf);
    // alGenBuffers(1, handler);
}

void SGCore::AudioBuffer::create() noexcept
{
    destroy();
    
    AL_CALL_E(m_isValid, alGenBuffers, 1, &m_handler);
    
    setFrequency(m_frequency);
    setBitsPerSample(m_bitsPerSample);
    setNumChannels(m_numChannels);
}

void SGCore::AudioBuffer::putData(const std::uint16_t& audioFormat,
                                  const void* data,
                                  const size_t& dataSize,
                                  const std::uint16_t& sampleRate) const noexcept
{
    if(m_isValid)
    {
        AL_CALL(alBufferData, m_handler, audioFormat, data, dataSize, sampleRate);
    }
}

void SGCore::AudioBuffer::setFrequency(const std::uint32_t& frequency) noexcept
{
    m_frequency = frequency;
    if(m_isValid)
    {
        AL_CALL(alBufferi, m_handler, AL_FREQUENCY, frequency);
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
