#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <vector>
#include <list>
#include <memory>
#include <map>
#include <variant>
#include <type_traits>
#include <entt/entt.hpp>

#include "Layer.h"
#include "ISystem.h"
#include "SGUtils/Utils.h"
#include "SGUtils/UniqueNamesManager.h"
#include "SGUtils/Event.h"
#include "SGUtils/EventListener.h"
#include "SGUtils/TypeInfo.h"

namespace SGCore
{
    // todo: make scenes list
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

        // size_t createBaseEntity() noexcept;

        auto& getECSRegistry() noexcept
        {
            return m_ecsRegistry;
        }

        template<typename FlagType>
        void addFlagObserverSystem(const Ref<ISystem>& system)
        {
            m_flagsObserverSystems[SGUtils::TypeID<FlagType>::id()].push_back(system);
        }

        template<typename FlagType>
        void invokeFlagObserverSystems(const entt::entity& flagOwner)
        {
            constexpr size_t flagTypeID = SGUtils::TypeID<FlagType>::id();

            auto& systemsVec = m_flagsObserverSystems[flagTypeID];

            for(auto& system : systemsVec)
            {
                system->onFlagChanged(shared_from_this(), flagOwner, flagTypeID);
            }
        }

        Layer createLayer(const std::string& name) noexcept;

    private:
        entt::registry m_ecsRegistry;

        static inline Ref<Scene> m_currentScene;

        Ref<UniqueNamesManager> m_uniqueNamesManager = MakeRef<UniqueNamesManager>();

        std::set<Ref<ISystem>> m_systems;
        std::map<size_t, std::vector<Ref<ISystem>>> m_flagsObserverSystems;
        std::unordered_map<std::string, Layer> m_layers;

        size_t m_maxLayersCount = 0;
    };
}

#endif //SUNGEARENGINE_SCENE_H