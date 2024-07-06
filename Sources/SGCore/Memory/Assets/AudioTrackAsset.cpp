//
// Created by ilya on 13.04.24.
//

#include <spdlog/spdlog.h>

#include "AudioTrackAsset.h"

void SGCore::AudioTrackAsset::doLoad(const std::string& path)
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
        spdlog::error("Audio file with extension '{0}' is not supported. Path to audio file: '{1}'", extension, path);
        return;
    }
    
    ByteFileAsset::doLoad(path);
    
    m_audioTrack.loadFromMemory(m_buffer, m_bufferSize, trackType);
}

SGCore::AudioTrack& SGCore::AudioTrackAsset::getAudioTrack() noexcept
{
    return m_audioTrack;
}
