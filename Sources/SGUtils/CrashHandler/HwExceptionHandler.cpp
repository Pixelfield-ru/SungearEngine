//
// Created by Ilya on 15.03.2024.
//

#ifndef SUNGEARENGINE_HWEXCEPTIONHANDLER_CPP
#define SUNGEARENGINE_HWEXCEPTIONHANDLER_CPP

#include <atomic>
#include <cstring>
#include <mutex>
#include <iostream>

#include "HwExcpetionHandler.h"
#include "platform.h"

#if defined(PLATFORM_OS_WINDOWS)
#include <cassert>
#include <excpt.h>
#include <windows.h>
#else
#include <csignal>
#include <pthread.h>
#endif

namespace SGCore {

#if defined(PLATFORM_OS_WINDOWS)
    static thread_local ExecutionContext* execution_context = nullptr;
#else
    template<typename T>
    class ThreadLocal final {
    public:
        ThreadLocal() { pthread_key_create(&key, nullptr); }
        ~ThreadLocal() { pthread_key_delete(key); }

        ThreadLocal& operator=(const T* value) { set(value); return *this; }

        const T* operator*() const { return get(); }
        T* operator->() const { return get(); }
        explicit operator bool() const { return get(); }
        explicit operator T*() const { return get(); }

    private:
        void set(const T* value) { pthread_setspecific(key, value); }
        T* get() const { return reinterpret_cast<T*>(pthread_getspecific(key)); }

        pthread_key_t key{};
    };

    static ThreadLocal<ExecutionContext> execution_context;
#endif

    static int user_count = 0;
    static std::mutex mutex;

#if defined(PLATFORM_OS_WINDOWS)
    static PVOID exception_handler_handle = nullptr;
#endif

    ExecutionContext::ExecutionContext()
            : prev_context(execution_context)
    {
        execution_context = this;
    }

    ExecutionContext::~ExecutionContext()
    {
#if defined(PLATFORM_OS_WINDOWS)
        // This Windows-specific fragment of code is pure magic.
    // Do not try to find a reasonable justification of existence of this code.
    // But believe that some tests will fail if this magic code is removed.
    if (execution_context != this)
        return;
    if (execution_context->dirty)
        RemoveVectoredExceptionHandler(exception_handler_handle);
#endif
        execution_context = execution_context->prev_context;
    }

    struct ExecutionContextStruct {
        jmp_buf environment;
        ExecutionContext* prev_context;
#if defined(PLATFORM_OS_WINDOWS)
        bool dirty;
#endif
        int exception_type;
    };

#if defined(PLATFORM_OS_WINDOWS)

    static LONG WINAPI vectoredExceptionHandler(struct _EXCEPTION_POINTERS* _exception_info)
{
    if (!execution_context || (_exception_info->ExceptionRecord->ExceptionCode != EXCEPTION_ACCESS_VIOLATION &&
                               _exception_info->ExceptionRecord->ExceptionCode != EXCEPTION_ILLEGAL_INSTRUCTION &&
                               _exception_info->ExceptionRecord->ExceptionCode != EXCEPTION_INT_DIVIDE_BY_ZERO &&
                               _exception_info->ExceptionRecord->ExceptionCode != EXCEPTION_STACK_OVERFLOW))
        return EXCEPTION_CONTINUE_SEARCH;

    reinterpret_cast<ExecutionContextStruct*>(static_cast<ExecutionContext*>(execution_context))->dirty = true;
    reinterpret_cast<ExecutionContextStruct*>(static_cast<ExecutionContext*>(execution_context))->exception_type =
        _exception_info->ExceptionRecord->ExceptionCode;
    std::cout << "bye bye" << std::endl;
    longjmp(execution_context->environment, 0);
}

#else

#define MIN2(A, B) (A < B ? A : B)
#define MIN4(A, B, C, D) (MIN2(A, B) < MIN2(C, D) ? MIN2(A, B) : MIN2(C, D))
#define MAX2(A, B) (A > B ? A : B)
#define MAX4(A, B, C, D) (MAX2(A, B) > MAX2(C, D) ? MAX2(A, B) : MAX2(C, D))

#define MIN_SIGNUM MIN4(SIGBUS, SIGFPE, SIGILL, SIGSEGV)
#define MAX_SIGNUM MAX4(SIGBUS, SIGFPE, SIGILL, SIGSEGV)

    static const int handled_signals[4] = { SIGBUS, SIGFPE, SIGILL, SIGSEGV };
    static struct sigaction prev_handlers[MAX_SIGNUM - MIN_SIGNUM + 1];
    static uint8_t exception_handler_stack[SIGSTKSZ];

    static void signalHandler(int signum)
    {
        if (execution_context) {
            sigset_t signals;
            sigemptyset(&signals);
            sigaddset(&signals, signum);
            sigprocmask(SIG_UNBLOCK, &signals, nullptr);
            reinterpret_cast<ExecutionContextStruct*>(static_cast<ExecutionContext*>(execution_context))->exception_type = signum;
            longjmp(execution_context->environment, 0);
        }
        else if (prev_handlers[signum - MIN_SIGNUM].sa_handler) {
            prev_handlers[signum - MIN_SIGNUM].sa_handler(signum);
        }
        else {
            signal(signum, SIG_DFL);
            raise(signum);
        }
    }

#endif

    HwExceptionHandler::HwExceptionHandler()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (user_count++ != 0)
            return;

#if defined(PLATFORM_OS_WINDOWS)
        exception_handler_handle = AddVectoredExceptionHandler(1, vectoredExceptionHandler);
#else

        static std::once_flag once_flag;

        std::call_once(once_flag, [] {
            stack_t ss;
            ss.ss_sp = exception_handler_stack;
            ss.ss_flags = 0;
            ss.ss_size = SIGSTKSZ;
            sigaltstack(&ss, nullptr);
        });

        struct sigaction sa{};
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = SA_ONSTACK;
        sa.sa_handler = signalHandler;

        for (int signum : handled_signals)
            sigaction(signum, &sa, &prev_handlers[signum - MIN_SIGNUM]);
#endif
    }

    HwExceptionHandler::~HwExceptionHandler()
    {
        std::lock_guard<std::mutex> lock(mutex);

        if (--user_count != 0)
            return;

#if defined(PLATFORM_OS_WINDOWS)
        RemoveVectoredExceptionHandler(exception_handler_handle);
    exception_handler_handle = nullptr;
#else
        for (int signum : handled_signals) {
            if (prev_handlers[signum - MIN_SIGNUM].sa_handler)
                sigaction(signum, &prev_handlers[signum - MIN_SIGNUM], nullptr);
            else
                signal(signum, SIG_DFL);
        }
#endif
    }

    const char* ExecutionContext::humanReadableName() const
    {
#if defined(PLATFORM_OS_WINDOWS)
        switch (exception_type)
        {
            case EXCEPTION_ACCESS_VIOLATION:
                return "Access violation";
            case EXCEPTION_ILLEGAL_INSTRUCTION:
                return "Illegal instruction";
            case EXCEPTION_INT_DIVIDE_BY_ZERO:
                return "Divide by zero";
            case EXCEPTION_STACK_OVERFLOW:
                return "Stack overflow";
            default:
                assert(0);
                return nullptr;
        }
#else
        return strsignal(exception_type);
#endif
    }
}

#endif //SUNGEARENGINE_HWEXCEPTIONHANDLER_CPP
