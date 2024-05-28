//
// Created by Ilya on 15.03.2024.
//

#ifndef SUNGEARENGINE_HWEXCEPTIONHANDLER_H
#define SUNGEARENGINE_HWEXCEPTIONHANDLER_H

#include <csetjmp>
#include <stdexcept>
#include "sgutils_export.h"
#include "sgcore_export.h"

#include "Platform.h"
#include "SGUtils/Utils.h"

#ifdef PLATFORM_OS_WINDOWS
#include <windows.h>
#include <winnt.h>
#endif
#ifdef PLATFORM_OS_LINUX
#include <csignal>
#include <cstring>
#include <ucontext.h>
#include <err.h>
#endif

#include <boost/stacktrace.hpp>

namespace SGCore
{
    class SGCORE_EXPORT HwExceptionHandler
    {
    private:
        #define eh_print(fmt, ...) printf(fmt, ##__VA_ARGS__); fprintf(fpCrash, fmt, ##__VA_ARGS__);

        static inline std::string m_outputLogFilePath = "last_crash.log";
        static inline std::string m_applicationName = "UNKNOWN";

        static inline size_t m_handlersCount = 0;

        #ifdef PLATFORM_OS_WINDOWS
        static LONG WINAPI winHandler(PEXCEPTION_POINTERS pExceptionInfo)
        {
            std::cout << "fatal error" << std::endl;

            char fnmBuffer[256];
            sprintf(fnmBuffer, m_outputLogFilePath.c_str(), time(0));

            FILE* fpCrash = fopen(fnmBuffer, "a");

            void* exAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;

            auto stackTrace = boost::stacktrace::stacktrace();

            boost::stacktrace::frame problematicFrame;
            for(const auto& f : stackTrace)
            {
                if(f.address() == exAddr)
                {
                    problematicFrame = f;
                    break;
                }
            }

            eh_print("***\n");
            eh_print("*** APPLICATION '%s' FATAL ERROR CRASH\n", m_applicationName.c_str());

            eh_print("***\n");
            eh_print("*** =========================================\n");
            eh_print("***\n");

            eh_print("*** PROBLEMATIC FRAME INFO\n");
            eh_print("*** File: %s\n", problematicFrame.source_file().c_str());
            eh_print("*** Line: %zu\n", problematicFrame.source_line());
            eh_print("*** Function: %s\n", problematicFrame.name().c_str());
            eh_print("*** Address: %p\n", problematicFrame.address());

            eh_print("***\n");
            eh_print("*** =========================================\n");
            eh_print("***\n");

            eh_print("*** Fault address: %p\n", exAddr);
            eh_print("***\n");

            eh_print("*** Stacktrace ");

            eh_print("(%zu frames):\n", stackTrace.size());

            for(size_t i = 0; i < stackTrace.size(); ++i)
            {
                const auto& frame = stackTrace[i];

                std::string src_file = frame.source_file().empty() ? "UNKNOWN" : frame.source_file();

                eh_print("*** %zu) Function: %s (%p) in %s : %zu\n", i, frame.name().c_str(), frame.address(),
                         src_file.c_str(), frame.source_line());
            }

            eh_print("***\n");

            fclose(fpCrash);

            if(pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ACCESS_VIOLATION)
            {
                throw std::runtime_error("Segmentation fault");
            }
            else if(pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_INT_DIVIDE_BY_ZERO)
            {
                throw std::runtime_error("Floating point error");
            }
            else if(pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_FLT_DIVIDE_BY_ZERO)
            {
                throw std::runtime_error("Floating point error");
            }
            else if(pExceptionInfo->ExceptionRecord->ExceptionCode == EXCEPTION_ILLEGAL_INSTRUCTION)
            {
                throw std::runtime_error("Illegal instruction");
            }
            else
            {
                throw std::runtime_error("Unknown signal");
            }

            return EXCEPTION_CONTINUE_SEARCH;
        }
        #endif
        
        #ifdef PLATFORM_OS_LINUX
        static void posixHandler(int sig, siginfo_t* siginfo, void* context)
        {
            char fnmBuffer[256];
            sprintf(fnmBuffer, m_outputLogFilePath.c_str(), time(0));

            FILE* fpCrash = fopen(fnmBuffer, "a");

            auto *u =(ucontext_t *) context;
            void* exAddr = (void*) u->uc_mcontext.gregs[REG_RIP];

            auto stackTrace = boost::stacktrace::stacktrace();

            boost::stacktrace::frame problematicFrame;
            for(const auto& f : stackTrace)
            {
                if(f.address() == exAddr)
                {
                    problematicFrame = f;
                    break;
                }
            }

            eh_print("***\n");
            eh_print("*** APPLICATION '%s' FATAL ERROR CRASH\n", m_applicationName.c_str());

            eh_print("***\n");
            eh_print("*** =========================================\n");
            eh_print("***\n");

            eh_print("*** PROBLEMATIC FRAME INFO\n");
            eh_print("*** File: %s\n", problematicFrame.source_file().c_str());
            eh_print("*** Line: %zu\n", problematicFrame.source_line());
            eh_print("*** Function: %s\n", problematicFrame.name().c_str());
            eh_print("*** Address: %p\n", problematicFrame.address());

            eh_print("***\n");
            eh_print("*** =========================================\n");
            eh_print("***\n");

            eh_print("*** Process ID: %d\n", siginfo->si_pid);
            eh_print("*** User ID: %d\n", siginfo->si_uid);
        #ifdef __LP64__
            eh_print("*** Arch: x64\n");
        #else
            hc_print("*** Arch: x86\n");
        #endif
            eh_print("*** Dump file: %s\n", fnmBuffer);
            eh_print("***\n");

            eh_print("*** Signal: %s\n", strsignal(sig));
            eh_print("*** Signal errno: %d\n", siginfo->si_errno);
            eh_print("*** Signal code: %d\n", siginfo->si_code);

            eh_print("*** Fault address: %p\n", exAddr);
            eh_print("***\n");

            eh_print("*** Stacktrace ");

            eh_print("(%zu frames):\n", stackTrace.size());

            for(size_t i = 0; i < stackTrace.size(); ++i)
            {
                const auto& frame = stackTrace[i];

                std::string src_file = frame.source_file().empty() ? "UNKNOWN" : frame.source_file();

                eh_print("*** %zu) Function: %s (%p) in %s : %zu\n", i, frame.name().c_str(), frame.address(),
                         src_file.c_str(), frame.source_line());
            }

            eh_print("***\n");

            fclose(fpCrash);

            /*struct sigaction sa;
            sa.sa_handler = SIG_DFL;
            sigemptyset(&sa.sa_mask);
            sa.sa_flags = 0;
            sigaction(sig, &sa, 0);
            
            raise(sig);*/
            
            if(siginfo->si_signo == SIGSEGV)
            {
                throw std::runtime_error("Segmentation fault");
            }
            else if(siginfo->si_signo == SIGFPE)
            {
                throw std::runtime_error("Floating point error");
            }
            else if(siginfo->si_signo == SIGILL)
            {
                throw std::runtime_error("Illegal instruction");
            }
            else
            {
                throw std::runtime_error("Unknown signal");
            }
            // raise(sig);
        }
        #endif

        #undef eh_print

    public:
        static std::string getOutputLogFilePath() noexcept
        {
           return m_outputLogFilePath;
        };

        static void setOutputLogFilePath(const std::string& path) noexcept
        {
            m_outputLogFilePath = path;
        };

        static std::string getApplicationName() noexcept
        {
            return m_applicationName;
        }

        static void setApplicationName(const std::string& name) noexcept
        {
            m_applicationName = name;
        }

        static void setupHandler()
        {
            #ifdef PLATFORM_OS_WINDOWS
            // AddVectoredExceptionHandler(m_handlersCount, winHandler);
            SetUnhandledExceptionFilter(winHandler);
            #endif
            
            #ifdef PLATFORM_OS_LINUX
            
            
            #define STACK_SZ (MINSIGSTKSZ + 135 * 1000)
            
            struct sigaction act { };
            act.sa_sigaction = posixHandler;
            sigemptyset(&act.sa_mask);
            act.sa_flags = SA_SIGINFO | SA_NODEFER;
            sigaction(SIGSEGV, &act, 0);
            sigaction(SIGILL, &act, 0);
            sigaction(SIGFPE, &act, 0);
            
            /*stack_t ss;
            ss.ss_sp = malloc(STACK_SZ);
            ss.ss_size = STACK_SZ;
            ss.ss_flags = 0;
            
            struct sigaction sig_action;
            sig_action.sa_sigaction = posixHandler;
            sigemptyset(&sig_action.sa_mask);
            
            sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
            
            sigaction(SIGSEGV, &sig_action, 0);
            sigaction(SIGILL, &sig_action, 0);
            sigaction(SIGFPE, &sig_action, 0);*/
            
            #undef STACK_SZ
            
            // todo:
            #endif

            ++m_handlersCount;
        }
    };
}

#endif //SUNGEARENGINE_HWEXCEPTIONHANDLER_H
