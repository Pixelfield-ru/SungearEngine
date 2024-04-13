//
// Created by ilya on 13.04.24.
//

#include <spdlog/spdlog.h>

#include "AudioDevice.h"
#include "AudioUtils.h"

SGCore::AudioDevice::AudioDevice(const char* deviceName)
{
    m_handler = AL_CALL(alcOpenDevice, deviceName);
    if(m_handler)
    {
        m_context = AL_CALL(alcCreateContext, m_handler, nullptr);
    }
    else
    {
        spdlog::error("Could not load  load a sound device with the name '{0}'.", deviceName);
    }
}

SGCore::AudioDevice::~AudioDevice()
{
    if(m_currentContext == m_context)
    {
        AL_CALL(alcMakeContextCurrent, nullptr);
    }
    
    if(m_context)
    {
        AL_CALL(alcDestroyContext, m_context);
    }
    
    if(m_handler)
    {
        AL_CALL(alcCloseDevice, m_handler);
    }
}

SGCore::Ref<SGCore::AudioDevice> SGCore::AudioDevice::createAudioDevice(const char* deviceName) noexcept
{
    Ref<AudioDevice> newAudioDevice = Ref<AudioDevice>(new AudioDevice(deviceName));
    m_devices.push_back(newAudioDevice);
    
    return newAudioDevice;
}

void SGCore::AudioDevice::makeCurrent() const noexcept
{
    if(m_handler && m_context)
    {
        AL_CALL(alcMakeContextCurrent, m_context);
        m_currentContext = m_context;
    }
}

SGCore::AudioDevice* SGCore::AudioDevice::getDefaultDevice() noexcept
{
    return m_defaultDevice;
}

bool SGCore::AudioDevice::isLoaded() const noexcept
{
    return m_handler && m_context;
}
