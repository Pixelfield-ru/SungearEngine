//
// Created by stuka on 16.04.2026.
//

#pragma once

#include "SGCore/Render/PostProcess/PostProcessEffect.h"

namespace SGCore
{
    struct SGCORE_EXPORT Distortion : PostProcessEffect
    {
        Distortion();

        void passValuesToSubPassShader() noexcept override;

        void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;
    };
}
