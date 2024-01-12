//
// Created by stuka on 22.10.2023.
//

#include "MeshesCollector.h"
#include "SGCore/ECS/ECSUtils.h"
#include "Mesh.h"
#include "Skybox.h"
#include "SGCore/ECS/Rendering/Gizmos/IGizmo.h"

SGCore::MeshesCollector::MeshesCollector()
{
    m_componentsCollector.configureCachingFunction<Mesh, Transform>();
}
