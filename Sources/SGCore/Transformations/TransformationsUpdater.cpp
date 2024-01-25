//
// Created by stuka on 02.05.2023.
//

#include "TransformationsUpdater.h"
#include "Transform.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "SGCore/Scene/Scene.h"
#include "GLFW/glfw3.h"

/*double curT = 0.0;
double accum = 0.0;
size_t fps = 0;*/

void SGCore::TransformationsUpdater::fixedUpdate
(const Ref<Scene>& scene)
{
    m_transforms.each([](Transform* transform) {
        transform->m_positionChanged = false;
        transform->m_rotationChanged = false;
        transform->m_scaleChanged = false;

        // checking position for changes. if changed then update
        if(transform->m_lastPosition != transform->m_position)
        {
            transform->m_translationMatrix = glm::translate(transform->m_translationMatrix,
                                                            transform->m_position -
                                                            transform->m_lastPosition
            );
            transform->m_lastPosition = transform->m_position;

            transform->m_positionChanged = true;
        }

        // todo: fix rotation. make global (not local) rotations
        // checking rotation for changes. if changed then update
        if(transform->m_lastRotation != transform->m_rotation)
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

            const float rotDifX = transform->m_rotation.x - transform->m_lastRotation.x;
            const float rotDifY = transform->m_rotation.y - transform->m_lastRotation.y;
            const float rotDifZ = transform->m_rotation.z - transform->m_lastRotation.z;

            transform->m_rotationMatrix = glm::rotate(transform->m_rotationMatrix,
                                                      -glm::radians(rotDifX),
                                                      glm::vec3(1, 0, 0));

            transform->m_rotationMatrix = glm::rotate(transform->m_rotationMatrix,
                                                      -glm::radians(rotDifY),
                                                      glm::vec3(0, 1, 0));
            transform->m_rotationMatrix = glm::rotate(transform->m_rotationMatrix,
                                                      -glm::radians(rotDifZ),
                                                      glm::vec3(0, 0, 1));

            // rotating directions vectors
            transform->m_left = glm::rotate(transform->m_left,
                                            -glm::radians(rotDifY), glm::vec3(0, 1, 0));
            transform->m_left = glm::rotate(transform->m_left,
                                            -glm::radians(rotDifZ), glm::vec3(0, 0, 1));

            transform->m_forward = glm::rotate(transform->m_forward,
                                               -glm::radians(rotDifX), glm::vec3(1, 0, 0));
            transform->m_forward = glm::rotate(transform->m_forward,
                                               -glm::radians(rotDifY), glm::vec3(0, 1, 0));

            transform->m_up = glm::rotate(transform->m_up,
                                          -glm::radians(rotDifX), glm::vec3(1, 0, 0));
            transform->m_up = glm::rotate(transform->m_up,
                                          -glm::radians(rotDifZ), glm::vec3(0, 0, 1));

            //transformComponent->m_rotationMatrix = glm::toMat4(rotQuat);

            transform->m_lastRotation = transform->m_rotation;

            transform->m_rotationChanged = true;
        }

        // checking scale for changes. if changed then update
        if(transform->m_lastScale != transform->m_scale)
        {
            transform->m_scaleMatrix = glm::scale(transform->m_scaleMatrix,
                                                  transform->m_scale -
                                                  transform->m_lastScale
            );
            transform->m_lastScale = transform->m_scale;

            transform->m_scaleChanged = true;
        }

        // if any matrix is changed then update model matrix
        if(transform->m_positionChanged || transform->m_rotationChanged ||
           transform->m_scaleChanged)
        {
            transform->m_modelMatrix =
                    transform->m_translationMatrix * transform->m_rotationMatrix *
                    transform->m_scaleMatrix;

            /*for(const auto& meshComponent : meshComponents)
            {
                updateMeshUniforms(meshComponent->m_mesh, transformComponent);
            }

            for(const auto& primitiveComponent : primitiveComponents)
            {
                updateMeshUniforms(primitiveComponent->m_mesh, transformComponent);
            }*/
        }
    });

    double t1 = glfwGetTime();
}

void SGCore::TransformationsUpdater::attachToScene(const SGCore::Ref<SGCore::Scene>& scene) noexcept
{
    m_transforms = scene->getECSRegistry()->view<Transform>();
}
