//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIOUTILS_H
#define SUNGEARENGINE_AUDIOUTILS_H

#include <string>
#include <source_location>
#include <al.h>
#include <spdlog/spdlog.h>

#include "SGUtils/TypeTraits.h"

#ifdef SUNGEAR_DEBUG
#define AL_CALL(alFunc, ...) SGCore::AudioUtils::alCallImpl(std::source_location::current(), alFunc, __VA_ARGS__)
#else
#define AL_CALL(alFunc, ...) alFunc(__VA_ARGS__)
#endif

namespace SGCore::AudioUtils
{
    bool checkALErrors(const std::source_location& sourceLocation)
    {
        ALenum error = alGetError();
        if(error != AL_NO_ERROR)
        {
            std::string errorString;
            switch(error)
            {
                case AL_INVALID_NAME:
                    errorString = "AL_INVALID_NAME: a bad name (ID) was passed to an OpenAL function";
                    break;
                case AL_INVALID_ENUM:
                    errorString = "AL_INVALID_ENUM: an invalid enum value was passed to an OpenAL function";
                    break;
                case AL_INVALID_VALUE:
                    errorString = "AL_INVALID_VALUE: an invalid value was passed to an OpenAL function";
                    break;
                case AL_INVALID_OPERATION:
                    errorString = "AL_INVALID_OPERATION: the requested operation is not valid";
                    break;
                case AL_OUT_OF_MEMORY:
                    errorString = "AL_OUT_OF_MEMORY: the requested operation resulted in OpenAL running out of memory";
                    break;
                default:
                    errorString = "UNKNOWN AL ERROR: " + std::to_string(error);
            }
            
            spdlog::error("OpenAL error (in {0} : {1}): {2}", sourceLocation.file_name(), sourceLocation.line(), errorString);
            
            return false;
        }
        return true;
    }
    
    template<typename ALFunc, typename... Args>
    std::enable_if_t<std::is_same_v<func_return_type_t<ALFunc>, void>, void> alCallImpl(const std::source_location& sourceLocation,
                                                                                        ALFunc alFunc,
                                                                                        Args... args)
    {
        alFunc(std::forward<Args>(args)...);
        checkALErrors(sourceLocation);
    }
    
    template<typename ALFunc, typename... Args>
    std::enable_if_t<!std::is_same_v<func_return_type_t<ALFunc>, void>, func_return_type_t<ALFunc>>
    alCallImpl(const std::source_location& sourceLocation,
               ALFunc alFunc,
               Args... args)
    {
        auto ret = alFunc(std::forward<Args>(args)...);
        checkALErrors(sourceLocation);
        return ret;
    }
}

#endif //SUNGEARENGINE_AUDIOUTILS_H
