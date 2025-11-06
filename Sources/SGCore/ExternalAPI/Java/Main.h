#ifndef _Included_com_pixelfield_sungearstarter_NativeMethods
#define _Included_com_pixelfield_sungearstarter_NativeMethods

#include <jni.h>

#include "SGCore/Utils/Platform.h"

#ifdef __cplusplus
extern "C" {
#endif

#if SG_PLATFORM_OS_ANDROID
  /*
   * Class:     com_pixelfield_sungearstarter_NativeMethods
   * Method:    startSGCore
   * Signature: ()V
   */
  JNIEXPORT void JNICALL Java_com_pixelfield_sungearstarter_AndroidNativeMethods_startCore(JNIEnv* env, jclass thisClass, jobject context, jobject surface);

  JNIEXPORT void JNICALL Java_com_pixelfield_sungearstarter_AndroidNativeMethods_onAppInstanceRestore(JNIEnv* env, jclass thisClass, jobject context, jobject surface);

  // todo: make java sungear core api
  JNIEXPORT void JNICALL Java_com_pixelfield_sungearstarter_AndroidNativeMethods_startMainCycle(JNIEnv* env, jclass thisClass);

  JNIEXPORT void JNICALL Java_com_pixelfield_sungearstarter_AndroidNativeMethods_recreateWindow(JNIEnv* env, jclass thisClass, jobject surface);

  // todo: make java sungear core api
  JNIEXPORT void JNICALL Java_com_pixelfield_sungearstarter_AndroidNativeMethods_loadConfig(JNIEnv* env, jclass thisClass, jstring configPath);

#endif

#ifdef __cplusplus
}

#if SG_PLATFORM_OS_ANDROID
#include <android/native_window.h>

namespace SGCore::__AndroidImpl
{
  void setAndroidMainWindowHandle(ANativeWindow* window) noexcept;
}

#endif

#endif
#endif
