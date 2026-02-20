//
// Created by ilya on 13.04.24.
//

#ifndef SUNGEARENGINE_AUDIODEVICE_H
#define SUNGEARENGINE_AUDIODEVICE_H

#include <alc.h>

#include "SGCore/Utils/Macroses.h"
#include "SGCore/Main/CoreGlobals.h"

namespace SGCore
{
    /**
     * Wrapper of OpenAL audio device.
     */
    struct AudioDevice
    {
        /**
         * Resets current context if context of device is current.\n
         * Destroys context.\n
         * Destroys handle of device.
         */
        ~AudioDevice();

        /**
         * Creates default device. Does not set default device as current.
         */
        static void init() noexcept;

        /**
         * Creates new audio device.
         * @param deviceName Name of device.
         * @return New device.
         */
        static Ref<AudioDevice> createAudioDevice(const char* deviceName) noexcept;

        /**
         * Makes device as current to play audio.
         */
        void makeCurrent() const noexcept;

        /**
         * @return Default device that was created
         */
        SG_NOINLINE static Ref<AudioDevice> getDefaultDevice() noexcept;

        /**
         * @return Whether context and handle of device are valid.
         */
        bool isLoaded() const noexcept;
        
    private:
        explicit AudioDevice(const char* deviceName);
        
        ALCdevice* m_handle = nullptr;
        ALCcontext* m_context = nullptr;
        
        std::string m_name;
        
        // TODO:
        Ref<AudioDevice> m_fallbackDevice;
        
        // preferred device
        static Ref<AudioDevice> m_defaultDevice;
        static std::vector<Ref<AudioDevice>> m_devices;
        
        static ALCcontext* m_currentContext;
    };
}

#endif //SUNGEARENGINE_AUDIODEVICE_H
