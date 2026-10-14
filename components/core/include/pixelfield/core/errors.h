//
// Created by stuka on 12.10.2026.
//

#pragma once

#include <system_error>

namespace pf::core
{
    enum class Error
    {
        kInvalidArgument = 1,
        kUnexpectedError,
        kUninitialized,
    };

    struct ErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "Core"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<Error>(ev))
            {
                case Error::kInvalidArgument: return "Invalid argument was passed.";
                case Error::kUnexpectedError: return "Unexpected error.";
                case Error::kUninitialized: return "Got uninitialized object.";
                default: return "Unknown error";
            }
        }
    };

    inline const std::error_category& error_category()
    {
        static const ErrorCategory cat;
        return cat;
    }

    inline std::error_code make_error_code(Error e) noexcept
    {
        return { static_cast<int>(e), error_category() };
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<pf::core::Error> : true_type {};
}