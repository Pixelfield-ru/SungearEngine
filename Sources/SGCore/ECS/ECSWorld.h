#ifndef SUNGEARENGINE_ECSWORLD_H
#define SUNGEARENGINE_ECSWORLD_H

#include "IComponent.h"
#include "ISystem.h"
#include "Scene.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Patterns/UUID.h"
#include <unordered_map>
#include <vector>
#include <map>
#include <tsl/robin_map.h>
#include <GLFW/glfw3.h>

#define SG_BEGIN_ITERATE_CACHED_ENTITIES(cachedEntities, layerName, entityName)   \
             for(auto& layerName ## Iter : cachedEntities)                        \
             {                                                                    \
                for(auto& entityName ## Iter : layerName ## Iter.second)          \
                {                                                                 \
                    if(!entityName ## Iter.second) continue;                          \
                    auto layerName = layerName ## Iter.second;                    \
                    auto entityName = entityName ## Iter.second;

#define SG_END_ITERATE_CACHED_ENTITIES }}
namespace Core::ECS
{
    struct SystemCachedEntities
    {
        std::unordered_map<std::shared_ptr<Entity>, std::shared_ptr<ComponentsCollection>> m_cachedEntities;
    };

    // TODO: add allocator and free for components that will remove component from cached components if free
    // TODO: when the component is free, the allocator will make a signal to the systems that cache this type of components for the recache
    class ECSWorld
    {
    public:
        static void init() noexcept;

        static void fixedUpdate(const std::shared_ptr<Scene>& scene);

        static void update(const std::shared_ptr<Scene>& scene);

        static void recacheEntity(const std::shared_ptr<Entity>& entity);

        static std::set<std::shared_ptr<ISystem>>& getSystems() noexcept;

    private:
        static inline std::set<std::shared_ptr<ISystem>> m_systems;
    };
}

#endif //SUNGEARENGINE_ECSWORLD_H
