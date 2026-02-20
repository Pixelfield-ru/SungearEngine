//
// Created by ilya on 14.04.24.
//

#ifndef SUNGEARENGINE_AUDIOSOURCE_H
#define SUNGEARENGINE_AUDIOSOURCE_H

#include <glm/vec3.hpp>
#include "AudioUtils.h"

#include "SGCore/Main/CoreGlobals.h"

#include "SGCore/Memory/Assets/AudioTrackAsset.h"
#include "SGCore/Serde/Defines.h"
#include "SGCore/ECS/Component.h"
#include "SGCore/Memory/AssetRef.h"
#include "SGCore/Utils/Playable.h"

sg_predeclare_serde()

namespace SGCore
{
    /**
     * Type of audio source.
     */
    enum class AudioSourceType
    {
        SG_WORLD,
        SG_AMBIENT
    };

    /**
     * Relative to entity`s transform audio source direction.
     */
    enum class AudioSourceDirectionType
    {
        SG_NOT_DIRECTIONAL,
        SG_FORWARD_DIRECTION,
        SG_UP_DIRECTION,
        SG_RIGHT_DIRECTION,
        SG_BACKWARD_DIRECTION,
        SG_DOWN_DIRECTION,
        SG_LEFT_DIRECTION
    };

    /**
     * ECS component to play 3D or ambient audio.\n
     * Uses SGCore::Transform of entity to update OpenAL audio source position and direction.
     */
    struct AudioSource : ECS::Component<AudioSource, const AudioSource>, Playable
    {
        sg_serde_as_friend()

        friend struct AudioProcessor;

        /// Audio source relative direction type.
        AudioSourceDirectionType m_directionType = AudioSourceDirectionType::SG_NOT_DIRECTIONAL;

        AudioSource() = default;
        AudioSource(const AudioSource&) = default;
        AudioSource(AudioSource&&) noexcept = default;

        /**
         * Calls SGCore::AudioSource::destroy() function.
         */
        ~AudioSource() override;

        /**
         * Destroys last audio source handle, creates new audio source handle and attaches last SGCore::AudioTrackAsset to new handle.\n
         * Can be used to recreate audio source.
         */
        void create() noexcept;

        /**
         * Detaches OpenAL audio buffer from source and deletes OpenAL handle.
         */
        void destroy() noexcept;

        /**
         * Attaches audio track that will be played to audio source.
         * @param audioTrackAsset Audio track asset.
         */
        void attachAudioTrack(const AssetRef<AudioTrackAsset>& audioTrackAsset) noexcept;

        /**
         * Detaches audio track from audio source.
         */
        void detachAudioTrack() const noexcept;

        /**
         * Sets 3D world position of audio source.
         * @param position 3D world position.
         */
        void setPosition(const glm::vec3& position) noexcept;

        /**
         * @return 3D world position of audio source.
         */
        glm::vec3 getPosition() const noexcept;

        /**
         * Sets 3D world velocity of audio source.
         * @param velocity 3D world velocity.
         */
        void setVelocity(const glm::vec3& velocity) noexcept;

        /**
         * @return 3D world velocity of audio source.
         */
        glm::vec3 getVelocity() const noexcept;

        /**
         * Sets 3D world direction of audio source.
         * @param direction 3D world direction.
         */
        void setDirection(const glm::vec3& direction) noexcept;

        /**
         * @return 3D world direction of audio source.
         */
        glm::vec3 getDirection() const noexcept;

        void setGain(const float& gain) noexcept;
        [[nodiscard]] float getGain() const noexcept;

        void setPitch(const float& pitch) noexcept;
        [[nodiscard]] float getPitch() const noexcept;

        void setRolloffFactor(const float& rolloffFactor) noexcept;
        [[nodiscard]] float getRolloffFactor() const noexcept;

        /**
         * @param time Current audio time.
         */
        void setCurrentTime(float time) const noexcept;

        /**
         * @return Current audio time.
         */
        float getCurrentTime() const noexcept;

        /**
         * @return Current audio source state.
         */
        [[nodiscard]] PlayableState getState() const noexcept override;

        /**
         * @param type Positional type of audio source.
         */
        void setType(const AudioSourceType& type) noexcept;

        /**
         * @return Positional type of audio source.
         */
        [[nodiscard]] AudioSourceType getType() const noexcept;

        /**
         * Sets whether audio source is looped.
         * @param isLooping Is audio source looped.
         */
        void setIsLooping(bool isLooping) noexcept;

        /**
         * @return Whether audio source is looped.
         */
        [[nodiscard]] bool isLooping() const noexcept;

        /**
         * Copies parameters from other audio source only if current audio source is valid.
         * @param other Audio source to copy from.
         * @return This.
         */
        AudioSource& operator=(const AudioSource& other) noexcept;

        /**
         * Moves parameters from other audio source only if current audio source is valid.\n
         * Function only detaches audio track asset from other audio source and does not destroy other audio source.
         * @param other Audio source to move from.
         * @return This.
         */
        AudioSource& operator=(AudioSource&& other) noexcept;
        
    private:
        AudioSourceType m_type = AudioSourceType::SG_WORLD;

        bool m_isLooping = false;

        AssetRef<AudioTrackAsset> m_attachedAudioTrack;
        
        ALuint m_handle = -1;
        bool m_isValid = false;

        /**
         * @param state SGCore::AudioSourceState of audio source.
         */
        void doSetState(PlayableState state) noexcept override;

        void resetTimelineTime() noexcept final;
    };
}

#endif //SUNGEARENGINE_AUDIOSOURCE_H
