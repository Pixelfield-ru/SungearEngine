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
        
        static Ref<AudioDevice> createAudioDevice(const char* deviceName) noexcept;
        
        void makeCurrent() const noexcept;
        
        SG_NOINLINE static AudioDevice* getDefaultDevice() noexcept;
        
        bool isLoaded() const noexcept;
        
    private:
        explicit AudioDevice(const char* deviceName);
        
        ALCdevice* m_handler = nullptr;
        ALCcontext* m_context = nullptr;
        Ref<AudioDevice> m_fallbackDevice;
        
        // preferred device
        static inline AudioDevice* m_defaultDevice = createAudioDevice(nullptr).get();
        static inline std::vector<Ref<AudioDevice>> m_devices;
        
        static inline ALCcontext* m_currentContext = nullptr;
    };
}

#endif //SUNGEARENGINE_AUDIODEVICE_H
