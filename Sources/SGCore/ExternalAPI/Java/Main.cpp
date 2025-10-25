//
// Created by stuka on 24.10.2025.
//

#include "Main.h"

#include "JNIManager.h"

#include "SGCore/Logger/AndroidLogcat.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"

#ifdef SG_PLATFORM_OS_ANDROID

#include <android/native_window.h>
#include <android/native_window_jni.h>

void Java_com_pixelfield_sungearstarter_NativeMethods_startSGCore(JNIEnv* env, jclass thisClass, jobject context, jobject surface)
{
    if(!SGCore::Java::JNIManager::initialized())
    {
        SGCore::Java::JNIManager::initialize(env, context);
    }

    if(!SGCore::Java::JNIManager::initialized())
    {
#ifdef SG_PLATFORM_OS_ANDROID
        LOGCAT_E(SGCORE_TAG, "Fatal error while initializing SGCore::Java::JNIManager!")
#else
        // todo: print error
#endif
        return;
    }

    SGCore::__AndroidImpl::setAndroidMainWindowHandle(ANativeWindow_fromSurface(env, surface));

    SGCore::CoreMain::start();

    SGCore::Java::JNIManager::cleanup(env);
}

void SGCore::__AndroidImpl::setAndroidMainWindowHandle(ANativeWindow* window) noexcept
{
    SGCore::CoreMain::getWindow().m_handle = window;
}

#endif
