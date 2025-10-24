//
// Created by stuka on 24.10.2025.
//

#pragma once

#include <jni.h>

#include <unordered_map>
#include <unordered_set>

namespace SGCore::Java
{
    enum class JNIInitResult
    {
        JNI_INIT_OK,
        JNI_INIT_ALREADY_INITIALIZED,
        JNI_INIT_UNKNOWN_ERROR
    };

    struct JNINativeThreadHandler
    {
        friend struct JNIManager;

        ~JNINativeThreadHandler();

    private:
        bool m_handlesThread = false;
    };

    struct JNIManager
    {
        friend struct JNINativeThreadHandler;

        static JNIInitResult initialize(JNIEnv* env, jobject context = nullptr) noexcept;
        static bool initialized() noexcept;
        static void cleanup(JNIEnv* env) noexcept;

        static JavaVM* getJavaVM() noexcept;
        static JNIEnv* getEnv(JNINativeThreadHandler& threadHandler) noexcept;
        static jobject getContext() noexcept;

    private:
        static JavaVM* m_jvm;
        static jobject m_context;

        static std::mutex m_accessMutex;
        static bool m_contextUsed;
    };
}