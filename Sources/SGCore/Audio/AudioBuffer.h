//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOBUFFER_H
#define SUNGEARENGINE_AUDIOBUFFER_H

#include "AudioUtils.h"

namespace SGCore
{
    struct AudioBuffer
    {
        friend struct AudioSource;
        
        ~AudioBuffer();
        
        void create() noexcept;
        void destroy() noexcept;
        
        void putData(const uint16_t& audioFormat, const void* data, const size_t& dataSize, const uint16_t& sampleRate) const noexcept;
        
        void setFrequency(const std::uint32_t& frequency) noexcept;
        [[nodiscard]] std::uint32_t getFrequency() const noexcept;
        
        void setBitsPerSample(const std::uint16_t& bitsPerSample) noexcept;
        [[nodiscard]] std::uint16_t getBitsPerSample() const noexcept;
        
        void setNumChannels(const std::uint16_t& numChannels) noexcept;
        [[nodiscard]] std::uint16_t getNumChannels() const noexcept;
        
    private:
        std::uint32_t m_frequency = 60;
        std::uint16_t m_bitsPerSample = 8;
        std::uint16_t m_numChannels = 2;
        
        ALuint m_handler = -1;
        bool m_isValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOBUFFER_H
