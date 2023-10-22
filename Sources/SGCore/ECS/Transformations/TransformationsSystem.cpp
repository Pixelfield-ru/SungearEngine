//
// Created by stuka on 02.05.2023.
//

#include "TransformationsSystem.h"
#include "TransformComponent.h"
#include "glm/ext/matrix_transform.hpp"
#include "glm/ext/quaternion_transform.hpp"
#include "glm/gtx/rotate_vector.hpp"
#include "glm/ext/quaternion_common.hpp"
#include "glm/detail/type_quat.hpp"
#include "glm/gtx/quaternion.hpp"
#include "SGCore/ECS/Rendering/MeshComponent.h"
#include "GLFW/glfw3.h"

void Core::ECS::TransformationsSystem::FPSNotRelativeFixedUpdate
(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity)
{
    double t0 = glfwGetTime();

    std::shared_ptr<TransformComponent> transformComponent = entity->getComponent<TransformComponent>();
    if(!transformComponent) return;

    transformComponent->m_translationChanged = false;
    transformComponent->m_rotationChanged = false;
    transformComponent->m_scaleChanged = false;

    // checking position for changes. if changed then update
    if(transformComponent->m_lastPosition != transformComponent->m_position)
    {
        transformComponent->m_translationMatrix = glm::translate(transformComponent->m_translationMatrix, transformComponent->m_position - transformComponent->m_lastPosition);
        transformComponent->m_lastPosition = transformComponent->m_position;

        transformComponent->m_translationChanged = true;
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
        transformComponent->m_scaleMatrix = glm::scale(transformComponent->m_scaleMatrix, transformComponent->m_scale - transformComponent->m_lastScale);
        transformComponent->m_lastScale = transformComponent->m_scale;

        transformComponent->m_scaleChanged = true;
    }

    // if any matrix is changed then update model matrix
    if(transformComponent->m_translationChanged || transformComponent->m_rotationChanged || transformComponent->m_scaleChanged)
    {
        transformComponent->m_modelMatrix = transformComponent->m_translationMatrix * transformComponent->m_rotationMatrix * transformComponent->m_scaleMatrix;
    }

    double t1 = glfwGetTime();

    //std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}
