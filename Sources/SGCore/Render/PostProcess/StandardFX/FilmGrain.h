//
// Created by stuka on 28.02.2026.
//

#pragma once

#include "SGCore/Render/PostProcess/PostProcessEffect.h"

namespace SGCore
{
    struct FilmGrain : PostProcessEffect
    {
        FilmGrain();

        void passValuesToSubPassShader() noexcept override;

        void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;
        void onRemoveAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;

        float getIntensity() const noexcept;
        void setIntensity(float intensity) noexcept;

    private:
        float m_intensity = 1.0f;
    };
}
