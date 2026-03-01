//
// Created by stuka on 28.02.2026.
//

#pragma once

#include "SGCore/Render/PostProcess/PostProcessEffect.h"

namespace SGCore
{
    struct Vignette : PostProcessEffect
    {
        Vignette();

        void passValuesToSubPassShader() noexcept override;

        void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;

        float getSmoothness() const noexcept;
        void setSmoothness(float smoothness) noexcept;

        float getRadius() const noexcept;
        void setRadius(float radius) noexcept;

    private:
        float m_radius = 0.6;
        float m_smoothness = 0.3;
    };
}
