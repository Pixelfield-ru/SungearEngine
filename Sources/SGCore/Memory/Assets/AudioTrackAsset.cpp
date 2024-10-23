//
// Created by ilya on 13.04.24.
//

#include <spdlog/spdlog.h>

#include "AudioTrackAsset.h"
#include "SGCore/Audio/AudioUtils.h"
#include "SGCore/Serde/Serde.h"
#include <stb_vorbis.c>

SGCore::AudioTrackAsset::~AudioTrackAsset()
{
    deleteData();
    destroyALBuffer();
}

void SGCore::AudioTrackAsset::destroyALBuffer() noexcept
{
    if(m_isALHandlerValid)
    {
        AL_CALL(alDeleteBuffers, 1, &m_ALHandler);

        m_isALHandlerValid = false;
    }
}

void SGCore::AudioTrackAsset::createALBuffer() noexcept
{
    destroyALBuffer();

    AL_CALL_E(m_isALHandlerValid, alGenBuffers, 1, &m_ALHandler);
}

void SGCore::AudioTrackAsset::putData(const uint16_t& numChannels, const uint16_t& bitsPerSample, const void* data,
                                      const size_t& dataSize, const uint16_t& sampleRate) noexcept
{
    deleteData();
    m_dataBuffer = static_cast<char*>(std::malloc(dataSize));
    std::memcpy(m_dataBuffer, data, dataSize);
    m_dataBufferSize = dataSize;

    if(m_isALHandlerValid)
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

        AL_CALL(alBufferData, m_ALHandler, alFormat, m_dataBuffer, dataSize, sampleRate);
    }

    setFrequency(m_frequency);
    setBitsPerSample(bitsPerSample);
    setNumChannels(numChannels);
}

void SGCore::AudioTrackAsset::doLoad(const std::filesystem::path& path)
{
    AudioTrackType trackType;
    std::string extension = m_path.extension().string();
    if(extension == ".wav")
    {
        trackType = AudioTrackType::WAV;
    }
    else if(extension == ".ogg")
    {
        trackType = AudioTrackType::OGG;
    }
    else if(extension == ".mp3")
    {
        trackType = AudioTrackType::MP3;
    }
    else
    {
        LOG_E(SGCORE_TAG, "Audio file with extension '{}' is not supported. Path to audio file: '{}'", extension, Utils::toUTF8(path.u16string()));
        return;
    }
    
    ByteFileAsset::doLoad(path);

    if(!m_dataBuffer)
    {
        LOG_E(SGCORE_TAG, "Could not load audio from memory: Buffer is nullptr.");
        return;
    }

    m_audioTrackType = trackType;

    char tmpBuf[4];

    if(trackType == AudioTrackType::WAV)
    {
        constexpr bool isLittleNative = std::endian::native == std::endian::little;

        // RIFF ascii letters
        std::memcpy(tmpBuf, m_dataBuffer, 4);
        if(std::strncmp(tmpBuf, "RIFF", 4) != 0)
        {
            LOG_E(SGCORE_TAG, "Could not load WAV audio from memory: ChunkID does not contain 'RIFF' letters.");
            return;
        }

        // WAVE ascii letters
        std::memcpy(tmpBuf, m_dataBuffer + 8, 4);
        if(std::strncmp(tmpBuf, "WAVE", 4) != 0)
        {
            LOG_E(SGCORE_TAG, "Could not load WAV audio from memory: Format does not contain 'WAVE' letters.");
            return;
        }

        // 'fmt ' ascii letters
        std::memcpy(tmpBuf, m_dataBuffer + 12, 4);
        if(std::strncmp(tmpBuf, "fmt ", 4) != 0)
        {
            LOG_E(SGCORE_TAG, "Could not load WAV audio from memory: Subchunk1ID does not contain 'fmt ' letters.");
            return;
        }

        std::uint32_t subchunk1Size = 0;

        std::memcpy(tmpBuf, m_dataBuffer + 16, 4);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 4);
        }
        std::memcpy(&subchunk1Size, tmpBuf, 4);
        std::cout << subchunk1Size << std::endl;

        std::memcpy(tmpBuf, m_dataBuffer + 20, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_audioFormat, tmpBuf, 2);

        std::memcpy(tmpBuf, m_dataBuffer + 22, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_numChannels, tmpBuf, 2);

        std::memcpy(tmpBuf, m_dataBuffer + 24, 4);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 4);
        }
        std::memcpy(&m_sampleRate, tmpBuf, 4);

        std::memcpy(tmpBuf, m_dataBuffer + 28, 4);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 4);
        }
        std::memcpy(&m_byteRate, tmpBuf, 4);

        std::memcpy(tmpBuf, m_dataBuffer + 32, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_blockAlign, tmpBuf, 2);

        std::memcpy(tmpBuf, m_dataBuffer + 34, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_bitsPerSample, tmpBuf, 2);

        std::uint32_t offsetToData = 20 + subchunk1Size;

        bool dataFound = true;

        // finding 'data' block
        while(std::strncmp(tmpBuf, "data", 4) != 0)
        {
            if(offsetToData + 4 >= m_dataBufferSize)
            {
                dataFound = false;
                break;
            }

            std::memcpy(tmpBuf, m_dataBuffer + offsetToData, 4);
            if(std::strncmp(tmpBuf, "data", 4) != 0)
            {
                std::uint32_t subchunkSize = 0;
                std::memcpy(&subchunkSize, m_dataBuffer + offsetToData + 4, 4);
                offsetToData += 4 + 4 + subchunkSize;
            }
        }

        if(!dataFound)
        {
            LOG_E(SGCORE_TAG, "Could not load WAV audio from memory: Unable to find 'data' chunk.");
            return;
        }

        std::memcpy(&m_dataBufferSize, m_dataBuffer + offsetToData + 4, 4);
        deleteData();
        m_dataBuffer = (char*) std::malloc(m_dataBufferSize);
        std::memcpy(m_dataBuffer, m_dataBuffer + offsetToData + 8, m_dataBufferSize);
    }
    else if(trackType == AudioTrackType::OGG)
    {
        int stbErr;
        stb_vorbis* vorbis = stb_vorbis_open_memory((std::uint8_t*) m_dataBuffer, m_dataBufferSize, &stbErr, nullptr);

        if(stbErr != STBVorbisError::VORBIS__no_error)
        {
            LOG_E(SGCORE_TAG, "Could not load ogg from memory. Error is: {0}", stbErr);
            stb_vorbis_close(vorbis);
            return;
        }

        m_bitsPerSample = 16;
        m_sampleRate = vorbis->sample_rate;
        m_numChannels = vorbis->channels;

        size_t trackByteSize = stb_vorbis_stream_length_in_samples(vorbis) * sizeof(short);

        m_dataBuffer = (char*) std::malloc(trackByteSize);
        m_dataBufferSize = trackByteSize;

        int samplesCount = stb_vorbis_decode_memory((std::uint8_t*) m_dataBuffer, trackByteSize, &m_numChannels, &m_sampleRate, (short**) &m_dataBuffer);

        LOG_I(SGCORE_TAG, "Loaded audio: samples count: {}, stream length: {}, track byte size: {}", samplesCount, vorbis->stream_len, trackByteSize);

        stb_vorbis_close(vorbis);
    }
}

void SGCore::AudioTrackAsset::deleteData() noexcept
{
    delete[] m_dataBuffer;
    m_dataBuffer = nullptr;
}

void SGCore::AudioTrackAsset::setFrequency(const std::uint32_t& frequency) noexcept
{
    m_frequency = frequency;
    if(m_isALHandlerValid)
    {
        AL_CALL(alBufferf, m_ALHandler, AL_FREQUENCY, frequency);
    }
}

std::uint32_t SGCore::AudioTrackAsset::getFrequency() const noexcept
{
    return m_frequency;
}

std::uint16_t SGCore::AudioTrackAsset::getAudioFormat() const noexcept
{
    return m_audioFormat;
}

void SGCore::AudioTrackAsset::setNumChannels(const uint16_t& numChannels) noexcept
{
    m_numChannels = numChannels;
    if(m_isALHandlerValid)
    {
        AL_CALL(alBufferi, m_ALHandler, AL_CHANNELS, numChannels);
    }
}

std::uint16_t SGCore::AudioTrackAsset::getNumChannels() const noexcept
{
    return m_numChannels;
}

std::uint16_t SGCore::AudioTrackAsset::getSampleRate() const noexcept
{
    return m_sampleRate;
}

std::uint16_t SGCore::AudioTrackAsset::getByteRate() const noexcept
{
    return m_byteRate;
}

std::uint16_t SGCore::AudioTrackAsset::getBlockAlign() const noexcept
{
    return m_blockAlign;
}

void SGCore::AudioTrackAsset::setBitsPerSample(const uint16_t& bitsPerSample) noexcept
{
    m_bitsPerSample = bitsPerSample;
    if(m_isALHandlerValid)
    {
        AL_CALL(alBufferi, m_ALHandler, AL_BITS, bitsPerSample);
    }
}

std::uint16_t SGCore::AudioTrackAsset::getBitsPerSample() const noexcept
{
    return m_bitsPerSample;
}

SGCore::AudioTrackType SGCore::AudioTrackAsset::getAudioTrackType() const noexcept
{
    return m_audioTrackType;
}

ALuint SGCore::AudioTrackAsset::getALHandler() const noexcept
{
    return m_ALHandler;
}

bool SGCore::AudioTrackAsset::isALHandlerValid() const noexcept
{
    return m_isALHandlerValid;
}

std::string SGCore::AudioTrackAsset::getSummary() const noexcept
{
    std::string sum = "Audio summary.\n";
    sum += "\tFormat: " + std::to_string(m_audioFormat) + "\n";
    sum += "\tNum channels: " + std::to_string(m_numChannels) + "\n";
    sum += "\tSample rate: " + std::to_string(m_sampleRate) + "\n";
    sum += "\tByte rate: " + std::to_string(m_byteRate) + "\n";
    sum += "\tBlock align: " + std::to_string(m_blockAlign) + "\n";
    sum += "\tBits per sample: " + std::to_string(m_bitsPerSample) + "\n";
    sum += "\tData buffer size: " + std::to_string(m_dataBufferSize);

    return sum;
}

void SGCore::AudioTrackAsset::serializeToPackage(SGCore::AssetsPackage::AssetSection& currentAssetSection,
                                                 bool isDataSerializing)
{
    currentAssetSection.addStandardInfo(this);

    if(isDataSerializing)
    {
        currentAssetSection.addSection("m_audioTrackType", m_audioTrackType);
        currentAssetSection.addSection("m_audioFormat", m_audioFormat);
        currentAssetSection.addSection("m_numChannels", m_numChannels);
        currentAssetSection.addSection("m_sampleRate", m_sampleRate);
        currentAssetSection.addSection("m_byteRate", m_byteRate);
        currentAssetSection.addSection("m_blockAlign", m_blockAlign);
        currentAssetSection.addSection("m_bitsPerSample", m_bitsPerSample);
        currentAssetSection.addSection("m_frequency", m_frequency);
        if(m_dataBuffer)
        {
            currentAssetSection.addSection("m_dataBufferSize", m_dataBufferSize);
            currentAssetSection.addSection("m_dataBuffer", m_dataBuffer);
        }
    }
}
