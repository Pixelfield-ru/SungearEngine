//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOSOURCE_H
#define SUNGEARENGINE_AUDIOSOURCE_H

#include <glm/vec3.hpp>
#include "SGCore/Utils/Event.h"
#include "AudioUtils.h"

#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Serde/Serde.h"
#include "SGCore/Memory/Assets/AudioTrackAsset.h"

namespace SGCore
{
    struct AudioBuffer;
    
    enum class AudioSourceState
    {
        PLAYING,
        PAUSED,
        STOPPED
    };
    
    enum class AudioSourceType
    {
        POSITIONAL,
        AMBIENT
    };

    sg_struct(type = "component")
    struct AudioSource
    {
        sg_serdespec_as_friend()

        friend struct AudioProcessor;
        
        ~AudioSource();
        
        void create() noexcept;
        void destroy() noexcept;
        
        void attachAudioTrack(const Ref<AudioTrackAsset>& audioTrackAsset) noexcept;
        void detachAudioTrack() const noexcept;

        sg_function(name = "setPosition", setterFor = "m_position")
        void setPosition(const glm::vec3& position) noexcept;
        sg_function(name = "getPosition", getterFor = "m_position")
        glm::vec3 getPosition() const noexcept;

        sg_function(name = "setVelocity", setterFor = "m_velocity")
        void setVelocity(const glm::vec3& velocity) noexcept;
        sg_function(name = "getVelocity", getterFor = "m_velocity")
        glm::vec3 getVelocity() const noexcept;

        sg_function(name = "setDirection", setterFor = "m_direction")
        void setDirection(const glm::vec3& direction) noexcept;
        sg_function(name = "getDirection", getterFor = "m_direction")
        glm::vec3 getDirection() const noexcept;

        sg_function(name = "setPitch", setterFor = "m_gain")
        void setGain(const float& gain) noexcept;
        sg_function(name = "getGain", getterFor = "m_gain")
        [[nodiscard]] float getGain() const noexcept;

        sg_function(name = "setPitch", setterFor = "m_pitch")
        void setPitch(const float& pitch) noexcept;
        sg_function(name = "getPitch", getterFor = "m_pitch")
        [[nodiscard]] float getPitch() const noexcept;

        sg_function(name = "setRolloffFactor", setterFor = "m_rolloffFactor")
        void setRolloffFactor(const float& rolloffFactor) noexcept;
        sg_function(name = "getRolloffFactor", getterFor = "m_rolloffFactor")
        [[nodiscard]] float getRolloffFactor() const noexcept;

        sg_function(name = "setState", setterFor = "m_lastState")
        void setState(const AudioSourceState& state) noexcept;
        sg_function(name = "getState", getterFor = "m_lastState")
        [[nodiscard]] AudioSourceState getState() const noexcept;

        sg_function(name = "setType", setterFor = "m_type")
        void setType(const AudioSourceType& type) noexcept;
        sg_function(name = "getType", getterFor = "m_type")
        [[nodiscard]] AudioSourceType getType() const noexcept;

        sg_function(name = "setIsLooping", setterFor = "m_isLooping")
        void setIsLooping(bool isLooping) noexcept;
        sg_function(name = "isLooping", getterFor = "m_isLooping")
        [[nodiscard]] bool isLooping() const noexcept;
        
        AudioSource& operator=(const AudioSource& other) noexcept;
        AudioSource& operator=(AudioSource&& other) noexcept;
        
        Event<void(AudioSource& source, AudioSourceState lastState, AudioSourceState newState)> onStateChanged;
        
    private:
        sg_member()
        AudioSourceState m_lastState = AudioSourceState::STOPPED;
        sg_member()
        AudioSourceType m_type = AudioSourceType::POSITIONAL;

        sg_member(varName = "m_rolloffFactor", serializableName = "m_rolloffFactor")
        sg_member(varName = "m_pitch", serializableName = "m_pitch")
        sg_member(varName = "m_gain", serializableName = "m_gain")
        sg_member(varName = "m_direction", serializableName = "m_direction")
        sg_member(varName = "m_velocity", serializableName = "m_velocity")
        sg_member(varName = "m_position", serializableName = "m_position")

        sg_member()
        bool m_isLooping = false;

        sg_member()
        Ref<AudioTrackAsset> m_attachedAudioTrack;
        
        ALuint m_handler = 0;
        bool m_isValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOSOURCE_H
