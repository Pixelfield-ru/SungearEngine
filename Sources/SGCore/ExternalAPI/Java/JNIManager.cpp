//
// Created by stuka on 24.10.2025.
//

#include "JNIManager.h"

#include "SGCore/Utils/Platform.h"

JavaVM* SGCore::Java::JNIManager::m_jvm = nullptr;
jobject SGCore::Java::JNIManager::m_context = nullptr;
std::mutex SGCore::Java::JNIManager::m_accessMutex;
bool SGCore::Java::JNIManager::m_contextUsed = false;;

SGCore::Java::JNINativeThreadHandler::~JNINativeThreadHandler()
{
    if(m_handlesThread && JNIManager::initialized())
    {
        std::lock_guard lock(JNIManager::m_accessMutex);
        JNIManager::m_jvm->DetachCurrentThread();
    }
}

SGCore::Java::JNIInitResult SGCore::Java::JNIManager::initialize(JNIEnv* env, jobject context) noexcept
{
    std::lock_guard lock(m_accessMutex);

    if(m_jvm) return JNIInitResult::JNI_INIT_ALREADY_INITIALIZED;

    jint result = env->GetJavaVM(&m_jvm);
    if(result != JNI_OK)
    {
        return JNIInitResult::JNI_INIT_UNKNOWN_ERROR;
    }

    if(context)
    {
        m_contextUsed = true;
        m_context = env->NewGlobalRef(context);
        if(!m_context) return JNIInitResult::JNI_INIT_UNKNOWN_ERROR;
    }
    else m_contextUsed = false;

    return JNIInitResult::JNI_INIT_OK;
}

bool SGCore::Java::JNIManager::initialized() noexcept
{
    std::lock_guard lock(m_accessMutex);

    return (!m_contextUsed || m_context) && m_jvm;
}

void SGCore::Java::JNIManager::cleanup(JNIEnv* env) noexcept
{
    if(!initialized()) return;

    std::lock_guard lock(m_accessMutex);

    if(m_context)
    {
        env->DeleteGlobalRef(m_context);
        m_context = nullptr;
    }

    m_jvm = nullptr;
}

JavaVM* SGCore::Java::JNIManager::getJavaVM() noexcept
{
    return m_jvm;
}

JNIEnv* SGCore::Java::JNIManager::getEnv(JNINativeThreadHandler& threadHandler) noexcept
{
    if (!initialized()) return nullptr;

    std::lock_guard lock(m_accessMutex);

    JNIEnv* env = nullptr;
    jint result = m_jvm->GetEnv(reinterpret_cast<void**>(&env), JNI_VERSION_1_6);

    if (result == JNI_EDETACHED)
    {
        threadHandler.m_handlesThread = true;

        // attaching to current c++ thread

#if JNI_VERSION_1_8
        result = m_jvm->AttachCurrentThread(reinterpret_cast<void**>(&env), nullptr);
#else
        result = m_jvm->AttachCurrentThread(&env, nullptr);
#endif
        if (result != JNI_OK) return nullptr;
    }

    threadHandler.m_handlesThread = false;

    return env;
}

jobject SGCore::Java::JNIManager::getContext() noexcept
{
    return m_context;
}
