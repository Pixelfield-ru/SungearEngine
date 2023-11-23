//
// Created by Ilya on 06.11.2023.
//

#include "LinesGizmosCollector.h"
#include "LineGizmo.h"
#include "SGCore/ECS/Transformations/Transform.h"

SGCore::LinesGizmosCollector::LinesGizmosCollector()
{
    m_componentsCollector.configureCachingFunction<LineGizmo, Transform>();
}
