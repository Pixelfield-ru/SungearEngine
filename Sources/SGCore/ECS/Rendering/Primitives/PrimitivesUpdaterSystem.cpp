#include "PrimitivesUpdaterSystem.h"
#include "IPrimitiveComponent.h"
#include "LineComponent.h"

void Core::ECS::PrimitivesUpdaterSystem::update(const std::shared_ptr<Scene>& scene,
                                                const std::shared_ptr<Core::ECS::Entity>& entity)
{
    std::list<std::shared_ptr<IPrimitiveComponent>> primitiveComponents = entity->getComponents<IPrimitiveComponent>();

    for(const auto& primitiveComponent : primitiveComponents)
    {
        primitiveComponent->m_mesh->m_material->getShader()->bind();

        if(primitiveComponent->m_offset != primitiveComponent->m_lastOffset)
        {
            primitiveComponent->m_mesh->m_material->getShader()->useVectorf(
                    "offset",
                    primitiveComponent->m_offset
                    );

            primitiveComponent->m_lastOffset = primitiveComponent->m_offset;
        }

        if(primitiveComponent->m_color != primitiveComponent->m_lastColor)
        {
            primitiveComponent->m_mesh->m_material->getShader()->useVectorf(
                    "color",
                    primitiveComponent->m_color
            );

            primitiveComponent->m_lastColor = primitiveComponent->m_color;
        }

        // todo: make individual update for box, sphere and other primitives
    }

}

void Core::ECS::PrimitivesUpdaterSystem::deltaUpdate(const std::shared_ptr<Scene>& scene,
                                                     const std::shared_ptr<Core::ECS::Entity>& entity,
                                                     const double& deltaTime)
{

}
