//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOBUFFER_H
#define SUNGEARENGINE_AUDIOBUFFER_H

#include "AudioUtils.h"

#include "SGCore/Annotations/Annotations.h"
#include "SGCore/Scene/Serializer.h"

namespace SGCore
{
    sg_struct()
    struct AudioBuffer
    {
        sg_serializer_as_friend(AudioBuffer)

        friend struct AudioSource;
        
        ~AudioBuffer();
        
        void create() noexcept;
        void destroy() noexcept;
        
        void putData(const uint16_t& numChannels, const std::uint16_t& bitsPerSample, const void* data, const size_t& dataSize, const uint16_t& sampleRate) noexcept;
        
        void setFrequency(const std::uint32_t& frequency) noexcept;
        [[nodiscard]] std::uint32_t getFrequency() const noexcept;
        
        void setBitsPerSample(const std::uint16_t& bitsPerSample) noexcept;
        [[nodiscard]] std::uint16_t getBitsPerSample() const noexcept;
        
        void setNumChannels(const std::uint16_t& numChannels) noexcept;
        [[nodiscard]] std::uint16_t getNumChannels() const noexcept;
        
    private:
        sg_member()
        std::uint32_t m_frequency = 60;
        sg_member()
        std::uint16_t m_bitsPerSample = 8;
        sg_member()
        std::uint16_t m_numChannels = 2;

        sg_member()
        std::vector<char> m_data;
        
        ALuint m_handler = -1;
        bool m_isValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOBUFFER_H
