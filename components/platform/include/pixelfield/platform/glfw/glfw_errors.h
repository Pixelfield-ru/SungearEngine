//
// Created by stuka on 13.10.2026.
//

#pragma once

#include <system_error>

namespace pf::platform
{
    enum class GLFWError
    {
        kCannotInitGLFW = 1,
        kCannotCreateWindow
    };

    struct GLFWErrorCategory final : public std::error_category
    {
        [[nodiscard]] const char* name() const noexcept override { return "GLFW"; }

        [[nodiscard]] std::string message(int ev) const override
        {
            switch(static_cast<GLFWError>(ev))
            {
                case GLFWError::kCannotInitGLFW: return "Cannot init GLFW library.";
                case GLFWError::kCannotCreateWindow: return "Cannot create GLFW window.";
                default: return "Unknown error";
            }
        }
    };

    inline const std::error_category& glfw_error_category()
    {
        static const GLFWErrorCategory cat;
        return cat;
    }

    inline std::error_code make_error_code(GLFWError e) noexcept
    {
        return { static_cast<int>(e), glfw_error_category() };
    }
}

namespace std
{
    template<>
    struct is_error_code_enum<pf::platform::GLFWError> : true_type {};
}