//
// Created by stuka on 14.10.2026.
//

#pragma once

#include <system_error>

namespace pf::runtime
{
    enum class Error
    {
        kIncompleteSystemContext = 1,
        kCyclicSystemGraph
    };

    struct ErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "Runtime"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<Error>(ev))
            {
                case Error::kIncompleteSystemContext: return "Incomplete context was passed.";
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
    struct is_error_code_enum<pf::runtime::Error> : true_type {};
}