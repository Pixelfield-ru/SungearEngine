#ifndef SUNGEARENGINE_IPRIMITIVECOMPONENT_H
#define SUNGEARENGINE_IPRIMITIVECOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/ImportedScenesArch/IMeshData.h"
#include "GizmosMeshesRebuilder.h"
#include "SGCore/ECS/Rendering/Mesh.h"

#include <glm/vec3.hpp>

namespace SGCore
{
    struct IGizmo : public Mesh
    {
        IGizmo() noexcept;

        friend class GizmosMeshesRebuilder;

        // will primitive component follow entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        glm::bvec3 m_followEntityTRS { true };

        glm::vec4 m_color { 1.0, 0.0, 0.0, 1.0 };

        glm::vec3 m_offset { 0.0 };

        glm::mat4 m_modelMatrix { 1.0 };

        float m_linesWidth = 3.0f;

        void registerRenderPipelineIfNotRegistered(const Ref<IRenderPipeline>& pipeline) noexcept override;

    protected:
        glm::bvec3 m_lastFollowEntityTRS = glm::vec3 { false, false, false };

        glm::vec4 m_lastColor { 0.0 };

        glm::vec3 m_lastOffset { 0.0 };
    };
}

#endif //SUNGEARENGINE_IPRIMITIVECOMPONENT_H
