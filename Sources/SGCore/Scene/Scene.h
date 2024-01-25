#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <vector>
#include <list>
#include <memory>
#include <map>
#include <variant>
#include <type_traits>

#include "Layer.h"
#include "ISystem.h"
#include "SGUtils/Utils.h"
#include "SGUtils/UniqueNamesManager.h"
#include "SGUtils/Event.h"
#include "SGUtils/EventListenerHolder.h"
#include "SGECS/Registry.h"

namespace SGCore
{
    class Entity;

    class Scene : public std::enable_shared_from_this<Scene>
    {
    public:
        Scene();

        std::string m_name;

        void createDefaultSystems();

        static Ref<Scene> getCurrentScene() noexcept;
        static void setCurrentScene(const Ref<Scene>& newCurrentScene) noexcept;

        void fixedUpdate();

        void update();

        template<typename SystemT>
        requires(std::is_base_of_v<ISystem, SystemT>)
        Ref<SystemT> getSystem()
        {
            for(auto& system : m_systems)
            {
                if(SG_INSTANCEOF(system.get(), SystemT))
                {
                    return std::static_pointer_cast<SystemT>(system);
                }
            }

            return nullptr;
        }

        std::set<Ref<ISystem>>& getSystems() noexcept;

        auto getUniqueNamesManager() const noexcept
        {
            return m_uniqueNamesManager;
        }

        size_t createBaseEntity() noexcept;

        auto& getECSRegistry() noexcept
        {
            return m_ecsRegistry;
        }

        Layer createLayer(const std::string& name) noexcept;

    private:
        Scope<SGECS::Registry> m_ecsRegistry = MakeScope<SGECS::Registry>();

        static inline Ref<Scene> m_currentScene;

        Ref<SGUtils::UniqueNamesManager> m_uniqueNamesManager = MakeRef<SGUtils::UniqueNamesManager>();

        std::set<Ref<ISystem>> m_systems;
        std::unordered_map<std::string, Layer> m_layers;

        size_t m_maxLayersCount = 0;
    };
}

#endif //SUNGEARENGINE_SCENE_H