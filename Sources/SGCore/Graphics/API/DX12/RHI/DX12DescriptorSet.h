//
// Created by 8bitniksis on 20.08.2026.
//

#pragma once

#if defined(_WIN32)

#include <cstdint>
#include <map>

#include <sgcore_export.h>

#include "SGCore/Graphics/RHI/IDescriptorSet.h"
#include "DX12Texture.h"
#include "DX12GPUBuffer.h"

namespace SGCore
{
    class DX12Device;

    /// Binding table on the CPU side, materialized into a shader-visible descriptor table matching
    /// the pipeline root signature when a draw is recorded (DX12CommandList::flushState()). The DX12
    /// twin of VulkanDescriptorSet, entry for entry.
    class SGCORE_EXPORT DX12DescriptorSet final : public IDescriptorSet
    {
    public:
        struct Entry
        {
            Ref<IGPUBuffer> m_buffer;
            std::uint64_t m_offset { };
            std::uint64_t m_range { };
            bool m_storage { };
            Ref<ITexture2D> m_texture;
            Ref<ICubemapTexture> m_cubemap;
            /// Set when the caller already holds the backend texture; takes precedence over m_texture.
            Ref<DX12Texture> m_dx12Texture;
            /// Typed buffer (GLSL samplerBuffer); takes precedence over the texture fields.
            Ref<DX12GPUBuffer> m_texelBuffer;
            std::uint32_t m_arrayIndex { };
        };

        explicit DX12DescriptorSet(DX12Device& device) noexcept : m_device(device) { }

        void setUniformBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                              std::uint64_t offset = 0, std::uint64_t range = 0) noexcept override;
        void setStorageBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer,
                              std::uint64_t offset = 0, std::uint64_t range = 0) noexcept override;
        void setTexture(std::uint32_t binding, const Ref<ITexture2D>& texture, std::uint32_t arrayIndex = 0) noexcept override;
        void setCubemap(std::uint32_t binding, const Ref<ICubemapTexture>& texture) noexcept override;

        void setBackendTexture(std::uint32_t binding, const Ref<IGPUObject>& texture, std::uint32_t arrayIndex = 0) noexcept override;

        /// DX12-only overload: binds a backend texture without going through a legacy facade.
        void setDX12Texture(std::uint32_t binding, const Ref<DX12Texture>& texture, std::uint32_t arrayIndex = 0) noexcept;

        /// (binding, arrayIndex) -> entry
        [[nodiscard]] const std::map<std::pair<std::uint32_t, std::uint32_t>, Entry>& getEntries() const noexcept { return m_entries; }
        [[nodiscard]] std::uint64_t getVersion() const noexcept { return m_version; }

    private:
        DX12Device& m_device;
        std::map<std::pair<std::uint32_t, std::uint32_t>, Entry> m_entries;
        std::uint64_t m_version { };
    };
}

#endif
