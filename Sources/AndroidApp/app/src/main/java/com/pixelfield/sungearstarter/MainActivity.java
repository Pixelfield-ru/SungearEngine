package com.pixelfield.sungearstarter;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.content.pm.ActivityInfo;
import android.content.res.Configuration;
import android.graphics.PixelFormat;
import android.opengl.EGL14;
import android.opengl.EGL15;
import android.opengl.EGLConfig;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

import java.io.File;
import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Paths;
import java.util.Comparator;
import java.util.Objects;

/**
 * An example full-screen activity that shows and hides the system UI (i.e.
 * status bar and navigation/system bar) with user interaction.
 */
public class MainActivity extends Activity implements SurfaceHolder.Callback {
    private SurfaceView surfaceView;
    private static boolean isCoreInitialized = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);

        surfaceView = new SurfaceView(this);
        surfaceView.getHolder().addCallback(this);
        setContentView(surfaceView);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void surfaceChanged(@NonNull SurfaceHolder holder, int format, int width, int height) {
    }

    @SuppressLint("UnsafeDynamicallyLoadedCode")
    @Override
    public void surfaceCreated(@NonNull SurfaceHolder holder) {
        // setRequestedOrientation(ActivityInfo.SCREEN_ORIENTATION_LANDSCAPE);

        // surface changed
        if(isCoreInitialized)
        {
            AndroidNativeMethods.onAppInstanceRestore(getApplicationContext(), holder.getSurface());
            // AndroidNativeMethods.recreateWindow(holder.getSurface());
            return;
        }

        final String externalStorage = Objects.requireNonNull(getExternalFilesDir(null)).getAbsolutePath();
        final String internalStorage = Objects.requireNonNull(getFilesDir()).getAbsolutePath();

        final var appInfo = getApplicationInfo();

        Log.i("SungearStarter", "External Files dir: " + externalStorage + "/assets/");
        Log.i("SungearStarter", "Internal Files dir: " + internalStorage + "/assets/");
        Common.copyAssets(this.getAssets(), "sungear", internalStorage + "/assets/sungear");

        /*if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.O) {
            try {
                Files.walk(Paths.get(externalStorage + "/logs"))
                        .sorted(Comparator.reverseOrder()) // удаляем сначала файлы, потом папки
                        .forEach(p -> {
                            try {
                                Files.delete(p);
                                Log.i("SungearStarter", "Удалено: " + p);
                            } catch (IOException e) {
                                Log.i("SungearStarter", "Ошибка удаления: " + p + " - " + e.getMessage());
                            }
                        });
            } catch (IOException e) {
                throw new RuntimeException(e);
            }
        }*/

        Log.d("SungearStarter", "Loading SGCore.so...");
        System.load(appInfo.sourceDir  + "!/lib/" + Common.getDeviceArchitecture() + "/SGCore.so");

        /*AndroidNativeMethods.startCore(this.getApplicationContext(), holder.getSurface());
        AndroidNativeMethods.loadConfig(internalStorage + "/assets/sungear/SungearEngineConfig.json");
        AndroidNativeMethods.startMainCycle();*/

        new Thread(() -> {
            AndroidNativeMethods.startCore(this.getApplicationContext(), holder.getSurface());
            AndroidNativeMethods.loadConfig(internalStorage + "/assets/sungear/SungearEngineConfig.json");
            AndroidNativeMethods.startMainCycle();
            Log.i("SungearStarter", "kekeekekeke");
        }).start();

        isCoreInitialized = true;
    }

    @Override
    public void surfaceDestroyed(@NonNull SurfaceHolder holder) {

    }
}