#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <list>
#include <memory>
#include <map>

#include "Layer.h"

namespace Core::ECS
{
    class Entity;

    class Scene
    {
        friend class ECSWorld;

    public:
        std::string name;

        Scene() noexcept;

        void setLayerName(const std::string& oldLayerName, std::string&& newLayerName) noexcept;
        void addLayer(std::string&& layerName) noexcept;
        void addEntity(const std::shared_ptr<Entity>& entity) noexcept;
        void addEntity(const std::shared_ptr<Entity>& entity, const std::string& layerName) noexcept;
        void addEntity(const std::shared_ptr<Entity>& entity, const std::shared_ptr<Layer>& layer) noexcept;

        static std::shared_ptr<Scene> getCurrentScene() noexcept;
        static void setCurrentScene(const std::shared_ptr<Scene>& newCurrentScene) noexcept;

        void setShadowsCastersNum(const int&);
        int getShadowsCastersNum() const noexcept;

        void setDirectionalLightsNum(const int&);
        int getDirectionalLightsNum() const noexcept;

        [[nodiscard]] const auto& getLayers() const noexcept
        {
            return m_layers;
        }

    private:
        static inline std::shared_ptr<Scene> m_currentScene;

        std::unordered_map<std::string, std::shared_ptr<Layer>> m_layers;

        size_t m_shadowsCastersNum = 0;
        size_t m_directionalLightsNum = 0;
    };
}

#endif //SUNGEARENGINE_SCENE_H