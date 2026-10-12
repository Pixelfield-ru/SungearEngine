//
// Created by stuka on 12.10.2026.
//

#pragma once

#include <system_error>

namespace pf::core
{
    enum class CoreError
    {
        kInvalidArgument = 1,
        kUnexpectedError
    };

    struct CoreErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "Core"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<CoreError>(ev))
            {
                case CoreError::kInvalidArgument: return "Invalid argument was passed.";
                case CoreError::kUnexpectedError: return "Unexpected error.";
                default: return "Unknown error";
            }
        }
    };

    inline const std::error_category& core_error_category()
    {
        static const CoreErrorCategory cat;
        return cat;
    }

    inline std::error_code make_error_code(CoreError e) noexcept
    {
        return { static_cast<int>(e), core_error_category() };
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<pf::core::CoreError> : true_type {};
}