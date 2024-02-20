//
// Created by stuka on 02.05.2023.
//

#pragma once

#include <entt/entity/observer.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>
#include "SGCore/Scene/ISystem.h"
#include "SGUtils/Utils.h"

namespace SGCore
{
    class IMeshData;

    struct TransformBase;

    struct TransformationsUpdater : public ISystem
    {
        void fixedUpdate() final;
        
        void setScene(const Ref<Scene>& scene) noexcept final;
    private:
        entt::observer m_transformUpdateObserver;
        entt::observer m_rigidbody3DUpdateObserver;
    };
}
