//
// Created by stuka on 24.10.2025.
//

#include "Main.h"

#include "JNIManager.h"

#include "SGCore/Logger/AndroidLogcat.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"

#if SG_PLATFORM_OS_ANDROID

#include <android/native_window.h>
#include <android/native_window_jni.h>

#include "SGCore/Utils/FileUtils.h"
#include "SGCore/Utils/StringInterpolation/InterpolationResolver.h"

void Java_com_pixelfield_sungearstarter_AndroidNativeMethods_startCore(JNIEnv* env, jclass thisClass, jobject context, jobject surface)
{
    if(!SGCore::Java::JNIManager::initialized())
    {
        SGCore::Java::JNIManager::initialize(env, context);
    }

    if(!SGCore::Java::JNIManager::initialized())
    {
        // CAN NOT USE LOG_E MACRO BECAUSE Core IS NOT INITIALIZED
        LOGCAT_E(SGCORE_TAG, "Fatal error while initializing SGCore::Java::JNIManager!")
        return;
    }

    SGCore::PathInterpolationMarkupSpec::setKey("appResources", SGCore::FileUtils::getAppResourcesPath());

    SGCore::__AndroidImpl::setAndroidMainWindowHandle(ANativeWindow_fromSurface(env, surface));

    SGCore::CoreMain::init();
}

void Java_com_pixelfield_sungearstarter_AndroidNativeMethods_startMainCycle(JNIEnv* env, jclass thisClass)
{
    SGCore::CoreMain::startCycle();

    SGCore::Java::JNIManager::cleanup(env);
}

void SGCore::__AndroidImpl::setAndroidMainWindowHandle(ANativeWindow* window) noexcept
{
    SGCore::CoreMain::getWindow().m_handle = window;
}

void Java_com_pixelfield_sungearstarter_AndroidNativeMethods_loadConfig(JNIEnv* env, jclass thisClass,
    jstring configPath)
{
    const char* nativeString = env->GetStringUTFChars(configPath, nullptr);
    if(nativeString == nullptr)
    {
        LOG_E(SGCORE_TAG, "Can not get native string for configPath argument in Java native function 'Java_com_pixelfield_sungearstarter_AndroidNativeMethods_loadConfig'")
        return;
    }

    const std::filesystem::path configPathCXX = nativeString;

    env->ReleaseStringUTFChars(configPath, nativeString);

    SGCore::CoreMain::loadConfig(configPathCXX);
}

#endif
