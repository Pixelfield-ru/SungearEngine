//
// Created by ilya on 13.04.24.
//

#include <spdlog/spdlog.h>

#include "AudioTrackAsset.h"

void SGCore::AudioTrackAsset::load(const std::string& path)
{
    m_path = path;
    
    AudioTrackType trackType;
    std::string extension = m_path.extension().string();
    if(extension == ".wav")
    {
        trackType = AudioTrackType::ATT_WAV;
    }
    else if(extension == ".ogg")
    {
        trackType = AudioTrackType::ATT_OGG;
    }
    else if(extension == ".mp3")
    {
        trackType = AudioTrackType::ATT_MP3;
    }
    else
    {
        spdlog::error("Audio file with extension '{0}' is not supported. Path to audio file: '{1}'", extension, path);
        return;
    }
    
    ByteFileAsset::load(path);
    
    m_audioTrack.loadFromMemory(m_buffer, m_bufferSize, trackType);
}

SGCore::AudioTrack& SGCore::AudioTrackAsset::getAudioTrack() noexcept
{
    return m_audioTrack;
}
