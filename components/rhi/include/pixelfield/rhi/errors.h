//
// Created by stuka on 27.09.2026.
//

#pragma once

#include <system_error>

namespace pf::rhi
{
    enum class LogLevel
    {
        kTrace,
        kDebug,
        kInfo,
        kWarn,
        kError,
        kFatal
    };

    enum class Error
    {
        kNotOptimalSwapchain = 1,
    };

    struct RenderErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "RHI"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<Error>(ev))
            {
                case Error::kNotOptimalSwapchain: return "Swapchain for current surface is not optimal.";
                default: return "Unknown error";
            }
        }
    };

    inline const std::error_category& render_error_category()
    {
        static const RenderErrorCategory cat;
        return cat;
    }

    inline std::error_code make_error_code(Error e) noexcept
    {
        return { static_cast<int>(e), render_error_category() };
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<pf::rhi::Error> : true_type {};
}
