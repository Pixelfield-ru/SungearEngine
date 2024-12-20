//
// Created by stuka on 19.12.2024.
//
#include "Pickable.h"

bool SGCore::Pickable::isPickableForCamera(const entity_t& cameraEntity) const noexcept
{
    for(const auto& cameraEntityRef : m_pickableForCameras)
    {
        if(*cameraEntityRef.m_referencedEntity == cameraEntity) return true;
    }

    return false;
}

