//
// Created by ilya on 19.04.24.
//

#ifndef SUNGEARENGINE_SSAO_H
#define SUNGEARENGINE_SSAO_H

#include <SGCore/pch.h>

#include <SGCore/Render/PostProcess/PostProcessEffect.h>
#include "SGCore/Graphics/API/ISubPassShader.h"

namespace SGCore
{
    struct SSAO : PostProcessEffect
    {
        SSAO();
        SSAO(const SSAO&) = default;
        SSAO(SSAO&&) = default;
        
        void generateKernel() noexcept;
        
        void passValuesToSubPassShader(const Ref<ISubPassShader>& subPassShader) noexcept override;
        
        [[nodiscard]] std::uint16_t getSamplesCount() const noexcept;
        void setSamplesCount(std::uint16_t samplesCount) noexcept;
        
    private:
        Ref<ITexture2D> m_noise;
        
        std::uint16_t m_samplesCount = 18;
        std::vector<glm::vec3> m_kernel { };
    };
}

#endif // SUNGEARENGINE_SSAO_H
