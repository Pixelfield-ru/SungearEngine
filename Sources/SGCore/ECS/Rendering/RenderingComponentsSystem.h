//
// Created by stuka on 27.07.2023.
//

#ifndef SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
#define SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ECS
{
    class RenderingComponentsSystem : public ISystem
    {
    public:
        void update(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity) final;

        void deltaUpdate(const std::shared_ptr<Scene>& scene, const std::shared_ptr<Core::ECS::Entity>& entity,
                         const double& deltaTime) final;
    };
}

#endif //SUNGEARENGINE_RENDERINGCOMPONENTSSYSTEM_H
