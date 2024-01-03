    //
// Created by stuka on 01.08.2023.
//

#include "PBRFRPDirectionalLightsCollector.h"
#include "PBRFRPDirectionalLight.h"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"
#include "GLFW/glfw3.h"
#include "SGCore/ECS/Transformations/TransformationsUpdater.h"

SGCore::PBRFRPDirectionalLightsCollector::PBRFRPDirectionalLightsCollector()
{
    m_componentsCollector.configureCachingFunction<PBRFRPDirectionalLight, Transform>();
}