//
// Created by stuka on 10.05.2023.
//

#pragma once

#ifndef NATIVECORE_SGNOTIMPLEMENTEDEXCEPTION_H
#define NATIVECORE_SGNOTIMPLEMENTEDEXCEPTION_H

#include <exception>
#include <utility>
#include <string>

namespace SGCore
{
    class SGNotImplementedException : std::exception
    {
    private:
        std::string m_message;
    public:
        explicit SGNotImplementedException(std::string message) noexcept : m_message(std::move(message)) { }

        [[nodiscard]] const char* what() const noexcept override
        {
            return m_message.c_str();
        }
    };
}

#endif //NATIVECORE_SGNOTIMPLEMENTEDEXCEPTION_H
