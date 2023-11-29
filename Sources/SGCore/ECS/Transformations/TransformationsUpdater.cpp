//
// Created by stuka on 02.05.2023.
//

#include "TransformationsUpdater.h"
#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/detail/type_quat.hpp"
#include "glm/gtx/quaternion.hpp"
#include "SGCore/ECS/Rendering/Mesh.h"
#include "GLFW/glfw3.h"
#include "SGCore/ECS/ECSUtils.h"
#include "SGCore/ECS/Rendering/MeshesCollector.h"

/*double curT = 0.0;
double accum = 0.0;
size_t fps = 0;*/

SGCore::TransformationsUpdater::TransformationsUpdater()
{
    m_componentsCollector.configureCachingFunction<Transform>();
}

void SGCore::TransformationsUpdater::fixedUpdate
(const Ref<Scene>& scene)
{
    SG_BEGIN_ITERATE_CACHED_ENTITIES(*m_componentsCollector.m_cachedEntities, layer, cachedEntity)
        auto transformComponent = cachedEntity.getComponent<Transform>();

        if(!transformComponent) continue;

        transformComponent->m_positionChanged = false;
        transformComponent->m_rotationChanged = false;
        transformComponent->m_scaleChanged = false;

        // checking position for changes. if changed then update
        if(transformComponent->m_lastPosition != transformComponent->m_position)
        {
            transformComponent->m_translationMatrix = glm::translate(transformComponent->m_translationMatrix,
                                                                     transformComponent->m_position -
                                                                     transformComponent->m_lastPosition
            );
            transformComponent->m_lastPosition = transformComponent->m_position;

            transformComponent->m_positionChanged = true;
        }

        // todo: fix rotation. make global (not local) rotations
        // checking rotation for changes. if changed then update
        if(transformComponent->m_lastRotation != transformComponent->m_rotation)
        {
            // todo:fix rotation
            /*glm::quat rotQuat = glm::angleAxis(
                                  -glm::radians(transformComponent->m_rotation.x - transformComponent->m_lastRotation.x),
                                  glm::vec3(1, 0, 0));
            rotQuat *= glm::angleAxis(
                                  -glm::radians(transformComponent->m_rotation.y - transformComponent->m_lastRotation.y),
                                  glm::vec3(0, 1, 0));
            rotQuat *= glm::angleAxis(
                                  -glm::radians(transformComponent->m_rotation.z - transformComponent->m_lastRotation.z),
                                  glm::vec3(0, 0, 1));*/

            const float rotDifX = transformComponent->m_rotation.x - transformComponent->m_lastRotation.x;
            const float rotDifY = transformComponent->m_rotation.y - transformComponent->m_lastRotation.y;
            const float rotDifZ = transformComponent->m_rotation.z - transformComponent->m_lastRotation.z;

            transformComponent->m_rotationMatrix = glm::rotate(transformComponent->m_rotationMatrix,
                                                               -glm::radians(rotDifX),
                                                               glm::vec3(1, 0, 0));

            transformComponent->m_rotationMatrix = glm::rotate(transformComponent->m_rotationMatrix,
                                                               -glm::radians(rotDifY),
                                                               glm::vec3(0, 1, 0));
            transformComponent->m_rotationMatrix = glm::rotate(transformComponent->m_rotationMatrix,
                                                               -glm::radians(rotDifZ),
                                                               glm::vec3(0, 0, 1));

            // rotating directions vectors
            transformComponent->m_left = glm::rotate(transformComponent->m_left,
                                                     -glm::radians(rotDifY), glm::vec3(0, 1, 0));
            transformComponent->m_left = glm::rotate(transformComponent->m_left,
                                                     -glm::radians(rotDifZ), glm::vec3(0, 0, 1));

            transformComponent->m_forward = glm::rotate(transformComponent->m_forward,
                                                        -glm::radians(rotDifX), glm::vec3(1, 0, 0));
            transformComponent->m_forward = glm::rotate(transformComponent->m_forward,
                                                        -glm::radians(rotDifY), glm::vec3(0, 1, 0));

            transformComponent->m_up = glm::rotate(transformComponent->m_up,
                                                   -glm::radians(rotDifX), glm::vec3(1, 0, 0));
            transformComponent->m_up = glm::rotate(transformComponent->m_up,
                                                   -glm::radians(rotDifZ), glm::vec3(0, 0, 1));

            //transformComponent->m_rotationMatrix = glm::toMat4(rotQuat);

            transformComponent->m_lastRotation = transformComponent->m_rotation;

            transformComponent->m_rotationChanged = true;
        }

        // checking scale for changes. if changed then update
        if(transformComponent->m_lastScale != transformComponent->m_scale)
        {
            transformComponent->m_scaleMatrix = glm::scale(transformComponent->m_scaleMatrix,
                                                           transformComponent->m_scale -
                                                           transformComponent->m_lastScale
            );
            transformComponent->m_lastScale = transformComponent->m_scale;

            transformComponent->m_scaleChanged = true;
        }

        // if any matrix is changed then update model matrix
        if(transformComponent->m_positionChanged || transformComponent->m_rotationChanged ||
           transformComponent->m_scaleChanged)
        {
            transformComponent->m_modelMatrix =
                    transformComponent->m_translationMatrix * transformComponent->m_rotationMatrix *
                    transformComponent->m_scaleMatrix;

            /*for(const auto& meshComponent : meshComponents)
            {
                updateMeshUniforms(meshComponent->m_mesh, transformComponent);
            }

            for(const auto& primitiveComponent : primitiveComponents)
            {
                updateMeshUniforms(primitiveComponent->m_mesh, transformComponent);
            }*/
        }
    SG_END_ITERATE_CACHED_ENTITIES

    double t1 = glfwGetTime();
}
