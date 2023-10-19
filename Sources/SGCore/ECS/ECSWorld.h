#ifndef SUNGEARENGINE_ECSWORLD_H
#define SUNGEARENGINE_ECSWORLD_H

#include "IComponent.h"
#include "ISystem.h"
#include "Scene.h"
#include "SGCore/Utils/Utils.h"
#include "SGCore/Patterns/UUID.h"
#include <unordered_map>
#include <vector>
#include <robin_map/include/tsl/robin_map.h>

namespace Core::ECS
{
    // TODO: add allocator and free for components that will remove component from cached components if free
    // TODO: when the component is free, the allocator will make a signal to the systems that cache this type of components for the recache
    class ECSWorld
    {
    public:
        static void init() noexcept;

        static void FPSNotRelativeFixedUpdate(const std::shared_ptr<Scene>& scene);

        static void FPSRelativeFixedUpdate(const std::shared_ptr<Scene>& scene);

        template<typename SystemT>
        requires(std::is_base_of_v<ISystem, SystemT>)
        static std::shared_ptr<SystemT> createSystem()
        {
            std::shared_ptr<SystemT> newSystem = std::make_shared<SystemT>();
            newSystem->addFlag(SystemsFlags::SGSF_PER_ENTITY);

            m_systems.push_back(newSystem);

            return newSystem;
        }

    private:
        static inline std::list<std::shared_ptr<ISystem>> m_systems;
    };
}

#endif //SUNGEARENGINE_ECSWORLD_H
