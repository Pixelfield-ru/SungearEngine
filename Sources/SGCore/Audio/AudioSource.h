//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOSOURCE_H
#define SUNGEARENGINE_AUDIOSOURCE_H

#include <glm/vec3.hpp>
#include "SGCore/Utils/Signal.h"
#include "AudioUtils.h"

#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Serde/Defines.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Memory/AssetRef.h"

sg_predeclare_serde()

namespace SGCore
{
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

    enum class AudioSourceDirectionType
    {
        NOT_DIRECTIONAL,
        FORWARD_DIRECTION,
        UP_DIRECTION,
        RIGHT_DIRECTION,
        BACKWARD_DIRECTION,
        DOWN_DIRECTION,
        LEFT_DIRECTION
    };

    struct AudioSource : ECS::Component<AudioSource, const AudioSource>
    {
        sg_serde_as_friend()

        friend struct AudioProcessor;

        AudioSourceDirectionType m_directionType = AudioSourceDirectionType::NOT_DIRECTIONAL;

        AudioSource() = default;
        AudioSource(const AudioSource&) = default;
        AudioSource(AudioSource&&) noexcept = default;

        ~AudioSource();
        
        void create() noexcept;
        void destroy() noexcept;
        
        void attachAudioTrack(const AssetRef<AudioTrackAsset>& audioTrackAsset) noexcept;
        void detachAudioTrack() const noexcept;

        void setPosition(const glm::vec3& position) noexcept;
        glm::vec3 getPosition() const noexcept;

        void setVelocity(const glm::vec3& velocity) noexcept;
        glm::vec3 getVelocity() const noexcept;

        void setDirection(const glm::vec3& direction) noexcept;
        glm::vec3 getDirection() const noexcept;

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
        
        Signal<void(AudioSource& source, AudioSourceState lastState, AudioSourceState newState)> onStateChanged;
        
    private:
        AudioSourceState m_lastState = AudioSourceState::STOPPED;
        AudioSourceType m_type = AudioSourceType::POSITIONAL;

        bool m_isLooping = false;

        AssetRef<AudioTrackAsset> m_attachedAudioTrack;
        
        ALuint m_handler = -1;
        bool m_isValid = false;
    };
}

#endif //SUNGEARENGINE_AUDIOSOURCE_H
