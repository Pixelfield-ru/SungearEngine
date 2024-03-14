#pragma once

/***
 * handlecrash.h
 * https://github.com/Crackshell/handlecrash.h
 * modified by pfhgil
 */

#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <ctime>
#include <execinfo.h>
#include <csignal>
#include <err.h>
#include <ucontext.h>
#include <boost/stacktrace.hpp>

#ifndef HC_NO_COMPRESSION
#include "miniz.c"
#endif

#ifndef HC_ALTSTACK_SIZE
#define HC_ALTSTACK_SIZE MINSIGSTKSZ + 135 * 1000
#endif

#ifndef HC_LOG_DESTINATION
#define HC_LOG_DESTINATION "/tmp"
#endif

namespace SGCore::CrashHandler
{
    
    std::string hc_log_file_output = "last_crash.log";
    std::string hc_application_name = "UNKNOWN";

    /**
     * `b64.h' - b64
     *
     * copyright (c) 2014 joseph werle
     */
    static const char b64_table[] = {
            'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H',
            'I', 'J', 'K', 'L', 'M', 'N', 'O', 'P',
            'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
            'Y', 'Z', 'a', 'b', 'c', 'd', 'e', 'f',
            'g', 'h', 'i', 'j', 'k', 'l', 'm', 'n',
            'o', 'p', 'q', 'r', 's', 't', 'u', 'v',
            'w', 'x', 'y', 'z', '0', '1', '2', '3',
            '4', '5', '6', '7', '8', '9', '+', '/'
    };
    
    static char* b64_encode(const unsigned char* src, size_t len)
    {
        int i = 0;
        int j = 0;
        char* enc = NULL;
        size_t size = 0;
        unsigned char buf[4];
        unsigned char tmp[3];
        
        // alloc
        enc = (char*) malloc(0);
        if(NULL == enc)
        { return NULL; }
        
        // parse until end of source
        while(len--)
        {
            // read up to 3 bytes at a time into `tmp'
            tmp[i++] = *(src++);
            
            // if 3 bytes read then encode into `buf'
            if(3 == i)
            {
                buf[0] = (tmp[0] & 0xfc) >> 2;
                buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
                buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
                buf[3] = tmp[2] & 0x3f;
                
                // allocate 4 new byts for `enc` and
                // then translate each encoded buffer
                // part by index from the base 64 index table
                // into `enc' unsigned char array
                enc = (char*) realloc(enc, size + 4);
                for(i = 0; i < 4; ++i)
                {
                    enc[size++] = b64_table[buf[i]];
                }
                
                // reset index
                i = 0;
            }
        }
        
        // remainder
        if(i > 0)
        {
            // fill `tmp' with `\0' at most 3 times
            for(j = i; j < 3; ++j)
            {
                tmp[j] = '\0';
            }
            
            // perform same codec as above
            buf[0] = (tmp[0] & 0xfc) >> 2;
            buf[1] = ((tmp[0] & 0x03) << 4) + ((tmp[1] & 0xf0) >> 4);
            buf[2] = ((tmp[1] & 0x0f) << 2) + ((tmp[2] & 0xc0) >> 6);
            buf[3] = tmp[2] & 0x3f;
            
            // perform same write to `enc` with new allocation
            for(j = 0; (j < i + 1); ++j)
            {
                enc = (char*) realloc(enc, size + 1);
                enc[size++] = b64_table[buf[j]];
            }
            
            // while there is still a remainder
            // append `=' to `enc'
            while((i++ < 3))
            {
                enc = (char*) realloc(enc, size + 1);
                enc[size++] = '=';
            }
        }
        
        // Make sure we have enough space to add '\0' character at end.
        enc = (char*) realloc(enc, size + 1);
        enc[size] = '\0';
        
        return enc;
    }
    
    enum
    {
        hc_dumpstack_none = 0,
        hc_dumpstack_fromsp = 1,
        hc_dumpstack_all = 2
    };
    
    static int _hc_dumpstack_type = hc_dumpstack_fromsp;
#ifndef HC_NO_COMPRESSION
    static bool _hc_dumpstack_compression = true;
#endif

#ifndef HC_MAX_STACK_FRAMES
#define HC_MAX_STACK_FRAMES 64
#endif

#if __GNUC__ < 5 || (defined(__clang__) && (__clang_major__ <= 3 && __clang_minor__ < 8))
#define HC_REGFMT16 "%016lx"
#define HC_REGFMT8 "%08lx"
#define HC_REGFMT4 "%04lx"
#else
#define HC_REGFMT16 "%016llx"
#define HC_REGFMT8 "%08llx"
#define HC_REGFMT4 "%04llx"
#endif
    
    void hc_handler_posix(int sig, siginfo_t* siginfo, void* context)
    {
#define hc_print(fmt, ...) printf(fmt, ##__VA_ARGS__); fprintf(fpCrash, fmt, ##__VA_ARGS__);
#define hc_print_file(fmt, ...) fprintf(fpCrash, fmt, ##__VA_ARGS__);
        
        char fnmBuffer[256];
        sprintf(fnmBuffer, hc_log_file_output.c_str(), time(0));
        
        FILE* fpCrash = fopen(fnmBuffer, "a");
        
        ucontext_t *u =(ucontext_t *) context;
        void* funcAddr = (void*) u->uc_mcontext.gregs[REG_RIP];
        
        auto stack_trace = boost::stacktrace::stacktrace();
        
        boost::stacktrace::frame problematic_frame;
        for(const auto& f : stack_trace)
        {
            if(f.address() == funcAddr)
            {
                problematic_frame = f;
                break;
            }
        }
        
        hc_print("***\n");
        hc_print("*** APPLICATION '%s' FATAL ERROR CRASH\n", hc_application_name.c_str());
        
        hc_print("***\n");
        hc_print("*** =========================================\n");
        hc_print("***\n");
        
        hc_print("*** PROBLEMATIC FRAME INFO\n");
        hc_print("*** File: %s\n", problematic_frame.source_file().c_str());
        hc_print("*** Line: %zu\n", problematic_frame.source_line());
        hc_print("*** Function: %s\n", problematic_frame.name().c_str());
        hc_print("*** Address: %p\n", problematic_frame.address());
        
        hc_print("***\n");
        hc_print("*** =========================================\n");
        hc_print("***\n");
        // hc_print()
        
        hc_print("*** Process ID: %d\n", siginfo->si_pid);
        hc_print("*** User ID: %d\n", siginfo->si_uid);
#ifdef __LP64__
        hc_print("*** Arch: x64\n");
#else
        hc_print("***          Arch: x86\n");
#endif
        hc_print("*** Dump file: %s\n", fnmBuffer);
        hc_print("***\n");
        
        hc_print("*** Signal: %s\n", strsignal(sig));
        hc_print("*** Signal errno: %d\n", siginfo->si_errno);
        hc_print("*** Signal code: %d\n", siginfo->si_code);
        
        hc_print("*** Fault address: %p\n", funcAddr);
        hc_print("***\n");
        
        hc_print("*** Stacktrace ");
        
        hc_print("(%zu frames):\n", stack_trace.size());
        
        for(size_t i = 0; i < stack_trace.size(); ++i)
        {
            const auto& frame = stack_trace[i];
            
            std::string src_file = frame.source_file().empty() ? "UNKNOWN" : frame.source_file();
            
            hc_print("*** %zu) Function: %s (%p) in %s : %zu\n", i, frame.name().c_str(), frame.address(),
                     src_file.c_str(), frame.source_line());
        }
        
        hc_print("***\n");
        
        hc_print("*** Registers:\n");
        
        struct ucontext_t* ctx = (ucontext_t*) context;
#define RV(reg) ctx->uc_mcontext.gregs[reg]
#define FPSTe(i) ctx->uc_mcontext.fpregs->_st[i].exponent
#define FPSTsa(i) (ctx->uc_mcontext.fpregs->_st[i].significand[3] << 16 | ctx->uc_mcontext.fpregs->_st[i].significand[2])
#define FPSTsb(i) (ctx->uc_mcontext.fpregs->_st[i].significand[1] << 16 | ctx->uc_mcontext.fpregs->_st[i].significand[0])

#ifdef __LP64__
        hc_print("***   RAX: " HC_REGFMT16 "     RBX: " HC_REGFMT16 "     RCX: " HC_REGFMT16 "\n", RV(REG_RAX),
                 RV(REG_RBX), RV(REG_RCX));
        hc_print("***   RDX: " HC_REGFMT16 "     RSI: " HC_REGFMT16 "     RDI: " HC_REGFMT16 "\n", RV(REG_RDX),
                 RV(REG_RSI), RV(REG_RDI));
        hc_print("***   RBP: " HC_REGFMT16 "      R8: " HC_REGFMT16 "      R9: " HC_REGFMT16 "\n", RV(REG_RBP),
                 RV(REG_R8), RV(REG_R9));
        hc_print("***   R10: " HC_REGFMT16 "     R11: " HC_REGFMT16 "     R12: " HC_REGFMT16 "\n", RV(REG_R10),
                 RV(REG_R11), RV(REG_R12));
        hc_print("***   R13: " HC_REGFMT16 "     R14: " HC_REGFMT16 "     R15: " HC_REGFMT16 "\n", RV(REG_R13),
                 RV(REG_R14), RV(REG_R15));
        hc_print("***   RSP: " HC_REGFMT16 "\n", RV(REG_RSP));
        hc_print("***\n");
        hc_print("***   RIP: " HC_REGFMT16 "     EFL: " HC_REGFMT8 "\n", RV(REG_RIP), RV(REG_EFL));
        hc_print("***\n");
        hc_print("***    CS: " HC_REGFMT4 "    FS: " HC_REGFMT4 "      GS: " HC_REGFMT4 "\n", RV(REG_CSGSFS) & 0xFFFF,
                 (RV(REG_CSGSFS) >> 16) & 0xFFFF, (RV(REG_CSGSFS) >> 32) & 0xFFFF);
        hc_print("***\n");
        for(int i = 0; i < 8; i += 2)
        {
            hc_print("***   ST(%d) %04hx %08x%08x      ST(%d) %04hx %08x%08x\n", i, FPSTe(i), FPSTsa(i), FPSTsb(i),
                     i + 1, FPSTe(i + 1), FPSTsa(i + 1), FPSTsb(i + 1));
        }
        hc_print("***\n");
#define FPXMMa(i) (ctx->uc_mcontext.fpregs->_xmm[i].element[1] << 8 | ctx->uc_mcontext.fpregs->_xmm[i].element[0])
#define FPXMMb(i) (ctx->uc_mcontext.fpregs->_xmm[i].element[3] << 24 | ctx->uc_mcontext.fpregs->_xmm[i].element[2] << 16)
        for(int i = 0; i < 16; i += 2)
        {
            hc_print("***   XMM%d%s: %016x%016x     XMM%d%s: %016x%016x\n", i, i < 10 ? " " : "", FPXMMa(i), FPXMMb(i),
                     i + 1, i < 10 ? " " : "", FPXMMa(i + 1), FPXMMb(i + 1));
        }
#else
        hc_print("***   EAX: %08x   EBX: %08x   ECX: %08x   EDX: %08x\n", RV(REG_EAX), RV(REG_EBX), RV(REG_ECX), RV(REG_EDX));
        hc_print("***   ESI: %08x   EDI: %08x   EBP: %08x   ESP: %08x\n", RV(REG_ESI), RV(REG_EDI), RV(REG_EBP), RV(REG_ESP));
        hc_print("***\n");
        hc_print("***   EIP: %08x   EFL: %08x\n", RV(REG_EIP), RV(REG_EFL));
        hc_print("***\n");
        hc_print("***   CS: %04x   DS: %04x   ES: %04x   FS: %04x   GS: %04x   SS: %04x\n", RV(REG_CS), RV(REG_DS), RV(REG_ES), RV(REG_FS), RV(REG_GS), RV(REG_SS));
        hc_print("***\n");
        for (int i = 0; i < 8; i += 2) {
            hc_print("***   ST(%d) %04hx %08x%08x      ST(%d) %04hx %08x%08x\n", i, FPSTe(i), FPSTsa(i), FPSTsb(i), i + 1, FPSTe(i + 1), FPSTsa(i + 1), FPSTsb(i + 1));
        }
#endif
        
        hc_print("***\n");
        
        unsigned char* dumpStackBegin = 0;
        unsigned char* dumpStackEnd = 0;
        
        FILE* fpMaps = fopen("/proc/self/maps", "r");
        if(fpMaps != 0)
        {
            hc_print("*** Memory map:\n");
            
            char* line = 0;
            size_t lineLen = 0;
            
            while(true)
            {
                if(getline(&line, &lineLen, fpMaps) == -1)
                {
                    break;
                }
                hc_print("***   %s", line);
                
                if(_hc_dumpstack_type == hc_dumpstack_none)
                {
                    continue;
                }
                if(strstr(line, "[stack]") != 0)
                {
                    sscanf(line, "%p-%p ", &dumpStackBegin, &dumpStackEnd);
                    
                    if(_hc_dumpstack_type == hc_dumpstack_fromsp)
                    {
#ifdef __LP64__
                        dumpStackBegin = (unsigned char*) RV(REG_RSP);
#else
                        dumpStackBegin = (unsigned char*)RV(REG_ESP);
#endif
                    }
                }
            }
            if(line)
            {
                free(line);
            }
            
            fclose(fpMaps);
        }
        
        hc_print("***\n");
        
        if(dumpStackBegin != 0 && dumpStackEnd != 0)
        {
            if(dumpStackBegin > dumpStackEnd)
            {
                hc_print("*** Could not dump stack memory due to invalid pointers: %p > %p!\n", dumpStackBegin,
                         dumpStackEnd);
            }
            else
            {
                hc_print_file("*** Stack memory: %p - %p\n", dumpStackBegin, dumpStackEnd);
                
                unsigned char* compressed = (unsigned char*) dumpStackBegin;
                int compressedSize = (int) (dumpStackEnd - dumpStackBegin);

#ifndef HC_NO_COMPRESSION
                if(_hc_dumpstack_compression)
                {
                    compressed = (unsigned char*) malloc((size_t) (dumpStackEnd - dumpStackBegin));
                    mz_ulong retSize = compressedSize;
                    int ret = mz_compress(compressed, &retSize, dumpStackBegin,
                                          (mz_ulong)(dumpStackEnd - dumpStackBegin));
                    compressedSize = retSize;
                    
                    if(ret != MZ_OK)
                    {
                        hc_print_file("***   Compression failed: %d\n", ret);
                        compressed = dumpStackBegin;
                        compressedSize = (int) (dumpStackEnd - dumpStackBegin);
                    }
                }
#endif
                
                char* mem = b64_encode((unsigned char*) compressed, compressedSize);
                int memlen = strlen(mem);
                
                char* memp = mem;
                char membuffer[121];
                membuffer[120] = '\0';
                while(memlen > 0)
                {
                    memcpy(membuffer, memp, 120);
                    hc_print_file("***   %s\n", membuffer);
                    memp += 120;
                    memlen -= 120;
                }
                
                free(mem);
#ifndef HC_NO_COMPRESSION
                if(compressed != dumpStackBegin)
                {
                    free(compressed);
                }
#endif
            }
        }
        
        fclose(fpCrash);
        
        struct sigaction sa;
        sa.sa_handler = SIG_DFL;
        sigemptyset(&sa.sa_mask);
        sa.sa_flags = 0;
        sigaction(sig, &sa, 0);
        raise(sig);

#undef hc_print
    }

#ifndef HC_NO_COMPRESSION
    
    void hc_install(int dumpstack = hc_dumpstack_all, bool dumpstackCompression = true)
#else
    void hc_install(int dumpstack = hc_dumpstack_all)
#endif
    {
        _hc_dumpstack_type = dumpstack;
#ifndef HC_NO_COMPRESSION
        _hc_dumpstack_compression = dumpstackCompression;
#endif
        
        stack_t ss;
        ss.ss_sp = malloc(HC_ALTSTACK_SIZE);
        ss.ss_size = HC_ALTSTACK_SIZE;
        ss.ss_flags = 0;
        
        if(sigaltstack(&ss, 0) != 0)
        { err(1, "sigaltstack"); }
        
        struct sigaction sig_action;
        sig_action.sa_sigaction = hc_handler_posix;
        sigemptyset(&sig_action.sa_mask);

#ifdef __APPLE__
        sig_action.sa_flags = SA_SIGINFO;
#else
        sig_action.sa_flags = SA_SIGINFO | SA_ONSTACK;
#endif
        
        if(sigaction(SIGSEGV, &sig_action, 0) != 0)
        { err(1, "sigaction"); }
        if(sigaction(SIGILL, &sig_action, 0) != 0)
        { err(1, "sigaction"); }
        if(sigaction(SIGFPE, &sig_action, 0) != 0)
        { err(1, "sigaction"); }
    }
}
