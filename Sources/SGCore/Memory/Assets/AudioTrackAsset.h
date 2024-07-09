//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIOTRACKASSET_H
#define SUNGEARENGINE_AUDIOTRACKASSET_H

#include "ByteFileAsset.h"
#include "al.h"

namespace SGCore
{
    enum class AudioTrackType
    {
        WAV,
        OGG,
        MP3,
        UNKNOWN
    };

    struct AudioTrackAsset : public ByteFileAsset
    {
        ~AudioTrackAsset();

        // TODO:
        void serializeData(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;
        // TODO:
        void serializeMeta(rapidjson::Document& toDocument, rapidjson::Value& parent, const std::string& varName) override;

        void createALBuffer() noexcept;
        void destroyALBuffer() noexcept;

        void putData(const uint16_t& numChannels, const std::uint16_t& bitsPerSample, const void* data, const size_t& dataSize, const uint16_t& sampleRate) noexcept;
        void deleteData() noexcept;

        [[nodiscard]] std::uint16_t getAudioFormat() const noexcept;

        void setFrequency(const std::uint32_t& frequency) noexcept;
        [[nodiscard]] std::uint32_t getFrequency() const noexcept;

        void setNumChannels(const std::uint16_t& numChannels) noexcept;
        [[nodiscard]] std::uint16_t getNumChannels() const noexcept;

        [[nodiscard]] std::uint16_t getSampleRate() const noexcept;
        [[nodiscard]] std::uint16_t getByteRate() const noexcept;
        [[nodiscard]] std::uint16_t getBlockAlign() const noexcept;

        void setBitsPerSample(const std::uint16_t& bitsPerSample) noexcept;
        [[nodiscard]] std::uint16_t getBitsPerSample() const noexcept;

        [[nodiscard]] AudioTrackType getAudioTrackType() const noexcept;

        [[nodiscard]] ALuint getALHandler() const noexcept;

        [[nodiscard]] bool isALHandlerValid() const noexcept;

        [[nodiscard]] std::string getSummary() const noexcept;
    
    protected:
        void doLoad(const std::string& path) override;
        
    private:
        AudioTrackType m_audioTrackType = AudioTrackType::UNKNOWN;

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

        std::uint32_t m_frequency = 60;

        ALuint m_ALHandler = -1;
        bool m_isALHandlerValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOTRACKASSET_H
