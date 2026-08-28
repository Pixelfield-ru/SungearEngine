//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <cstdint>
#include <memory>
#include <vector>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IPipelineState.h"
#include "VulkanContext.h"

namespace SGCore
{
    class VulkanDevice;
    class VulkanShaderProgram;

    /// Attachment formats of the render pass a pipeline is used in. Not part of PipelineStateDesc
    /// (the GL backend never needed them), so a VulkanPipelineState keeps one VkPipeline per set of
    /// formats it meets, created lazily at bind time.
    struct VulkanPassFormats
    {
        std::vector<VkFormat> m_colorFormats;
        VkFormat m_depthFormat = VK_FORMAT_UNDEFINED;
        VkFormat m_stencilFormat = VK_FORMAT_UNDEFINED;
        VkSampleCountFlagBits m_samples = VK_SAMPLE_COUNT_1_BIT;

        bool operator==(const VulkanPassFormats&) const noexcept = default;
    };

    class SGCORE_EXPORT VulkanPipelineState final : public IPipelineState
    {
    public:
        VulkanPipelineState(VulkanDevice& device, const PipelineStateDesc& desc) noexcept;
        ~VulkanPipelineState() override;

        /// The VkPipeline for the given pass formats (created on first use). VK_NULL_HANDLE on failure.
        [[nodiscard]] VkPipeline getOrCreate(const VulkanPassFormats& formats) noexcept;

        [[nodiscard]] VulkanShaderProgram* getProgram() const noexcept;
        [[nodiscard]] VkPipelineLayout getLayout() const noexcept;
        [[nodiscard]] VkFrontFace getFrontFace() const noexcept { return m_frontFace; }
        [[nodiscard]] bool usesIndices() const noexcept { return m_desc.m_meshRenderState.m_useIndices; }

        /// Destroys every built VkPipeline; see VulkanShaderProgram::releaseGPU().
        void releaseGPU() noexcept;

    private:
        struct Variant
        {
            VulkanPassFormats m_formats;
            VkPipeline m_pipeline = VK_NULL_HANDLE;
        };

        [[nodiscard]] VkPipeline build(const VulkanPassFormats& formats) noexcept;

        std::shared_ptr<VulkanContext> m_context;
        std::vector<Variant> m_variants;
        VkFrontFace m_frontFace = VK_FRONT_FACE_CLOCKWISE;
    };
}
