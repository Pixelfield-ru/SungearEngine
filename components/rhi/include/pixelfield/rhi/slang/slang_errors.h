//
// Created by stuka on 12.10.2026.
//

#pragma once

#include <system_error>

namespace pf::rhi
{
    enum class SlangError
    {
        kCannotLoadShader = 1,
        kCannotComposeShader,
        kCannotLinkShader,
        kCannotCompileShader
    };

    struct SlangErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "RHI (Slang)"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<SlangError>(ev))
            {
                case SlangError::kCannotLoadShader: return "Cannot load shader.";
                case SlangError::kCannotComposeShader: return "Cannot compose shader.";
                case SlangError::kCannotLinkShader: return "Cannot link shader.";
                case SlangError::kCannotCompileShader: return "Cannot compiler shader.";
                default: return "Unknown error";
            }
        }
    };

    inline const std::error_category& slang_error_category()
    {
        static const SlangErrorCategory cat;
        return cat;
    }

    inline std::error_code make_error_code(SlangError e) noexcept
    {
        return { static_cast<int>(e), slang_error_category() };
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<pf::rhi::SlangError> : true_type {};
}
