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
    auto lockedScene = getScene();
    if(!lockedScene) return;
    
    auto renderingBasesView = lockedScene->getECSRegistry()->view<RenderingBase, Transform>();

    renderingBasesView.each([](RenderingBase::reg_t& renderingBase, Transform::reg_t& transform) {
        TransformBase& finalTransform = transform->m_finalTransform;
        TransformBase& ownTransform = transform->m_ownTransform;

        bool viewMatrixChanged = ownTransform.m_rotationChanged ||
            ownTransform.m_positionChanged ||
            ownTransform.m_scaleChanged;
        ownTransform.m_rotation = glm::angleAxis(
            glm::radians(ownTransform.m_yawPitchRoll.x),
            MathUtils::right3
        );
        ownTransform.m_rotation *= glm::angleAxis(
            glm::radians(ownTransform.m_yawPitchRoll.y),
            MathUtils::up3
        );
        ownTransform.m_rotation *= glm::angleAxis(
            glm::radians(ownTransform.m_yawPitchRoll.z),
            MathUtils::forward3
        );

        renderingBase->m_projectionSpaceMatrixChanged = false;

        bool projectionMatrixChanged = false;
        {
            // RTS
            // #TODO: fix incorrect position. This -ownTransform.m_position may cause incorrect behaviour in TU, PW2d and C3DU transforms
            renderingBase->m_viewMatrix = glm::toMat4(ownTransform.m_rotation);

            renderingBase->m_viewMatrix = glm::translate(renderingBase->m_viewMatrix,
                -ownTransform.m_position
            );

            renderingBase->m_viewMatrix =
                glm::scale(renderingBase->m_viewMatrix, transform->m_ownTransform.m_scale);
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
