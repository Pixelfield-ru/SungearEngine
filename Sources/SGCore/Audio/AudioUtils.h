//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIOUTILS_H
#define SUNGEARENGINE_AUDIOUTILS_H

#include <string>
#include <source_location>
#include <al.h>
#include <AL/alc.h>
#include <AL/alext.h>
#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "SGCore/Utils/TypeTraits.h"
#include "SGCore/Utils/Utils.h"

#define AL_CALL(alFunc, ...) SGCore::AudioUtils::alCallImpl(std::source_location::current(), alFunc, __VA_ARGS__)
#define AL_CALL_E(noError, alFunc, ...) SGCore::AudioUtils::alCallImplE(noError, std::source_location::current(), alFunc, __VA_ARGS__)

namespace SGCore::AudioUtils
{
    static bool checkALErrors(const std::source_location& sourceLocation)
    {
        auto error = alGetError();
        if(error != AL_NO_ERROR)
        {
            std::string errorString;
            switch(error)
            {
                case AL_INVALID_NAME:
                    errorString = "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function.";
                    break;
                case AL_INVALID_ENUM:
                    errorString = "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function.";
                    break;
                case AL_INVALID_VALUE:
                    errorString = "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function.";
                    break;
                case AL_INVALID_OPERATION:
                    errorString = "AL_INVALID_OPERATION: the requested operation is not valid. Maybe current context is nor set.";
                    break;
                case AL_OUT_OF_MEMORY:
                    errorString = "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory.";
                    break;
                default:
                    errorString = "UNKNOWN AL ERROR: " + std::to_string(error);
            }

            LOG_E("OpenAL error ({}) (in {} : {}): {}", error,
                  sourceLocation.file_name(),
                  sourceLocation.line(),
                  errorString);
            
            return false;
        }
        return true;
    }
    
    template<typename ALFunc, typename... Args>
    SG_FORCEINLINE std::enable_if_t<std::is_same_v<func_return_type_t<ALFunc>, void>, void>
    alCallImpl(const std::source_location& sourceLocation,
               ALFunc alFunc,
               Args&& ... args)
    {
        alFunc(args...);
        #ifdef SUNGEAR_DEBUG
        checkALErrors(sourceLocation);
        #endif
    }
    
    template<typename ALFunc, typename... Args>
    std::enable_if_t<!std::is_same_v<func_return_type_t<ALFunc>, void>, func_return_type_t<ALFunc>>
    SG_FORCEINLINE alCallImpl(const std::source_location& sourceLocation,
                              ALFunc alFunc,
                              Args&& ... args)
    {
        auto ret = alFunc(args...);
        #ifdef SUNGEAR_DEBUG
        checkALErrors(sourceLocation);
        #endif
        return ret;
    }
    
    template<typename ALFunc, typename... Args>
    std::enable_if_t<std::is_same_v<func_return_type_t<ALFunc>, void>, void>
    SG_FORCEINLINE alCallImplE(bool& noError,
                               const std::source_location& sourceLocation,
                               ALFunc alFunc,
                               Args&& ... args)
    {
        
        alFunc(std::forward<Args>(args)...);
        #ifdef SUNGEAR_DEBUG
        noError = checkALErrors(sourceLocation);
        #else
        noError = true;
        #endif
    }
    
    template<typename ALFunc, typename... Args>
    std::enable_if_t<!std::is_same_v<func_return_type_t<ALFunc>, void>, func_return_type_t<ALFunc>>
    SG_FORCEINLINE alCallImplE(bool& noError,
                               const std::source_location& sourceLocation,
                               ALFunc alFunc,
                               Args&& ... args)
    {
        auto ret = alFunc(std::forward<Args>(args)...);
        #ifdef SUNGEAR_DEBUG
        noError = checkALErrors(sourceLocation);
        #else
        noError = true;
        #endif
        return ret;
    }
}

#endif //SUNGEARENGINE_AUDIOUTILS_H
