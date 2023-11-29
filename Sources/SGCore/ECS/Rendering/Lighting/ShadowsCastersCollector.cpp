//
// Created by stuka on 27.07.2023.
//

#include "ShadowsCastersCollector.h"
#include "ShadowsCaster.h"
#include "SGCore/ECS/Transformations/Transform.h"
#include "SGCore/ECS/Rendering/Mesh.h"

SGCore::ShadowsCastersCollector::ShadowsCastersCollector()
{
    m_componentsCollector.configureCachingFunction<ShadowsCaster, Transform>();
}
