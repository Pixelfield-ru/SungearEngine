package com.pixelfield.sungearstarter;

import android.content.Context;
import android.view.Surface;

public class NativeMethods {
    static native void startSGCore(Context context, Surface surfaceView);
}
