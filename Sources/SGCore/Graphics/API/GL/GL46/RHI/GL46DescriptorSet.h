//
// Created by 8bitniksis on 17.08.2026.
//

#pragma once

#include <vector>
#include <glad/glad.h>

#include "SGCore/Graphics/RHI/IDescriptorSet.h"

namespace SGCore
{
    /// A recorded list of GL binding calls, replayed by GL46CommandList::bindDescriptorSet.
    class GL46DescriptorSet final : public IDescriptorSet
    {
    public:
        void setUniformBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer, std::uint64_t offset, std::uint64_t range) noexcept override;
        void setStorageBuffer(std::uint32_t binding, const Ref<IGPUBuffer>& buffer, std::uint64_t offset, std::uint64_t range) noexcept override;
        void setTexture(std::uint32_t binding, const Ref<ITexture2D>& texture, std::uint32_t arrayIndex) noexcept override;
        void setCubemap(std::uint32_t binding, const Ref<ICubemapTexture>& texture) noexcept override;

        void apply() const noexcept;

    private:
        struct BufferEntry
        {
            GLenum m_target { };
            std::uint32_t m_binding { };
            Ref<IGPUBuffer> m_buffer;
            std::uint64_t m_offset { };
            std::uint64_t m_range { };
        };

        struct TextureEntry
        {
            std::uint32_t m_unit { };
            GLuint m_handle { };
            /// Kept alive while the set references it.
            Ref<ITexture2D> m_texture2D;
            Ref<ICubemapTexture> m_cubemap;
        };

        std::vector<BufferEntry> m_buffers;
        std::vector<TextureEntry> m_textures;

        void setBuffer(GLenum target, std::uint32_t binding, const Ref<IGPUBuffer>& buffer, std::uint64_t offset, std::uint64_t range) noexcept;
    };
}
