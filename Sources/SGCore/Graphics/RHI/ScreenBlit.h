//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <sgcore_export.h>

#include "ICommandList.h"
#include "IDescriptorSet.h"
#include "IGPUBuffer.h"
#include "IPipelineState.h"
#include "IShaderProgram.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    class IDevice;
    class ITexture2D;

    /**
     * Draws a texture into a rectangle of the swapchain backbuffer through the RHI — the RHI
     * replacement of IRenderer::renderTextureOnScreen and the first migrated pass. Owns the
     * screen quad, the screen shader program (features/screen.sgshader via RHIShaderLoader),
     * the legacy uniform block buffer (u_flipOutput) and a descriptor set.
     */
    class SGCORE_EXPORT ScreenBlit
    {
    public:
        bool init(IDevice& device) noexcept;
        [[nodiscard]] bool isReady() const noexcept { return m_ready; }

        /// Rectangle in window pixels, origin per DeviceProperties::m_originBottomLeft.
        void blit(const ITexture2D* texture, bool flipOutput, int x, int y, int width, int height) noexcept;

    private:
        IDevice* m_device { };
        bool m_ready { };

        Ref<IShaderProgram> m_program;
        Ref<IGPUBuffer> m_vertexBuffer;
        Ref<IGPUBuffer> m_indexBuffer;
        Ref<IGPUBuffer> m_uniformBuffer;
        Ref<IDescriptorSet> m_descriptorSet;
        Ref<IPipelineState> m_pipeline;
        Ref<ICommandList> m_commandList;

        std::uint32_t m_textureBinding { };
        std::uint32_t m_flipOffset { };
        bool m_lastFlip { };
        bool m_flipWritten { };
    };
}
