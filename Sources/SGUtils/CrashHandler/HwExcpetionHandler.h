//
// Created by Ilya on 15.03.2024.
//

#ifndef SUNGEARENGINE_HWEXCPETIONHANDLER_H
#define SUNGEARENGINE_HWEXCPETIONHANDLER_H

#include <csetjmp>
#include <stdexcept>

#include "platform.h"

namespace SGCore
{
    class HwExceptionHandler final
    {
    public:
        HwExceptionHandler();

        ~HwExceptionHandler();
    };

    class ExecutionContext final
    {
        friend class HwException;

    public:
        ExecutionContext();

        ~ExecutionContext();

        jmp_buf environment {};

    private:
        const char* humanReadableName() const;

        ExecutionContext* prev_context;
#if defined(PLATFORM_OS_WINDOWS)
        bool dirty {};
#endif
        long long exception_type {};
    };

    class HwException final : public std::runtime_error
    {
    public:
        explicit HwException(const ExecutionContext& execution_context)
                : std::runtime_error(execution_context.humanReadableName())
        {
        }
    };

#define HW_TO_SW_CONVERTER_UNIQUE_NAME(NAME, LINE) NAME##LINE

#define HW_TO_SW_CONVERTER_INTERNAL(NAME, LINE)                              \
    SGCore::ExecutionContext HW_TO_SW_CONVERTER_UNIQUE_NAME(NAME, LINE); \
    if (setjmp(HW_TO_SW_CONVERTER_UNIQUE_NAME(NAME, LINE).environment))      \
    throw SGCore::HwException(HW_TO_SW_CONVERTER_UNIQUE_NAME(NAME, LINE))

#define HW_TO_SW_CONVERTER() HW_TO_SW_CONVERTER_INTERNAL(execution_context, __LINE__)
}

#endif //SUNGEARENGINE_HWEXCPETIONHANDLER_H
