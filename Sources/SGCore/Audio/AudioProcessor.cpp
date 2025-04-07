//
// Created by ilya on 15.04.24.
//

#include "AudioProcessor.h"

#include "SGCore/Scene/Scene.h"
#include "AudioSource.h"
#include "SGCore/Transformations/Transform.h"

void SGCore::AudioProcessor::update(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    auto registry = lockedScene->getECSRegistry();
    
    auto audioSourcesView = registry->view<AudioSource>();
    
    audioSourcesView.each([lockedScene](ECS::entity_t audioEntity, AudioSource::reg_t& audioSource) {
        auto* audioEntityTransform = lockedScene->getECSRegistry()->tryGet<Transform>(audioEntity);
        if(audioEntityTransform)
        {
            audioSource.setPosition((*audioEntityTransform)->m_finalTransform.m_position);

            switch(audioSource.m_directionType)
            {
                case AudioSourceDirectionType::NOT_DIRECTIONAL:
                {
                    audioSource.setDirection({ 0.0f, 0.0f, 0.0f });
                    break;
                }
                case AudioSourceDirectionType::FORWARD_DIRECTION:
                {
                    audioSource.setDirection((*audioEntityTransform)->m_finalTransform.m_forward);
                    break;
                }
                case AudioSourceDirectionType::UP_DIRECTION:
                {
                    audioSource.setDirection((*audioEntityTransform)->m_finalTransform.m_up);
                    break;
                }
                case AudioSourceDirectionType::RIGHT_DIRECTION:
                {
                    audioSource.setDirection((*audioEntityTransform)->m_finalTransform.m_right);
                    break;
                }
                case AudioSourceDirectionType::BACKWARD_DIRECTION:
                {
                    audioSource.setDirection(-(*audioEntityTransform)->m_finalTransform.m_forward);
                    break;
                }
                case AudioSourceDirectionType::DOWN_DIRECTION:
                {
                    audioSource.setDirection(-(*audioEntityTransform)->m_finalTransform.m_up);
                    break;
                }
                case AudioSourceDirectionType::LEFT_DIRECTION:
                {
                    audioSource.setDirection(-(*audioEntityTransform)->m_finalTransform.m_right);
                    break;
                }
            }
        }

        AudioSourceState currentState = audioSource.getState();
        if(currentState != audioSource.m_lastState)
        {
            audioSource.onStateChanged(audioSource, audioSource.m_lastState, currentState);
            audioSource.m_lastState = currentState;
        }
    });
}
