//
// Created by stuka on 03.08.2023.
//

#include "SkyboxesCollector.h"

#include "Skybox.h"
#include "Mesh.h"
#include "SGCore/ECS/Transformations/Transform.h"

SGCore::SkyboxesCollector::SkyboxesCollector()
{
    m_componentsCollector.configureCachingFunction<Skybox, Mesh, Transform>();
}
