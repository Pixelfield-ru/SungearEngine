//
// Created by ilya on 13.04.24.
//

#include "AudioTrack.h"

#include <memory>
#include <string>
#include <iostream>
#include <spdlog/spdlog.h>

#include "SGCore/Utils/Utils.h"
#include "SGCore/Main/CoreGlobals.h"

#include <stb_vorbis.c>

SGCore::AudioTrack::~AudioTrack()
{
    deleteData();
}

void SGCore::AudioTrack::loadFromMemory(const char* buffer, const size_t& bufferSize,
                                        const SGCore::AudioTrackType& type)
{
    if(!buffer)
    {
        spdlog::error("Could not load audio from memory: Buffer is nullptr.");
        return;
    }
    
    char tmpBuf[4];
    
    if(type == AudioTrackType::WAV)
    {
        constexpr bool isLittleNative = std::endian::native == std::endian::little;
        
        // RIFF ascii letters
        std::memcpy(tmpBuf, buffer, 4);
        if(std::strncmp(tmpBuf, "RIFF", 4) != 0)
        {
            spdlog::error("Could not load WAV audio from memory: ChunkID does not contain 'RIFF' letters.");
            return;
        }
        
        // WAVE ascii letters
        std::memcpy(tmpBuf, buffer + 8, 4);
        if(std::strncmp(tmpBuf, "WAVE", 4) != 0)
        {
            spdlog::error("Could not load WAV audio from memory: Format does not contain 'WAVE' letters.");
            return;
        }
        
        // 'fmt ' ascii letters
        std::memcpy(tmpBuf, buffer + 12, 4);
        if(std::strncmp(tmpBuf, "fmt ", 4) != 0)
        {
            spdlog::error("Could not load WAV audio from memory: Subchunk1ID does not contain 'fmt ' letters.");
            return;
        }
        
        std::uint32_t subchunk1Size = 0;
        
        std::memcpy(tmpBuf, buffer + 16, 4);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 4);
        }
        std::memcpy(&subchunk1Size, tmpBuf, 4);
        std::cout << subchunk1Size << std::endl;
        
        std::memcpy(tmpBuf, buffer + 20, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_audioFormat, tmpBuf, 2);
        
        std::memcpy(tmpBuf, buffer + 22, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_numChannels, tmpBuf, 2);
        
        std::memcpy(tmpBuf, buffer + 24, 4);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 4);
        }
        std::memcpy(&m_sampleRate, tmpBuf, 4);
        
        std::memcpy(tmpBuf, buffer + 28, 4);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 4);
        }
        std::memcpy(&m_byteRate, tmpBuf, 4);
        
        std::memcpy(tmpBuf, buffer + 32, 2);
        if(!isLittleNative)
        {
            SGCore::Utils::swapEndian(tmpBuf, 2);
        }
        std::memcpy(&m_blockAlign, tmpBuf, 2);
        
        std::memcpy(tmpBuf, buffer + 34, 2);
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
            if(offsetToData + 4 >= bufferSize)
            {
                dataFound = false;
                break;
            }
            
            std::memcpy(tmpBuf, buffer + offsetToData, 4);
            if(std::strncmp(tmpBuf, "data", 4) != 0)
            {
                std::uint32_t subchunkSize = 0;
                std::memcpy(&subchunkSize, buffer + offsetToData + 4, 4);
                offsetToData += 4 + 4 + subchunkSize;
            }
        }
        
        if(!dataFound)
        {
            spdlog::error("Could not load WAV audio from memory: Unable to find 'data' chunk.");
            return;
        }
        
        std::memcpy(&m_dataBufferSize, buffer + offsetToData + 4, 4);
        deleteData();
        m_dataBuffer = (char*) std::malloc(m_dataBufferSize);
        std::memcpy(m_dataBuffer, buffer + offsetToData + 8, m_dataBufferSize);
    }
    else if(type == AudioTrackType::OGG)
    {
        int stbErr;
        stb_vorbis* vorbis = stb_vorbis_open_memory((std::uint8_t*) buffer, bufferSize, &stbErr, nullptr);
        
        if(stbErr != STBVorbisError::VORBIS__no_error)
        {
            spdlog::error("Could not load ogg from memory. Error is: {0}", stbErr);
            stb_vorbis_close(vorbis);
            return;
        }
        
        m_bitsPerSample = 16;
        m_sampleRate = vorbis->sample_rate;
        m_numChannels = vorbis->channels;
        
        size_t trackByteSize = stb_vorbis_stream_length_in_samples(vorbis) * sizeof(short);
        
        m_dataBuffer = (char*) std::malloc(trackByteSize);
        m_dataBufferSize = trackByteSize;
        
        int samplesCount = stb_vorbis_decode_memory((std::uint8_t*) buffer, trackByteSize, &m_numChannels, &m_sampleRate, (short**) &m_dataBuffer);
        
        std::cout << "samples count: " << samplesCount << ", stream len: " << vorbis->stream_len << ", trackByteSize: " << trackByteSize << std::endl;
        
        stb_vorbis_close(vorbis);
    }
}

void SGCore::AudioTrack::deleteData() noexcept
{
    delete m_dataBuffer;
    m_dataBuffer = nullptr;
}

std::uint16_t SGCore::AudioTrack::getAudioFormat() const noexcept
{
    return m_audioFormat;
}

std::uint16_t SGCore::AudioTrack::getNumChannels() const noexcept
{
    return m_numChannels;
}

std::uint16_t SGCore::AudioTrack::getSampleRate() const noexcept
{
    return m_sampleRate;
}

std::uint16_t SGCore::AudioTrack::getByteRate() const noexcept
{
    return m_byteRate;
}

std::uint16_t SGCore::AudioTrack::getBlockAlign() const noexcept
{
    return m_blockAlign;
}

std::uint16_t SGCore::AudioTrack::getBitsPerSample() const noexcept
{
    return m_bitsPerSample;
}

char* SGCore::AudioTrack::getDataBuffer() const noexcept
{
    return m_dataBuffer;
}

std::uint32_t SGCore::AudioTrack::getDataBufferSize() const noexcept
{
    return m_dataBufferSize;
}

std::string SGCore::AudioTrack::getSummary() const noexcept
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
