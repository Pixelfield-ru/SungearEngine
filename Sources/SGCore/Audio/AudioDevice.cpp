//
// Created by ilya on 13.04.24.
//

#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "AudioDevice.h"
#include "AudioUtils.h"

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
    
    m_handler = alcOpenDevice(deviceName);
    if(m_handler)
    {
        m_context = alcCreateContext(m_handler, nullptr);
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
        ALCboolean contextMadeAsCurrent = alcMakeContextCurrent(m_context);
        if(contextMadeAsCurrent)
        {
            m_currentContext = m_context;
        }
        else
        {
            LOG_E(SGCORE_TAG, "OpenAL error: could not make device`s '{}' context as current. Device: {}, context: {}.",
                  m_name, (void*) m_handler, (void*) m_context)
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
    return m_handler && m_context;
}
