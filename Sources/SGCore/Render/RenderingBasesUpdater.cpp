//
// Created by stuka on 03.02.2024.
//

#include <glm/detail/type_quat.hpp>

#include "RenderingBasesUpdater.h"
#include "SGCore/Transformations/Transform.h"
#include "RenderingBase.h"
#include "SGCore/Scene/Scene.h"
#include "SGCore/Main/CoreMain.h"
#include "glm/ext/quaternion_trigonometric.hpp"
#include "glm/gtx/quaternion.hpp"

void SGCore::RenderingBasesUpdater::fixedUpdate(const double& dt, const double& fixedDt)
{
    auto lockedScene = m_scene.lock();
    if(!lockedScene) return;
    
    auto renderingBasesView = lockedScene->getECSRegistry()->view<Ref<RenderingBase>, Ref<Transform>>();

    renderingBasesView.each([](Ref<RenderingBase>& renderingBase, Ref<Transform>& transform) {
        TransformBase& ownTransform = transform->m_ownTransform;

        bool viewMatrixChanged = ownTransform.m_rotationChanged ||
                                 ownTransform.m_positionChanged ||
                                 ownTransform.m_scaleChanged;

        glm::quat rotationQuat;
        // if(viewMatrixChanged)
        {
            rotationQuat = glm::angleAxis(
                    glm::radians(ownTransform.m_rotation.x),
                    glm::vec3(1, 0, 0)
            );
            rotationQuat *= glm::angleAxis(
                    glm::radians(ownTransform.m_rotation.y),
                    glm::vec3(0, 1, 0)
            );
            rotationQuat *= glm::angleAxis(
                    glm::radians(ownTransform.m_rotation.z),
                    glm::vec3(0, 0, 1)
            );
        }

        renderingBase->m_projectionSpaceMatrixChanged = false;

        bool projectionMatrixChanged = false;

        // TODO: make checking for lastTransformation != current transformation
        // if(viewMatrixChanged)
        {
            renderingBase->m_viewMatrix = glm::toMat4(rotationQuat);
            renderingBase->m_viewMatrix = glm::translate(renderingBase->m_viewMatrix,
                                                        -ownTransform.m_position
            );
            renderingBase->m_viewMatrix =
                    glm::scale(renderingBase->m_viewMatrix, ownTransform.m_scale);
        }

        // if some part of projection matrix of camera is changed
        if(renderingBase->m_lastFov != renderingBase->m_fov ||
           renderingBase->m_lastAspect != renderingBase->m_aspect ||
           renderingBase->m_lastZNear != renderingBase->m_zNear ||
           renderingBase->m_lastZFar != renderingBase->m_zFar)
        {
            renderingBase->m_projectionMatrix = glm::perspective<float>(
                    glm::radians(renderingBase->m_fov),
                    renderingBase->m_aspect,
                    renderingBase->m_zNear,
                    renderingBase->m_zFar
            );

            renderingBase->m_lastFov = renderingBase->m_fov;
            renderingBase->m_lastAspect = renderingBase->m_aspect;
            renderingBase->m_lastZNear = renderingBase->m_zNear;
            renderingBase->m_lastZFar = renderingBase->m_zFar;

            projectionMatrixChanged = true;
        }

        // if(viewMatrixChanged || projectionMatrixChanged)
        {
            renderingBase->m_projectionSpaceMatrix =
                    renderingBase->m_projectionMatrix * renderingBase->m_viewMatrix;
            renderingBase->m_projectionSpaceMatrixChanged = true;
        }
        
        renderingBase->m_frustum = { renderingBase->m_projectionSpaceMatrix };
        
        renderingBase->m_orthographicMatrix = glm::ortho<float>(renderingBase->m_left, renderingBase->m_right,
                                                                renderingBase->m_bottom, renderingBase->m_top);
        
        {
            renderingBase->m_orthographicSpaceMatrix =
                    renderingBase->m_orthographicMatrix * renderingBase->m_viewMatrix;
            renderingBase->m_projectionSpaceMatrixChanged = true;
        }
    });
}
