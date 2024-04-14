//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIODEVICE_H
#define SUNGEARENGINE_AUDIODEVICE_H

#include <alc.h>

#include "SGUtils/Utils.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    struct AudioDevice
    {
        ~AudioDevice();
        
        static void init() noexcept;
        
        static Ref<AudioDevice> createAudioDevice(const char* deviceName) noexcept;
        
        void makeCurrent() const noexcept;
        
        SG_NOINLINE static Ref<AudioDevice> getDefaultDevice() noexcept;
        
        bool isLoaded() const noexcept;
        
    private:
        explicit AudioDevice(const char* deviceName);
        
        ALCdevice* m_handler = nullptr;
        ALCcontext* m_context = nullptr;
        
        std::string m_name;
        
        // TODO:
        Ref<AudioDevice> m_fallbackDevice;
        
        // preferred device
        static inline Ref<AudioDevice> m_defaultDevice;
        static inline std::vector<Ref<AudioDevice>> m_devices;
        
        static inline ALCcontext* m_currentContext = nullptr;
    };
}

#endif //SUNGEARENGINE_AUDIODEVICE_H
