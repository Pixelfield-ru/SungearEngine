//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOSOURCE_H
#define SUNGEARENGINE_AUDIOSOURCE_H

#include <glm/vec3.hpp>
#include "SGCore/Utils/Event.h"
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
    
    enum AudioSourceType
    {
        AST_POSITIONAL,
        AST_AMBIENT
    };
    
    struct AudioSource
    {
        friend struct AudioProcessor;
        
        ~AudioSource();
        
        void create() noexcept;
        void destroy() noexcept;
        
        void attachBuffer(const Ref<AudioBuffer>& buffer) noexcept;
        void detachBuffer() const noexcept;
        
        void setPosition(const glm::vec3& position) noexcept;
        glm::vec3 getPosition() noexcept;
        
        void setVelocity(const glm::vec3& velocity) noexcept;
        glm::vec3 getVelocity() noexcept;
        
        void setDirection(const glm::vec3& direction) noexcept;
        glm::vec3 getDirection() noexcept;
        
        void setGain(const float& gain) noexcept;
        [[nodiscard]] float getGain() const noexcept;
        
        void setPitch(const float& pitch) noexcept;
        [[nodiscard]] float getPitch() const noexcept;
        
        void setRolloffFactor(const float& rolloffFactor) noexcept;
        [[nodiscard]] float getRolloffFactor() const noexcept;
        
        void setState(const AudioSourceState& state) noexcept;
        [[nodiscard]] AudioSourceState getState() const noexcept;
        
        void setType(const AudioSourceType& type) noexcept;
        [[nodiscard]] AudioSourceType getType() const noexcept;
        
        void setIsLooping(bool isLooping) noexcept;
        [[nodiscard]] bool isLooping() const noexcept;
        
        AudioSource& operator=(const AudioSource& other) noexcept;
        AudioSource& operator=(AudioSource&& other) noexcept;
        
        Event<void(AudioSource& source, AudioSourceState lastState, AudioSourceState newState)> onStateChanged;
        
    private:
        AudioSourceState m_lastState = AudioSourceState::SOURCE_STOPPED;
        AudioSourceType m_type = AudioSourceType::AST_POSITIONAL;
        
        bool m_isLooping = false;
        
        Weak<AudioBuffer> m_attachedBuffer;
        
        ALuint m_handler = 0 ;
        bool m_isValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOSOURCE_H
