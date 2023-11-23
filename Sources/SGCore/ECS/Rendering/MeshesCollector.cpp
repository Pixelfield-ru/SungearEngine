//
// Created by stuka on 22.10.2023.
//

#include "MeshesCollector.h"
#include "SGCore/ECS/ECSWorld.h"
#include "Mesh.h"
#include "Skybox.h"
#include "SGCore/ECS/Rendering/Gizmos/IGizmo.h"

SGCore::MeshesCollector::MeshesCollector()
{
    m_componentsCollector.configureCachingFunction<Mesh, Transform>([](const Ref<Entity>& entity)
                                                                    {
                                                                        if(entity->getComponent<Skybox>())
                                                                        {
                                                                            return false;
                                                                        }

                                                                        return true;
                                                                    }, [](const Ref<Entity>& entity,
                                                                          const Ref<IComponent>& component)
                                                                    {
                                                                        // we wont cache mesh component if it is gizmo, because gizmo renderings in different way
                                                                        return !SG_INSTANCEOF(component.get(), IGizmo);
                                                                    }
    );
}
