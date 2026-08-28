//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include "SGCore/Graphics/RHI/RHILegacyShader.h"

namespace SGCore
{
    /// The DX12 half of the shared legacy shader facade: only where the "currently bound" shader is
    /// kept. Everything else (legacy uniform blocks by reflection, the sampler unit table, the
    /// per-draw descriptor set) lives in RHILegacyShader.
    class DX12Shader final : public RHILegacyShader
    {
        friend class DX12Renderer;

    public:
        ~DX12Shader() noexcept override = default;

    protected:
        DX12Shader() noexcept = default;

        void setAsCurrentShader() const noexcept override;
    };
}

#endif
