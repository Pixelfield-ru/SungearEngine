#pragma once

#ifndef SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
#define SUNGEARENGINE_CAMERARENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ImportedScene
{
    class IMesh;
}

namespace Core::ECS
{
    class CameraRenderingSystem : public ISystem
    {
        SG_DECLARE_SINGLETON(CameraRenderingSystem)

    private:
        void updateMeshUniforms(const std::shared_ptr<ImportedScene::IMesh>& mesh, const std::shared_ptr<TransformComponent>& transformComponent) const noexcept;

    public:
        void update(const std::shared_ptr<Scene>& scene) final;

        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) const final;
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
