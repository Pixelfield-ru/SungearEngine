//
// Created by stuka on 26.11.2024.
//

#pragma once

#include <SGCore/Render/PostProcess/PostProcessEffect.h>
#include "SGCore/Graphics/API/IShader.h"

namespace SGCore
{
    struct SSAO : PostProcessEffect
    {
        SSAO();
        SSAO(const SSAO&) = default;
        SSAO(SSAO&&) = default;

        void onSetupAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;
        void onRemoveAttachments(const Ref<IFrameBuffer>& targetFrameBuffer) noexcept override;

        void generateKernel() noexcept;

        void passValuesToSubPassShader() noexcept override;

        [[nodiscard]] std::uint16_t getSamplesCount() const noexcept;
        void setSamplesCount(std::uint16_t samplesCount) noexcept;

    private:
        AssetRef<ITexture2D> m_noise;

        std::uint16_t m_samplesCount = 18;
        std::vector<glm::vec3> m_kernel { };
    };
}

