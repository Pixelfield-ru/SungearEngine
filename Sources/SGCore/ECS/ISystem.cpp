//
// Created by stuka on 30.10.2023.
//

#include "ISystem.h"
#include "SGCore/ECS/Rendering/Pipelines/PBRForwardPipelineSystem.h"

bool Core::ECS::ISystem::in()
{
    return SG_INSTANCEOF(this, PBRForwardPipelineSystem);
}