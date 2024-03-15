//
// Created by Ilya on 15.03.2024.
//

#ifndef SUNGEARENGINE_HWEXCPETIONHANDLER_H
#define SUNGEARENGINE_HWEXCPETIONHANDLER_H

#include <csetjmp>
#include <stdexcept>

#include "platform.h"

#ifdef PLATFORM_OS_WINDOWS
#include <windows.h>
#include <winnt.h>
#endif

#include <boost/stacktrace.hpp>

namespace SGCore::HwExceptionHandler
{
    #define eh_print(fmt, ...) printf(fmt, ##__VA_ARGS__); fprintf(fpCrash, fmt, ##__VA_ARGS__);

    std::string logFileOutput = "last_crash.log";
    std::string applicationName = "UNKNOWN";

    #ifdef PLATFORM_OS_WINDOWS
    LONG WINAPI topLevelExceptionHandler(PEXCEPTION_POINTERS pExceptionInfo)
    {
        std::cout << "fatal error" << std::endl;

        char fnmBuffer[256];
        sprintf(fnmBuffer, logFileOutput.c_str(), time(0));

        FILE* fpCrash = fopen(fnmBuffer, "a");

        void* exAddr = pExceptionInfo->ExceptionRecord->ExceptionAddress;


        //fsfsdfsdfsdfs
        std::cout << "addr: " << exAddr << std::endl;

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
        eh_print("*** APPLICATION '%s' FATAL ERROR CRASH\n", applicationName.c_str());

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

        return EXCEPTION_CONTINUE_SEARCH;
    }
    #endif



    void setupHandler()
    {
        #ifdef PLATFORM_OS_WINDOWS
        SetUnhandledExceptionFilter(topLevelExceptionHandler);
        #elif PLATFORM_OS_LINUX
        // todo:
        #endif
    }
}

#endif //SUNGEARENGINE_HWEXCPETIONHANDLER_H
