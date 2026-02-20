//
// Created by ilya on 15.04.24.
//

#include "AudioProcessor.h"

#include "SGCore/Scene/Scene.h"
#include "AudioSource.h"
#include "SGCore/Transformations/Transform.h"
#include "SGCore/Utils/Defer.h"

void SGCore::AudioProcessor::update(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    auto registry = lockedScene->getECSRegistry();
    
    auto audioSourcesView = registry->view<AudioSource>();
    
    audioSourcesView.each([lockedScene](ECS::entity_t audioEntity, AudioSource::reg_t& audioSource) {
        sg_defer [&audioSource]() {
            audioSource.checkIsStateChanged();
        };

        auto* audioEntityTransform = lockedScene->getECSRegistry()->tryGet<Transform>(audioEntity);
        if(!audioEntityTransform) return;

        audioSource.setPosition((*audioEntityTransform)->m_finalTransform.m_position);

        switch(audioSource.m_directionType)
        {
            case AudioSourceDirectionType::SG_NOT_DIRECTIONAL:
            {
                audioSource.setDirection({ 0.0f, 0.0f, 0.0f });
                break;
            }
            case AudioSourceDirectionType::SG_FORWARD_DIRECTION:
            {
                audioSource.setDirection((*audioEntityTransform)->m_finalTransform.m_forward);
                break;
            }
            case AudioSourceDirectionType::SG_UP_DIRECTION:
            {
                audioSource.setDirection((*audioEntityTransform)->m_finalTransform.m_up);
                break;
            }
            case AudioSourceDirectionType::SG_RIGHT_DIRECTION:
            {
                audioSource.setDirection((*audioEntityTransform)->m_finalTransform.m_right);
                break;
            }
            case AudioSourceDirectionType::SG_BACKWARD_DIRECTION:
            {
                audioSource.setDirection(-(*audioEntityTransform)->m_finalTransform.m_forward);
                break;
            }
            case AudioSourceDirectionType::SG_DOWN_DIRECTION:
            {
                audioSource.setDirection(-(*audioEntityTransform)->m_finalTransform.m_up);
                break;
            }
            case AudioSourceDirectionType::SG_LEFT_DIRECTION:
            {
                audioSource.setDirection(-(*audioEntityTransform)->m_finalTransform.m_right);
                break;
            }
        }
    });
}
