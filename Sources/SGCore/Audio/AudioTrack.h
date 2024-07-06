//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIOTRACK_H
#define SUNGEARENGINE_AUDIOTRACK_H

#include <cstdint>
#include <cstddef>
#include <string>

// WAV SPECIFICATION: http://soundfile.sapp.org/doc/WaveFormat/

namespace SGCore
{
    enum class AudioTrackType
    {
        WAV,
        OGG,
        MP3
    };
    
    struct AudioTrack
    {
        ~AudioTrack();
        
        void loadFromMemory(const char* buffer, const size_t& bufferSize, const AudioTrackType& type);
        void deleteData() noexcept;
        
        [[nodiscard]] std::uint16_t getAudioFormat() const noexcept;
        [[nodiscard]] std::uint16_t getNumChannels() const noexcept;
        [[nodiscard]] std::uint16_t getSampleRate() const noexcept;
        [[nodiscard]] std::uint16_t getByteRate() const noexcept;
        [[nodiscard]] std::uint16_t getBlockAlign() const noexcept;
        [[nodiscard]] std::uint16_t getBitsPerSample() const noexcept;
        [[nodiscard]] char* getDataBuffer() const noexcept;
        [[nodiscard]] std::uint32_t getDataBufferSize() const noexcept;
        
        [[nodiscard]] std::string getSummary() const noexcept;
        
    private:
        // if WAV:
        // PCM = 1
        // if OGG: not provided
        std::int16_t m_audioFormat = 0;
        // if WAV or OGG:
        // MONO = 1
        // STEREO = 2
        std::int32_t m_numChannels = 0;
        // PROVIDED IN WAV, OGG
        std::int32_t m_sampleRate = 0;
        // if WAV:
        // m_sampleRate * m_numChannels * m_bitsPerSample / 8
        // if OGG: not provided
        std::uint32_t m_byteRate = 0;
        // if WAV:
        // m_numChannels * m_bitsPerSample / 8
        // if OGG: not provided
        std::uint16_t m_blockAlign = 0;
        // if OGG: 16 bits
        std::uint16_t m_bitsPerSample = 0;
        
        // buffer in subchunk 'data'
        char* m_dataBuffer = nullptr;
        std::uint32_t m_dataBufferSize = 0;
    };
}

#endif //SUNGEARENGINE_AUDIOTRACK_H
