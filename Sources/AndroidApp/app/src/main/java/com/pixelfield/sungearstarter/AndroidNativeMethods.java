package com.pixelfield.sungearstarter;

import android.content.Context;
import android.view.Surface;

public class AndroidNativeMethods {
    static native void startCore(Context context, Surface surfaceView);

    // todo: make java sungear core api
    static native void startMainCycle();

    static native void recreateWindow(Surface surfaceView);

    // todo: make java sungear core api
    static native void loadConfig(String configPath);
}
