//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <string>
#include <vector>
#include <glm/vec4.hpp>

#include "SGCore/Graphics/API/GAPIType.h"
#include "SGCore/Graphics/API/GraphicsDataTypes.h"
#include "SGCore/Graphics/API/RenderState.h"
#include "SGCore/Utils/SGSL/SGSLESubShaderType.h"

// Data types of the render hardware interface (see docs/RHI_DESIGN.md). Everything a render
// pass needs to describe GPU work without touching a graphics API directly.

namespace SGCore
{
    class IFrameBuffer;

    /// Facts about the active backend that render code must query instead of assuming.
    struct DeviceProperties
    {
        GAPIType m_apiType = SG_API_TYPE_UNKNOWN;

        // coordinate conventions differ between GL and the explicit APIs
        bool m_originBottomLeft { };
        bool m_depthZeroToOne { };
        bool m_ndcYFlipRequired { };

        std::uint32_t m_framesInFlight = 1;
        /// Largest texel buffer the device can address, in texels. Batching sizes its vertex/index
        /// texel buffers against this; zero would mean "nothing fits" and reject every mesh.
        std::uint32_t m_maxTexelBufferElements = 1u << 27;
        std::uint32_t m_pushConstantsMaxSize = 128;

        // capabilities that the permanent GL fallback lacks; passes degrade instead of failing
        bool m_supportsExplicitBarriers { };
        bool m_supportsMultithreadedRecording { };
        bool m_supportsBindless { };
    };

    enum class GPUResourceState
    {
        SGG_STATE_UNDEFINED,
        SGG_STATE_RENDER_TARGET,
        SGG_STATE_DEPTH_WRITE,
        SGG_STATE_SHADER_READ,
        SGG_STATE_TRANSFER_SRC,
        SGG_STATE_TRANSFER_DST,
        SGG_STATE_PRESENT
    };

    enum class GPUBufferUsage : std::uint32_t
    {
        SGG_VERTEX_BUFFER = 1u << 0,
        SGG_INDEX_BUFFER = 1u << 1,
        SGG_UNIFORM_BUFFER = 1u << 2,
        SGG_STORAGE_BUFFER = 1u << 3,
        SGG_TRANSFER_SRC = 1u << 4,
        SGG_TRANSFER_DST = 1u << 5
    };

    [[nodiscard]] constexpr GPUBufferUsage operator|(GPUBufferUsage a, GPUBufferUsage b) noexcept
    {
        return static_cast<GPUBufferUsage>(static_cast<std::uint32_t>(a) | static_cast<std::uint32_t>(b));
    }

    [[nodiscard]] constexpr bool hasUsage(GPUBufferUsage usage, GPUBufferUsage flag) noexcept
    {
        return (static_cast<std::uint32_t>(usage) & static_cast<std::uint32_t>(flag)) != 0;
    }

    enum class GPUMemoryAccess
    {
        /// GPU-only memory; filled through ICommandList::uploadData.
        SGG_DEVICE_LOCAL,
        /// CPU-writable, GPU-readable; for per-frame data (uniforms, dynamic vertices).
        SGG_HOST_VISIBLE
    };

    struct GPUBufferDesc
    {
        std::uint64_t m_size { };
        GPUBufferUsage m_usage = GPUBufferUsage::SGG_VERTEX_BUFFER;
        GPUMemoryAccess m_access = GPUMemoryAccess::SGG_DEVICE_LOCAL;
        std::string m_debugName;
    };

    enum class SGIndexType
    {
        SGG_UINT16,
        SGG_UINT32
    };

    struct Viewport
    {
        float m_x { };
        float m_y { };
        float m_width { };
        float m_height { };
        float m_minDepth = 0.0f;
        float m_maxDepth = 1.0f;

        bool operator==(const Viewport&) const noexcept = default;
    };

    struct Scissor
    {
        std::int32_t m_x { };
        std::int32_t m_y { };
        std::int32_t m_width { };
        std::int32_t m_height { };

        bool operator==(const Scissor&) const noexcept = default;
    };

    /// One vertex attribute of the vertex input layout (replaces the OpenGL VAO concept).
    struct VertexAttributeDesc
    {
        std::uint32_t m_location { };
        /// Which bound vertex buffer slot the attribute reads from.
        std::uint32_t m_bufferSlot { };
        SGGDataType m_dataType = SGGDataType::SGG_FLOAT;
        std::uint32_t m_componentsCount = 1;
        std::uint32_t m_offset { };
        bool m_normalized { };

        bool operator==(const VertexAttributeDesc&) const noexcept = default;
    };

    struct VertexBufferSlotDesc
    {
        std::uint32_t m_slot { };
        std::uint32_t m_stride { };
        bool m_perInstance { };

        bool operator==(const VertexBufferSlotDesc&) const noexcept = default;
    };

    struct VertexInputDesc
    {
        std::vector<VertexAttributeDesc> m_attributes;
        std::vector<VertexBufferSlotDesc> m_slots;

        bool operator==(const VertexInputDesc&) const noexcept = default;
    };

    /// Attachment formats a pipeline renders into. On GL this is informational; explicit APIs
    /// bake it into the pipeline object.
    struct RenderTargetsDesc
    {
        std::vector<SGGColorInternalFormat> m_colorFormats;
        SGGColorInternalFormat m_depthFormat = SGGColorInternalFormat::SGG_DEPTH_COMPONENT32;
        bool m_hasDepth { };

        bool operator==(const RenderTargetsDesc&) const noexcept = default;
    };

    /// Source of one shader stage handed to IDevice::createShaderProgram. `m_code` is target
    /// GLSL (output of SGSLEVulkanizer for the device's dialect); `m_spirv` is used by backends
    /// that consume SPIR-V. A backend takes whichever it needs.
    struct ShaderStageSource
    {
        SGSLESubShaderType m_type = SGSLESubShaderType::SST_NONE;
        std::string m_code;
        std::vector<std::uint32_t> m_spirv;
    };

    struct ShaderProgramDesc
    {
        std::vector<ShaderStageSource> m_stages;
        std::string m_debugName;
    };

    enum class LoadOp
    {
        SGG_LOAD,
        SGG_CLEAR,
        SGG_DONT_CARE
    };

    /// Begins rendering into a target. During the migration the target is an existing
    /// IFrameBuffer (nullptr = swapchain backbuffer); it becomes ITexture-based once
    /// framebuffers move under the RHI.
    struct RenderPassBeginDesc
    {
        IFrameBuffer* m_frameBuffer { };
        std::vector<SGFrameBufferAttachmentType> m_colorAttachments;
        LoadOp m_colorLoadOp = LoadOp::SGG_LOAD;
        glm::vec4 m_clearColor { 0.0f, 0.0f, 0.0f, 1.0f };
        LoadOp m_depthLoadOp = LoadOp::SGG_LOAD;
        float m_clearDepth = 1.0f;
        std::int32_t m_width { };
        std::int32_t m_height { };
    };
}
