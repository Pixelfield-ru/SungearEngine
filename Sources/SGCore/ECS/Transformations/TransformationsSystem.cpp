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
#include "SGCore/ECS/ECSWorld.h"
#include "SGCore/ECS/Rendering/MeshedEntitiesCollectorSystem.h"
#include "SGCore/ECS/Rendering/Primitives/IPrimitiveComponent.h"

/*double curT = 0.0;
double accum = 0.0;
size_t fps = 0;*/

void Core::ECS::TransformationsSystem::fixedUpdate
(const std::shared_ptr<Scene>& scene)
{
    /*double lastT = curT;
    curT = glfwGetTime();

    accum += curT - lastT;
    if(accum >= 1.0)
    {
        accum = 0.0;
        std::cout << "fps: " << std::to_string(fps) << std::endl;
        fps = 0;
    }
    fps++;*/

    double t0 = glfwGetTime();

    for(const auto& cachedEntities : m_cachedEntities)
    {
        if(!cachedEntities.second) continue;

        auto transformComponent = cachedEntities.second->getComponent<TransformComponent>();

        if(!transformComponent) continue;

        transformComponent->m_positionChanged = false;
        transformComponent->m_rotationChanged = false;
        transformComponent->m_scaleChanged = false;

        // checking position for changes. if changed then update
        if(transformComponent->m_lastPosition != transformComponent->m_position)
        {
            transformComponent->m_translationMatrix = glm::translate(transformComponent->m_translationMatrix, transformComponent->m_position - transformComponent->m_lastPosition);
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
            transformComponent->m_scaleMatrix = glm::scale(transformComponent->m_scaleMatrix, transformComponent->m_scale - transformComponent->m_lastScale);
            transformComponent->m_lastScale = transformComponent->m_scale;

            transformComponent->m_scaleChanged = true;
        }

        // if any matrix is changed then update model matrix
        if(transformComponent->m_positionChanged || transformComponent->m_rotationChanged || transformComponent->m_scaleChanged)
        {
            transformComponent->m_modelMatrix = transformComponent->m_translationMatrix * transformComponent->m_rotationMatrix * transformComponent->m_scaleMatrix;

            /*for(const auto& meshComponent : meshComponents)
            {
                updateMeshUniforms(meshComponent->m_mesh, transformComponent);
            }

            for(const auto& primitiveComponent : primitiveComponents)
            {
                updateMeshUniforms(primitiveComponent->m_mesh, transformComponent);
            }*/
        }
    }

    double t1 = glfwGetTime();

    // std::cout << "ms: " << std::to_string((t1 - t0) * 1000.0) << std::endl;
}

void Core::ECS::TransformationsSystem::updateMeshUniforms(const std::shared_ptr<ImportedScene::IMesh>& mesh,
                                                          const std::shared_ptr<TransformComponent>& transformComponent) const noexcept
{
    for(const auto& shader : mesh->m_material->getShaders())
    {
        shader.second->bind();

        shader.second->useMatrix("objectModelMatrix",
                                 transformComponent->m_modelMatrix
        );
        shader.second->useVectorf("objectPosition",
                                  transformComponent->m_position
        );
        shader.second->useVectorf("objectRotation",
                                  transformComponent->m_rotation
        );
        shader.second->useVectorf("objectScale",
                                  transformComponent->m_scale
        );

        // TODO: MOVE MATERIAL UPDATE TO OTHER SYSTEM
        shader.second->useVectorf("materialDiffuseCol",
                                  mesh->m_material->m_diffuseColor
        );
        shader.second->useVectorf("materialSpecularCol",
                                  mesh->m_material->m_specularColor
        );
        shader.second->useVectorf("materialAmbientCol",
                                  mesh->m_material->m_ambientColor
        );
        shader.second->useVectorf("materialEmissionCol",
                                  mesh->m_material->m_emissionColor
        );
        shader.second->useVectorf("materialTransparentCol",
                                  mesh->m_material->m_transparentColor
        );
        shader.second->useFloat("materialShininess",
                                mesh->m_material->m_shininess
        );
        shader.second->useFloat("materialMetallicFactor",
                                mesh->m_material->m_metallicFactor
        );
        shader.second->useFloat("materialRoughnessFactor",
                                mesh->m_material->m_roughnessFactor
        );
    }
}

void Core::ECS::TransformationsSystem::cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity)
{
    cacheEntityComponents<TransformComponent>(entity);
}
