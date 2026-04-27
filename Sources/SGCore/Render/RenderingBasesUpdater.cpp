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

void SGCore::RenderingBasesUpdater::update(double dt, double fixedDt)
{
    auto lockedScene = getScene();
    if(!lockedScene) return;
    
    auto renderingBasesView = lockedScene->getECSRegistry()->view<EntityBaseInfo, RenderingBase, Transform>();

    renderingBasesView.each([lockedScene](EntityBaseInfo& cameraInfo, RenderingBase& renderingBase, Transform& transform) {
        TransformBase& worldTransform = transform.m_worldTransform;
        TransformBase& localTransform = transform.m_localTransform;

        bool viewMatrixChanged = localTransform.m_rotationChanged ||
            localTransform.m_positionChanged ||
            localTransform.m_scaleChanged;

        /*localTransform.m_rotation = glm::quat(glm::vec3(glm::radians(localTransform.m_yawPitchRoll.x),
                                                      glm::radians(localTransform.m_yawPitchRoll.y),
                                                      glm::radians(localTransform.m_yawPitchRoll.z)));*/

        renderingBase.m_projectionSpaceMatrixChanged = false;

        bool projectionMatrixChanged = false;

        renderingBase.m_viewMatrix = glm::inverse(worldTransform.m_animatedModelMatrix);

        // if some part of projection matrix of camera is changed
        if(renderingBase.m_lastFov != renderingBase.m_fov ||
           renderingBase.m_lastAspect != renderingBase.m_aspect ||
           renderingBase.m_lastZNear != renderingBase.m_zNear ||
           renderingBase.m_lastZFar != renderingBase.m_zFar)
        {
            renderingBase.m_projectionMatrix = glm::perspective<float>(
                    glm::radians(renderingBase.m_fov),
                    renderingBase.m_aspect,
                    renderingBase.m_zNear,
                    renderingBase.m_zFar
            );

            renderingBase.m_lastFov = renderingBase.m_fov;
            renderingBase.m_lastAspect = renderingBase.m_aspect;
            renderingBase.m_lastZNear = renderingBase.m_zNear;
            renderingBase.m_lastZFar = renderingBase.m_zFar;

            projectionMatrixChanged = true;
        }

        // if(viewMatrixChanged || projectionMatrixChanged)
        {
            renderingBase.m_projectionSpaceMatrix =
                    renderingBase.m_projectionMatrix * renderingBase.m_viewMatrix;
            renderingBase.m_projectionSpaceMatrixChanged = true;
        }
        
        renderingBase.m_frustum = { renderingBase.m_projectionSpaceMatrix };

        renderingBase.m_orthographicMatrix = glm::ortho<float>(renderingBase.m_left, renderingBase.m_right,
                                                                renderingBase.m_bottom, renderingBase.m_top,
                                                                renderingBase.m_zNear, renderingBase.m_zFar);
        
        {
            renderingBase.m_orthographicSpaceMatrix =
                    renderingBase.m_orthographicMatrix * renderingBase.m_viewMatrix;
            renderingBase.m_projectionSpaceMatrixChanged = true;
        }
    });
}
