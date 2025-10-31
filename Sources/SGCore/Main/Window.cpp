#include <spdlog/spdlog.h>
#include <SGCore/Logger/Logger.h>

#include "SGCore/Graphics/API/IRenderer.h"
#include "Window.h"
#include "CoreMain.h"
#include "SGCore/ExternalAPI/Java/JNIManager.h"

#include "SGCore/Input/PCInput.h"
#include "SGCore/Logger/AndroidLogcat.h"

void SGCore::Window::create()
{
#if SG_PLATFORM_PC
    glfwSetErrorCallback(errorCallback);

    if(!glfwInit())
    {
        LOG_E(SGCORE_TAG, "Failed to initialize GLFW!\n{0}", SG_CURRENT_LOCATION_STR);
    }

    LOG_I(SGCORE_TAG, "-----------------------------------");
    LOG_I(SGCORE_TAG, "GLFW info:");
    LOG_I(SGCORE_TAG,
          "GLFW version is {}.{}.{}",
          GLFW_VERSION_MAJOR,
          GLFW_VERSION_MINOR,
          GLFW_VERSION_REVISION);
    LOG_I(SGCORE_TAG, "-----------------------------------");

    glfwDefaultWindowHints(); // установка для будущего окна дефолтных настроек
    glfwWindowHint(GLFW_VISIBLE, GLFW_FALSE);

    // OpenGL is the default API for GLFW, so it's not here
    if(CoreMain::getRenderer()->getGAPIType() == GAPIType::SG_API_TYPE_VULKAN)
    {
        glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    }

    m_handle = glfwCreateWindow(this->m_config.m_sizeX, this->m_config.m_sizeY, this->m_config.m_title.c_str(), m_config.m_fullsreen ? glfwGetPrimaryMonitor() : nullptr, nullptr);

    if(!m_handle)
    {
        LOG_E(SGCORE_TAG, "Failed to initialize GLFW Window!\n{}", SG_CURRENT_LOCATION_STR);
        return;
    }
    
    glfwSetWindowCloseCallback(m_handle, nativeCloseCallback);
    glfwSetWindowIconifyCallback(m_handle, nativeIconifyCallback);
    glfwSetFramebufferSizeCallback(m_handle, nativeFramebufferSizeCallback);

    makeCurrent();

    glfwSetWindowPos(m_handle, this->m_config.m_positionX, this->m_config.m_positionY);
    glfwSetWindowSizeLimits(m_handle, this->m_config.m_sizeMinLimitX, this->m_config.m_sizeMinLimitY, this->m_config.m_sizeMaxLimitX, this->m_config.m_sizeMaxLimitY);
    glfwSetWindowUserPointer(m_handle, this);

    glfwShowWindow(m_handle);

    setSwapInterval(this->m_config.m_swapInterval);

    glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, this->m_config.m_enableStickyKeys);
    glfwSetInputMode(m_handle, GLFW_LOCK_KEY_MODS, GLFW_TRUE);

    // ------------- post create
    const GLFWvidmode* primaryVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());

    if(m_config.m_useHalfMonitor)
    {
        setSize(primaryVideoMode->width / 2, primaryVideoMode->height / 2);
    }

    if(m_config.m_centralizeWindow)
    {
        int wndSizeX = 0;
        int wndSizeY = 0;

        getSize(wndSizeX, wndSizeY);
        setPosition((primaryVideoMode->width - wndSizeX) / 2, (primaryVideoMode->height - wndSizeY) / 2);
    }
    // -------------------------
#elif SG_PLATFORM_OS_ANDROID
    if(!m_handle)
    {
        LOG_E(SGCORE_TAG, "SGCore: Failed to create second window. In: {}", SG_CURRENT_LOCATION_STR);
        return;
    }

    m_eglDisplay = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    if(m_eglDisplay == EGL_NO_DISPLAY)
    {
        LOG_E(SGCORE_TAG, "SGCore: Failed to create EGL display. In: {}", SG_CURRENT_LOCATION_STR);
        return;
    }

    EGLint major, minor;
    if(!eglInitialize(m_eglDisplay, &major, &minor))
    {
        LOG_E(SGCORE_TAG, "SGCore: Failed to initialize EGL. In: {}", SG_CURRENT_LOCATION_STR);
        return;
    }

    // todo: make customizable
    const EGLint configAttribs[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_ES3_BIT,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_BLUE_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_RED_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_DEPTH_SIZE, 24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };

    EGLint numConfigs;
    if(!eglChooseConfig(m_eglDisplay, configAttribs, &m_eglConfig, 1, &numConfigs))
    {
        LOG_E(SGCORE_TAG, "SGCore: Failed to choose EGL config. In: {}", SG_CURRENT_LOCATION_STR);
        return;
    }

    m_eglSurface = eglCreateWindowSurface(m_eglDisplay, m_eglConfig, m_handle, nullptr);
    if(m_eglSurface == EGL_NO_SURFACE)
    {
        LOG_E(SGCORE_TAG, "SGCore: Failed to create EGL Surface. In: {}", SG_CURRENT_LOCATION_STR);
        return;
    }

    // todo: customizable
    const EGLint contextAttribs[] = {
        EGL_CONTEXT_CLIENT_VERSION, 3,
        EGL_NONE
    };

    m_eglContext = eglCreateContext(m_eglDisplay, m_eglConfig, EGL_NO_CONTEXT, contextAttribs);
    if(m_eglContext == EGL_NO_CONTEXT)
    {
        LOG_E(SGCORE_TAG, "SGCore: Failed to create EGL context. In: {}", SG_CURRENT_LOCATION_STR);
        return;
    }

    makeCurrent();

    LOG_I(SGCORE_TAG, "SGCore: EGL Window initialized. Version: {}.{}", major, minor);

#endif
}

void SGCore::Window::recreate()
{
#if SG_PLATFORM_PC
    glfwMakeContextCurrent(nullptr);
    glfwDestroyWindow(m_handle);
#elif SG_PLATFORM_OS_ANDROID
#endif

    create();
}

void SGCore::Window::makeCurrent() noexcept
{
    Input::PC::setupInput(*this);

    GAPIType apiType = CoreMain::getRenderer()->getGAPIType();
    if(apiType >= GAPIType::SG_API_TYPE_GL4 && apiType <= GAPIType::SG_API_TYPE_GLES3)
    {
#if SG_PLATFORM_PC
        glfwMakeContextCurrent(m_handle);
#elif SG_PLATFORM_OS_ANDROID
        LOG_I(SGCORE_TAG, "EGL Display was made current")
        eglMakeCurrent(m_eglDisplay, m_eglSurface, m_eglSurface, m_eglContext);
#endif
    }
}

#pragma region Setters

void SGCore::Window::setSize(const int& sizeX, const int& sizeY) noexcept
{
    m_config.m_sizeX = sizeX;
    m_config.m_sizeY = sizeY;

#if SG_PLATFORM_PC
    glfwSetWindowSize(m_handle, sizeX, sizeY);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::setSizeLimits(const int& sizeMinLimitX, const int& sizeMinLimitY, const int& sizeMaxLimitX, const int& sizeMaxLimitY) noexcept
{
    m_config.m_sizeMinLimitX = sizeMinLimitX;
    m_config.m_sizeMinLimitY = sizeMinLimitY;

    m_config.m_sizeMaxLimitX = sizeMaxLimitX;
    m_config.m_sizeMaxLimitY = sizeMaxLimitY;

#if SG_PLATFORM_PC
    glfwSetWindowSizeLimits(m_handle, sizeMinLimitX, sizeMinLimitY, sizeMaxLimitX, sizeMaxLimitY);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::setPosition(const int& posX, const int& posY) noexcept
{
    m_config.m_positionX = posX;
    m_config.m_positionY = posY;

#if SG_PLATFORM_PC
    glfwSetWindowPos(m_handle, posX, posY);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::setTitle(const std::string& title) noexcept
{
    m_config.m_title = title;

#if SG_PLATFORM_PC
    glfwSetWindowTitle(m_handle, title.c_str());
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::setSwapInterval(const bool& swapInterval) noexcept
{
    m_config.m_swapInterval = swapInterval;

    GAPIType apiType = CoreMain::getRenderer()->getGAPIType();
    if(apiType >= GAPIType::SG_API_TYPE_GL4 && apiType <= GAPIType::SG_API_TYPE_GLES3)
    {
#if SG_PLATFORM_PC
        glfwSwapInterval(swapInterval);
#elif SG_PLATFORM_OS_ANDROID
#endif
    }
}

void SGCore::Window::setEnableStickyKeys(const bool& enableStickyKeys) noexcept
{
    m_config.m_enableStickyKeys = enableStickyKeys;

#if SG_PLATFORM_PC
    glfwSetInputMode(m_handle, GLFW_STICKY_KEYS, enableStickyKeys);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::setHideAndCentralizeCursor(const bool& hideAndCentralizeCursor) noexcept
{
    m_config.m_hideAndCentralizeCursor = hideAndCentralizeCursor;

#if SG_PLATFORM_PC
    glfwSetInputMode(m_handle, GLFW_CURSOR, hideAndCentralizeCursor ? GLFW_CURSOR_HIDDEN : GLFW_CURSOR_NORMAL);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

bool SGCore::Window::isHideAndCentralizeCursor() noexcept
{
    return m_config.m_hideAndCentralizeCursor;
}

void SGCore::Window::setConfig(WindowConfig&& other) noexcept
{
    m_config = std::move(other);
}

void SGCore::Window::setShouldClose(const bool& shouldClose) noexcept
{
#if SG_PLATFORM_PC
    glfwSetWindowShouldClose(m_handle, shouldClose);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

#pragma endregion

#pragma region Getters

void SGCore::Window::getPrimaryMonitorSize(int& sizeX, int& sizeY) noexcept
{
#if SG_PLATFORM_PC
    const GLFWvidmode* primaryVideoMode = glfwGetVideoMode(glfwGetPrimaryMonitor());
    sizeX = primaryVideoMode->width;
    sizeY = primaryVideoMode->height;
#elif SG_PLATFORM_OS_ANDROID
    // caching result
    static std::pair<int, int> impl = []() -> std::pair<int, int> {
        Java::JNINativeThreadHandler threadHandler;
        auto* env = Java::JNIManager::getEnv(threadHandler);
        auto* context = Java::JNIManager::getContext();

        if(!env || !context)
        {
            LOG_E(SGCORE_TAG, "Cannot getPrimaryMonitorSize: JNIManager is not initialized.")
            return { 1920, 1080 };
        }

        // getting display metrics
        jclass metricsClass = env->FindClass("android/util/DisplayMetrics");
        if (!metricsClass)
        {
            LOG_E(SGCORE_TAG, "Cannot find DisplayMetrics class");
            return { 1920, 1080 };
        }

        jmethodID constructor = env->GetMethodID(metricsClass, "<init>", "()V");
        jobject metrics = env->NewObject(metricsClass, constructor);

        // getting system service
        jclass contextClass = env->FindClass("android/content/Context");
        jfieldID windowServiceField = env->GetStaticFieldID(contextClass, "WINDOW_SERVICE", "Ljava/lang/String;");
        jstring windowServiceStr = (jstring)env->GetStaticObjectField(contextClass, windowServiceField);

        jmethodID getSystemService = env->GetMethodID(contextClass, "getSystemService", "(Ljava/lang/String;)Ljava/lang/Object;");
        jobject windowManager = env->CallObjectMethod(context, getSystemService, windowServiceStr);

        if (!windowManager)
        {
            LOG_E(SGCORE_TAG, "Cannot get WindowManager");
            return { 1920, 1080 };
        }

        jclass wmClass = env->GetObjectClass(windowManager);
        jmethodID getDefaultDisplay = env->GetMethodID(wmClass, "getDefaultDisplay", "()Landroid/view/Display;");
        jobject display = env->CallObjectMethod(windowManager, getDefaultDisplay);

        jclass displayClass = env->GetObjectClass(display);
        jmethodID getRealMetrics = env->GetMethodID(displayClass, "getRealMetrics", "(Landroid/util/DisplayMetrics;)V");
        env->CallVoidMethod(display, getRealMetrics, metrics);

        jfieldID widthField = env->GetFieldID(metricsClass, "widthPixels", "I");
        jfieldID heightField = env->GetFieldID(metricsClass, "heightPixels", "I");

        int screenWidth = env->GetIntField(metrics, widthField);
        int screenHeight = env->GetIntField(metrics, heightField);

        LOG_I(SGCORE_TAG, "Real screen size: {}x{}", screenWidth, screenHeight);

        // cleanup
        env->DeleteLocalRef(metricsClass);
        env->DeleteLocalRef(metrics);
        env->DeleteLocalRef(contextClass);
        env->DeleteLocalRef(windowServiceStr);
        env->DeleteLocalRef(windowManager);
        env->DeleteLocalRef(wmClass);
        env->DeleteLocalRef(display);
        env->DeleteLocalRef(displayClass);

        return { screenWidth, screenHeight };
    }();

    sizeX = impl.first;
    sizeY = impl.second;
#endif
}

bool SGCore::Window::shouldClose() noexcept
{
#if SG_PLATFORM_PC
    return glfwWindowShouldClose(m_handle);
#elif SG_PLATFORM_OS_ANDROID
    return false;
#endif
}

SGCore::WindowConfig& SGCore::Window::getConfig() noexcept
{
    return m_config;
}

void SGCore::Window::getSize(int& sizeX, int& sizeY) noexcept
{
#if SG_PLATFORM_PC
    glfwGetWindowSize(m_handle, &sizeX, &sizeY);
#elif SG_PLATFORM_OS_ANDROID
    sizeX = ANativeWindow_getWidth(m_handle);
    sizeY = ANativeWindow_getHeight(m_handle);
#endif
}

void SGCore::Window::getBordersSize(int& left, int& top, int& right, int& bottom) noexcept
{
#if SG_PLATFORM_PC
    glfwGetWindowFrameSize(m_handle, &left, &top, &right, &bottom);
#elif SG_PLATFORM_OS_ANDROID

#endif
}

void SGCore::Window::getSizeWithBorders(int& sizeX, int& sizeY) noexcept
{
    getSize(sizeX, sizeY);

    int leftBorder;
    int topBorder;
    int rightBorder;
    int bottomBorder;

    getBordersSize(leftBorder, topBorder, rightBorder, bottomBorder);

    sizeX = sizeX + leftBorder + rightBorder;
    sizeY = sizeY + topBorder + bottomBorder;
}

#pragma endregion

void SGCore::Window::nativeCloseCallback(window_handle window) noexcept
{
#if SG_PLATFORM_PC
    onClose(*((Window*) glfwGetWindowUserPointer(window)));
#elif SG_PLATFORM_OS_ANDROID
#endif

    LOG_I(SGCORE_TAG, "Application window closed.");
}

void SGCore::Window::nativeIconifyCallback(window_handle window, int iconified) noexcept
{
#if SG_PLATFORM_PC
    onIconify(*((Window*) glfwGetWindowUserPointer(window)), iconified);
#elif SG_PLATFORM_OS_ANDROID
#endif

    LOG_I(SGCORE_TAG, "Application window iconified.");
}

void SGCore::Window::nativeFramebufferSizeCallback(window_handle window, int width, int height) noexcept
{
#if SG_PLATFORM_PC
    onFrameBufferSizeChanged(*((Window*) glfwGetWindowUserPointer(window)), width, height);
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::errorCallback(int errCode, const char* err_msg)
{
    LOG_E(SGCORE_TAG, "GLFW error (code {}): {}\n{}", errCode, err_msg, SG_CURRENT_LOCATION_STR);
}

void SGCore::Window::swapBuffers()
{
    auto t0 = Utils::getTimeSecondsAsDouble();

#if SG_PLATFORM_PC
    glfwSwapBuffers(m_handle);
#elif SG_PLATFORM_OS_ANDROID
    eglSwapBuffers(m_eglDisplay, m_eglSurface);
#endif

    auto t1 = Utils::getTimeSecondsAsDouble();

    m_swapBuffersExecutionTime = (t1 - t0) * 1000.0;
}

void SGCore::Window::pollEvents()
{
#if SG_PLATFORM_PC
    glfwPollEvents();
#elif SG_PLATFORM_OS_ANDROID
#endif
}

void SGCore::Window::setFullscreen(bool fullscreen) noexcept
{
    m_config.m_fullsreen = fullscreen;

    int sizeX, sizeY;
    getPrimaryMonitorSize(sizeX, sizeY);

#if SG_PLATFORM_PC
    if(fullscreen)
    {
        glfwSetWindowMonitor(m_handle, glfwGetPrimaryMonitor(), 0, 0, sizeX, sizeY, glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
    }
    else
    {
        glfwSetWindowMonitor(m_handle, nullptr, 0, 0, sizeX, sizeY, glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate);
    }
#elif SG_PLATFORM_OS_ANDROID
#endif
}

bool SGCore::Window::isFullscreen() const noexcept
{
    return m_config.m_fullsreen;
}

double SGCore::Window::getSwapBuffersExecutionTime() const noexcept
{
    return m_swapBuffersExecutionTime;
}

SGCore::Window::window_handle SGCore::Window::getNativeHandle() noexcept
{
    return m_handle;
}

int SGCore::Window::getPrimaryMonitorRefreshRate() noexcept
{
#if SG_PLATFORM_PC
    return glfwGetVideoMode(glfwGetPrimaryMonitor())->refreshRate;
#elif SG_PLATFORM_OS_ANDROID
    Java::JNINativeThreadHandler threadHandler;
    auto* jniEnv = Java::JNIManager::getEnv(threadHandler);
    auto* jniContext = Java::JNIManager::getContext();

    jclass contextClass = jniEnv->GetObjectClass(jniContext);
    jmethodID getSystemServiceFunction = jniEnv->GetMethodID(contextClass, "getSystemService",
                                                             "(Ljava/lang/String;)Ljava/lang/Object;");

    jstring displayServiceStr = jniEnv->NewStringUTF("display");
    jobject displayManager = jniEnv->CallObjectMethod(jniContext, getSystemServiceFunction, displayServiceStr);
    jniEnv->DeleteLocalRef(displayServiceStr);

    jclass displayManagerClass = jniEnv->FindClass("android/hardware/display/DisplayManager");
    jmethodID getDisplayFunction = jniEnv->GetMethodID(displayManagerClass, "getDisplay", "(I)Landroid/view/Display;");

    // getting primary display (0)
    jobject display = jniEnv->CallObjectMethod(displayManager, getDisplayFunction, 0);

    jclass displayClass = jniEnv->FindClass("android/view/Display");
    jmethodID getRefreshRateMethod = jniEnv->GetMethodID(displayClass, "getRefreshRate", "()F");

    const float refreshRate = jniEnv->CallFloatMethod(display, getRefreshRateMethod);

    jniEnv->DeleteLocalRef(displayManager);
    jniEnv->DeleteLocalRef(display);

    return refreshRate;
#endif
}
