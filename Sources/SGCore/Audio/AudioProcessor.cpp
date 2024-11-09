//
// Created by ilya on 15.04.24.
//

#include "AudioProcessor.h"

#include "SGCore/Scene/Scene.h"
#include "AudioSource.h"

void SGCore::AudioProcessor::update(const double& dt, const double& fixedDt)
{
    auto lockedScene = getScene();

    if(!lockedScene) return;
    
    auto registry = lockedScene->getECSRegistry();
    
    auto audioSourcesView = registry->view<AudioSource>();
    
    audioSourcesView.each([](AudioSource& audioSource) {
        AudioSourceState currentState = audioSource.getState();
        if(currentState != audioSource.m_lastState)
        {
            audioSource.onStateChanged(audioSource, audioSource.m_lastState, currentState);
            audioSource.m_lastState = currentState;
        }
    });
}
