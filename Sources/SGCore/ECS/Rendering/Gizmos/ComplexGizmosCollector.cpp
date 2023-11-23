//
// Created by Ilya on 06.11.2023.
//

#include "ComplexGizmosCollector.h"
#include "IComplexGizmo.h"
#include "SGCore/ECS/Transformations/Transform.h"

SGCore::ComplexGizmosCollector::ComplexGizmosCollector()
{
    m_componentsCollector.configureCachingFunction<IComplexGizmo, Transform>();
}
