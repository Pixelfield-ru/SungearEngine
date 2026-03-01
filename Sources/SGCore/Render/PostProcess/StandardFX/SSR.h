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
    };
}
