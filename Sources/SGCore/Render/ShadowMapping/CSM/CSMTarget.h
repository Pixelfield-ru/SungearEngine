//
// Created by stuka on 08.07.2025.
//

#pragma once

#include "CSMCascade.h"
#include "SGCore/ECS/Component.h"

namespace SGCore
{
    class IShader;

    struct SGCORE_EXPORT CSMTarget : ECS::Component<CSMTarget, const CSMTarget>
    {
        CSMTarget();

        const std::vector<CSMCascade>& getCascades() const noexcept;
        CSMCascade& getCascade(std::uint8_t idx) noexcept;
        const CSMCascade& getCascade(std::uint8_t idx) const noexcept;

        /**
         * @param forShader Bind uniforms to shader.
         * @param cameraZFar Camera far Z.
         * @param texUnitOffset Textures unit offset for CSM shadow maps.
         * @return New textures unit offset.
         */
        [[nodiscard]] int bindUniformsToShader(IShader* forShader,
                                               float cameraZFar,
                                               int texUnitOffset) const noexcept;

    private:
        std::vector<CSMCascade> m_levels;
    };
}
