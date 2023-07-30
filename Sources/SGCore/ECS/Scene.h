#pragma once

#ifndef SUNGEARENGINE_SCENE_H
#define SUNGEARENGINE_SCENE_H

#include <list>
#include <memory>

#include "Entity.h"

namespace Core::ECS
{
    class Scene
    {
    private:
        static inline std::shared_ptr<Scene> m_currentScene;

        int m_shadowsCastersNum = 0;

    public:
        std::string name;

        // TODO: JUST FOR TEST. MUST MAKE LAYERING
        std::list<std::shared_ptr<Entity>> m_entities;

        static std::shared_ptr<Scene> getCurrentScene() noexcept;
        static void setCurrentScene(const std::shared_ptr<Scene>& newCurrentScene) noexcept;

        void setShadowsCastersNum(const int&);
        int getShadowsCastersNum() const;
    };
}

#endif //SUNGEARENGINE_SCENE_H
