//
// Created by ilya on 13.04.24.
//

#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "AudioDevice.h"

#include <iostream>

#include "AudioUtils.h"

SGCore::Ref<SGCore::AudioDevice> SGCore::AudioDevice::m_defaultDevice;
std::vector<SGCore::Ref<SGCore::AudioDevice>> SGCore::AudioDevice::m_devices;
ALCcontext* SGCore::AudioDevice::m_currentContext {};

SGCore::AudioDevice::AudioDevice(const char* deviceName)
{
    if(!deviceName)
    {
        m_name = "Preferred";
    }
    else
    {
        m_name = deviceName;
    }
    
    m_handle = alcOpenDevice(deviceName);
    if(m_handle)
    {
        m_context = alcCreateContext(m_handle, nullptr);
    }
    else
    {
        LOG_E(SGCORE_TAG, "Could not load a sound device with the name '{}'.", deviceName);
    }
}

void SGCore::AudioDevice::init() noexcept
{
    m_defaultDevice = createAudioDevice(nullptr);
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
    
    if(m_handle)
    {
        AL_CALL(alcCloseDevice, m_handle);
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
    if(m_handle && m_context)
    {
        ALCboolean contextMadeAsCurrent = alcMakeContextCurrent(m_context);
        if(contextMadeAsCurrent)
        {
            m_currentContext = m_context;
        }
        else
        {
            LOG_E(SGCORE_TAG, "OpenAL error: could not make device`s '{}' context as current. Device: {}, context: {}.",
                  m_name, (void*) m_handle, (void*) m_context)
        }
        
        std::cout << "current context: " << alcGetCurrentContext() << std::endl;
    }
}

SGCore::Ref<SGCore::AudioDevice> SGCore::AudioDevice::getDefaultDevice() noexcept
{
    return m_defaultDevice;
}

bool SGCore::AudioDevice::isLoaded() const noexcept
{
    return m_handle && m_context;
}
