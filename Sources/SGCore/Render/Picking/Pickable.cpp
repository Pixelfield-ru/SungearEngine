//
// Created by stuka on 19.12.2024.
//
#include "Pickable.h"

bool SGCore::Pickable::isPickableForCamera(const ECS::entity_t& cameraEntity) const noexcept
{
    const size_t camerasSize = m_pickableForCameras.size();
    for(size_t i = 0; i < camerasSize; ++i)
    {
        const auto& camE = m_pickableForCameras[i];

        if(*camE.m_referencedEntity == cameraEntity) return true;
    }

    return false;
}

