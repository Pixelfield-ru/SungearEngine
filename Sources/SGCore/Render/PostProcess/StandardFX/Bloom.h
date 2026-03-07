//
// Created by stuka on 07.03.2026.
//

#pragma once

#include "SGCore/Render/PostProcess/PostProcessEffect.h"

namespace SGCore
{
    struct Bloom : PostProcessEffect
    {
        Bloom();

        void passValuesToSubPassShader() noexcept override;

        void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;
    };
}
