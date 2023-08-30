#ifndef SUNGEARENGINE_ECSWORLD_H
#define SUNGEARENGINE_ECSWORLD_H

#include "IComponent.h"
#include "ISystem.h"
#include "Scene.h"

namespace Core::ECS
{
    class ECSWorld
    {
    private:
        static inline std::list<std::shared_ptr<ISystem>> m_systems;

    public:
        static void init() noexcept;

        static void update(const std::shared_ptr<Scene>& scene);
        static void deltaUpdate(const std::shared_ptr<Scene>& scene, const double& deltaTime);

        template<typename SystemT>
        requires(std::is_base_of_v<ISystem, SystemT>)
        static std::shared_ptr<SystemT> createSystem()
        {
            std::shared_ptr<SystemT> newSystem = std::make_shared<SystemT>();
            newSystem->addFlag(SystemsFlags::SGSF_PER_ENTITY);

            m_systems.push_back(newSystem);

            return newSystem;
        }
    };
}

#endif //SUNGEARENGINE_ECSWORLD_H
