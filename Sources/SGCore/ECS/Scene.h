#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <vector>
#include <list>
#include <memory>
#include <map>
#include <variant>

#include "Layer.h"

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

    class Scene
    {
        friend class ECSWorld;

    public:
        std::list<Ref<Entity>> m_entities;

        std::string name;

        Scene() noexcept;

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

    private:
        static inline Ref<Scene> m_currentScene;

        std::map<std::string, Ref<Layer>> m_layers;
        // first - index
        //std::map<size_t, std::shared_ptr<Layer>> m_layers;
    };
}

#endif //SUNGEARENGINE_SCENE_H