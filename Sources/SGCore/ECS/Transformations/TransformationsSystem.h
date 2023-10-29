//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "SGCore/ECS/ISystem.h"

namespace Core::ImportedScene
{
    class IMesh;
}

namespace Core::ECS
{
    class TransformComponent;

    class TransformationsSystem : public ISystem
    {
        SG_DECLARE_SINGLETON(TransformationsSystem)

    private:
        void updateMeshUniforms(const std::shared_ptr<ImportedScene::IMesh>& mesh, const std::shared_ptr<TransformComponent>& transformComponent) const noexcept;

    public:
        void fixedUpdate(const std::shared_ptr<Scene>& scene) final;

        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
