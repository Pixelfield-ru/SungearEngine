//
// Created by stuka on 01.03.2026.
//

#pragma once

#include "SGCore/Render/PostProcess/PostProcessEffect.h"

namespace SGCore
{
    struct SSR : PostProcessEffect
    {
        SSR();

        void passValuesToSubPassShader() noexcept override;

        void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;

        void setBlurRadius(float blurRadius) noexcept;
        float getBlurRadius() const noexcept;

        void setMaxSteps(std::int32_t maxSteps) noexcept;
        std::int32_t getMaxSteps() const noexcept;

        void setInitialStepSize(float initialStepSize) noexcept;
        float getInitialStepSize() const noexcept;

        void setMinStepSize(float minStepSize) noexcept;
        float getMinStepSize() const noexcept;

        void setIntensity(float intensity) noexcept;
        float getIntensity() const noexcept;

    private:
        float m_blurRadius = 1.0f;
        std::int32_t m_maxSteps = 20;
        float m_initialStepSize = 5.0f;
        float m_minStepSize = 1.0f;
        float m_intensity = 25000.0f;
    };
}
