//
// Created by ilya on 20.03.24.
//

#ifndef SUNGEARENGINE_DYNAMICLIBRARY_H
#define SUNGEARENGINE_DYNAMICLIBRARY_H

#include "SGUtils/CrashHandler/Platform.h"
#include "SGUtils/TypeInfo.h"

#include <functional>

#ifdef PLATFORM_OS_LINUX
#include <dlfcn.h>
#endif

#ifdef PLATFORM_OS_WINDOWS
#define DL_POSTFIX ".dll"
#elif defined(PLATFORM_OS_LINUX)
#define DL_POSTFIX ".so"
#endif

namespace SGCore
{
    struct DynamicLibrary
    {
        ~DynamicLibrary()
        {
            unload();
        }
        
        void load(const std::string& pluginDLPath, std::string& err) noexcept
        {
            unload();
            
            #ifdef PLATFORM_OS_LINUX
            m_nativeHandler = dlopen(pluginDLPath.c_str(), RTLD_NOW | RTLD_GLOBAL);
            if(!m_nativeHandler)
            {
                err = dlerror();
            }
            #endif
        }
        
        void unload()
        {
            #ifdef PLATFORM_OS_LINUX
            if(m_nativeHandler)
            {
                dlclose(m_nativeHandler);
                m_nativeHandler = nullptr;
            }
            #endif
        }
        
        template<typename FuncT>
        std::function<FuncT> loadFunction(const char* funcName, std::string& err) noexcept
        {
            if(!m_nativeHandler)
            {
                err = "The function cannot be loaded: the library is not loaded.";
                
                return nullptr;
            }
            
            return DLFuncLoader<FuncT>()(m_nativeHandler, funcName, err);
        }
        
        [[nodiscard]] void* getNativeHandler() noexcept
        {
            return m_nativeHandler;
        }
        
    private:
        template<typename> struct DLFuncLoader;
        
        template<typename Result, typename... Args>
        struct DLFuncLoader<Result(Args...)>
        {
            std::function<Result(Args...)> operator()(void* nativeHandler, const char* funcName, std::string& err) const noexcept
            {
                using func_t = Result(*)(Args...);
                
                func_t func = nullptr;
                
                #ifdef PLATFORM_OS_LINUX
                func = (func_t) dlsym(nativeHandler, funcName);
                if(!func)
                {
                    err = dlerror();
                }
                #elif defined(PLATFORM_OS_WINDOWS)
                #endif
                
                return func;
            }
        };
        
        void* m_nativeHandler = nullptr;
    };
}

#endif //SUNGEARENGINE_DYNAMICLIBRARY_H
