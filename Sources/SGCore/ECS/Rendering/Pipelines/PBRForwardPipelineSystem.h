#pragma once

#ifndef SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
#define SUNGEARENGINE_CAMERARENDERINGSYSTEM_H

#include "SGCore/ECS/ISystem.h"
#include "IPipelineSystem.h"

namespace Core::ImportedScene
{
    class IMesh;
}

namespace Core::ECS
{
    class PBRForwardPipelineSystem : public IPipelineSystem
    {
        SG_DECLARE_COPY_MOVE_SINGLETON(PBRForwardPipelineSystem)

    protected:
        PBRForwardPipelineSystem();

    private:
        void updateUniforms(const std::shared_ptr<Graphics::IShader>& shader,
                            const std::shared_ptr<Memory::Assets::IMaterial>& material,
                            const std::shared_ptr<TransformComponent>& transformComponent) const noexcept;

    public:
        void update(const std::shared_ptr<Scene>& scene) final;

        void cacheEntity(const std::shared_ptr<Core::ECS::Entity>& entity) final;
    };
}

#endif //SUNGEARENGINE_CAMERARENDERINGSYSTEM_H
