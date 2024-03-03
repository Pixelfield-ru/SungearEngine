//
// Created by stuka on 02.05.2023.
//

#pragma once

#include <entt/entity/observer.hpp>
#include <entt/entity/registry.hpp>
#include <entt/entity/entity.hpp>
#include <thread>
#include "SGCore/Scene/ISystem.h"
#include "SGUtils/Utils.h"
#include "SGUtils/Timer.h"

namespace SGCore
{
    class IMeshData;

    struct TransformBase;

    struct TransformationsUpdater : public ISystem
    {
        TransformationsUpdater();
        ~TransformationsUpdater();
        
        void fixedUpdate(const double& dt, const double& fixedDt) noexcept final;
        
        Timer m_updaterTimer;
        
        void setScene(const Ref<Scene>& scene) noexcept final;
        
    private:
        bool m_isAlive = true;
        
        void updateTransformations(const double& dt, const double& fixedDt) noexcept;
        
        std::thread m_updaterThread;
    };
}
