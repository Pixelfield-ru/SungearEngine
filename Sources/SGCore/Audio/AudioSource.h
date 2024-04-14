//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOSOURCE_H
#define SUNGEARENGINE_AUDIOSOURCE_H

#include "AudioUtils.h"

#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct AudioBuffer;
    
    enum AudioSourceState
    {
        SOURCE_PLAYING,
        SOURCE_PAUSED,
        SOURCE_STOPPED
    };
    
    struct AudioSource
    {
        ~AudioSource();
        
        void create() noexcept;
        void destroy() noexcept;
        
        void attachBuffer(const Ref<AudioBuffer>& buffer) noexcept;
        void detachBuffer() const noexcept;
        
        void setState(const AudioSourceState& state) noexcept;
        [[nodiscard]] AudioSourceState getState() const noexcept;
        
        void setIsLooping(bool isLooping) noexcept;
        [[nodiscard]] bool isLooping() const noexcept;
        
        AudioSource& operator=(const AudioSource& other) noexcept;
        AudioSource& operator=(AudioSource&& other) noexcept;
        
    private:
        AudioSourceState m_state = AudioSourceState::SOURCE_STOPPED;
        bool m_isLooping = false;
        
        Weak<AudioBuffer> m_attachedBuffer;
        
        ALuint m_handler = 0;
        bool m_isValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOSOURCE_H
