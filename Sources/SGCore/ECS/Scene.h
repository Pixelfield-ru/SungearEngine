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
#include "SGCore/Utils/Utils.h"

namespace SGCore
{
    class Entity;

    struct LayerKey
    {
        std::string m_name;
        size_t m_index = 0;
    };

    struct LayerKeyComparator
    {
        bool operator()(const LayerKey& lk0, const LayerKey& lk1) const
        {
            return lk0.m_index < lk1.m_index;
        }
    };

    class Scene : public std::enable_shared_from_this<Scene>
    {
    public:
        std::list<Ref<Entity>> m_entities;

        std::string name;

        Scene() noexcept;

        void createDefaultSystems();

        void addLayer(std::string&& layerName) noexcept;
        void addEntity(const Ref<Entity>& entity) noexcept;
        void addEntity(const Ref<Entity>& entity, const std::string& layerName) noexcept;
        void addEntity(const Ref<Entity>& entity, const Ref<Layer>& layer) noexcept;

        static Ref<Scene> getCurrentScene() noexcept;
        static void setCurrentScene(const Ref<Scene>& newCurrentScene) noexcept;

        Ref<Layer> getLayer(const size_t& layerIndex) noexcept;

        [[nodiscard]] const auto& getLayers() const noexcept
        {
            return m_layers;
        }

        void fixedUpdate();

        void update();

        void recacheEntity(const Ref<Entity>& entity);

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

        size_t getCountOfEntities(const std::string& entitiesNames) const noexcept;

    private:
        static inline Ref<Scene> m_currentScene;

        std::set<Ref<ISystem>> m_systems;

        std::map<std::string, Ref<Layer>> m_layers;
    };
}

#endif //SUNGEARENGINE_SCENE_H