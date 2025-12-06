//
// Created by stuka on 06.12.2025.
//

#pragma once

#include <vector>

#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreGlobals.h"
#include "SGCore/Math/MathPrimitivesUtils.h"
#include "Steps/INavMeshBuildStep.h"

namespace SGCore::Navigation
{
    struct NavMesh : ECS::Component<NavMesh, const NavMesh>
    {
        NavMeshConfig m_config{};
        std::vector<MathPrimitivesUtils::Triangle<>> m_inputSceneTriangles{};

        void useStandardSteps() noexcept;

        void build(std::vector<MathPrimitivesUtils::Triangle<>> sceneTriangles) noexcept;

        template<typename T>
        requires(std::is_base_of_v<INavMeshBuildStep, T>)
        void addStep(std::uint8_t stepPriority, const Ref<T>& step) noexcept
        {
            if(getStep<T>())
            {
                LOG_E(SGCORE_TAG, "Error while adding NavMesh build step: this step already exist. Type of step: ", typeid(T).name())
                return;
            }

            step->m_stepPriority = stepPriority;
            m_steps.insert(m_steps.begin() + stepPriority, step);
        }

        template<typename T>
        requires(std::is_base_of_v<INavMeshBuildStep, T>)
        Ref<T> getStep() const noexcept
        {
            for(const auto& step : m_steps)
            {
                if(dynamic_cast<T*>(step.get()))
                {
                    return std::static_pointer_cast<T>(step);
                }
            }

            return nullptr;
        }

    private:
        std::vector<Ref<INavMeshBuildStep>> m_steps;
    };
}
