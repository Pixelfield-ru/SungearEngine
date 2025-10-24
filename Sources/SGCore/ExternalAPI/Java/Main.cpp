//
// Created by stuka on 24.10.2025.
//

#include "Main.h"

#include "JNIManager.h"
#include "SGCore/Logger/AndroidLogcat.h"
#include "SGCore/Logger/Logger.h"
#include "SGCore/Main/CoreMain.h"

void Java_com_pixelfield_sungearstarter_NativeMethods_startSGCore(JNIEnv* env, jclass clazz, jobject context)
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

    SGCore::CoreMain::start();

    SGCore::Java::JNIManager::cleanup(env);
}
