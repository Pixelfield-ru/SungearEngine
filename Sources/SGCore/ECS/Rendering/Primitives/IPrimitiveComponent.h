#ifndef SUNGEARENGINE_IPRIMITIVECOMPONENT_H
#define SUNGEARENGINE_IPRIMITIVECOMPONENT_H

#include "SGCore/ECS/IComponent.h"
#include "SGCore/ImportedScenesArch/IMesh.h"
#include "PrimitivesUpdaterSystem.h"

#include <glm/vec3.hpp>

namespace Core::ECS
{
    class IPrimitiveComponent : public IComponent
    {
        friend class PrimitivesUpdaterSystem;

    public:
        // will primitive component follow entity`s translation, rotation and scale
        // x - follow translation
        // y - follow rotation
        // z - follow scale
        glm::bvec3 m_followEntityTRS = glm::vec3 { true, true, true };

        glm::vec4 m_color { 1.0, 0.0, 0.0, 1.0 };

        glm::vec3 m_offset { 0.0, 0.0, 0.0 };

        glm::mat4 m_modelMatrix { 1.0 };

        virtual void setVertexPosition(const size_t& vertexIdx, const float& x, const float& y, const float& z) = 0;

        float m_linesWidth = 3.0f;

        std::shared_ptr<ImportedScene::IMesh> m_mesh;

    protected:
        glm::bvec3 m_lastFollowEntityTRS = glm::vec3 { false, false, false };

        glm::vec4 m_lastColor;

        glm::vec3 m_lastOffset;

        void init() noexcept final { }
    };
}

#endif //SUNGEARENGINE_IPRIMITIVECOMPONENT_H
