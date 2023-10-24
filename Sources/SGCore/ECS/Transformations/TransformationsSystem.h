//
// Created by stuka on 02.05.2023.
//

#pragma once

#ifndef NATIVECORE_TRANSFORMATIONSSYSTEM_H
#define NATIVECORE_TRANSFORMATIONSSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "SGCore/ECS/Transformations/TransformComponent.h"
#include "SGCore/ECS/Rendering/MeshComponent.h"

namespace Core::ECS
{
    class TransformationsSystem : public ISystem
    {
    private:
        void updateMeshUniforms(const std::shared_ptr<ImportedScene::IMesh>& mesh, const std::shared_ptr<TransformComponent>& transformComponent) const noexcept;

    public:
        void fixedUpdate(const std::shared_ptr<Scene>& scene) final;
    };
}

#endif //NATIVECORE_TRANSFORMATIONSSYSTEM_H
