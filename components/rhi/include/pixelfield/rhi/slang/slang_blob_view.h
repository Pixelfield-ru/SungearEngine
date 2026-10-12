//
// Created by stuka on 03.09.2026.
//

#pragma once

#include <atomic>

#include <slang.h>
#include <slang-com-helper.h>

namespace pf::rhi
{
    class SlangBlobView final : public ISlangBlob
    {
        const void* data_ = nullptr;
        size_t size_ = 0;
        std::atomic<uint32_t> ref_{1};
    public:
        SlangBlobView(const void* data, size_t size) : data_(data), size_(size) {}

        SLANG_NO_THROW SlangResult SLANG_MCALL queryInterface(SlangUUID const& uuid, void** out) override
        {
            if (uuid == ISlangUnknown::getTypeGuid() || uuid == ISlangBlob::getTypeGuid())
            {
                addRef();
                *out = static_cast<ISlangBlob*>(this);
                return SLANG_OK;
            }

            *out = nullptr;

            return SLANG_E_NO_INTERFACE;
        }

        SLANG_NO_THROW uint32_t SLANG_MCALL addRef() override { return ++ref_; }

        SLANG_NO_THROW uint32_t SLANG_MCALL release() override
        {
            const uint32_t n = --ref_;
            if (n == 0) delete this;
            return n;
        }

        SLANG_NO_THROW void const* SLANG_MCALL getBufferPointer() override { return data_; }

        SLANG_NO_THROW size_t SLANG_MCALL getBufferSize() override { return size_; }
    };
}
