package com.pixelfield.sungearstarter;

import android.content.Context;
import android.content.res.AssetManager;
import android.os.Build;
import android.util.Log;

import java.io.File;
import java.io.FileNotFoundException;
import java.io.FileOutputStream;
import java.io.InputStream;
import java.io.OutputStream;
import java.nio.file.Files;

public class Common {
    public static String getDeviceArchitecture() {
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            String[] abis = Build.SUPPORTED_ABIS;
            if (abis != null && abis.length > 0) {
                return abis[0];
            }
        }

        // for old Android versions
        return Build.CPU_ABI;
    }

    public static void copyAssets(AssetManager assetManager, String sourcePath, String targetPath) {
        String[] assets;

        try {
            assets = assetManager.list(sourcePath);
            if(assets == null) {
                throw new RuntimeException("Can not get assets of application.");
            }

            if(assets.length == 0) {
                return;
            }

            File targetDir = new File(targetPath);
            if(!targetDir.exists() && !targetDir.mkdirs()) {
                throw new RuntimeException("Can not create directory to copy assets in. Path: " + targetPath);
            }

            for(String asset : assets) {
                Log.i("SungearStarter", "Has asset: " + asset);

                final String currentSourcePath = sourcePath.isEmpty() ? asset : sourcePath + File.separator + asset;
                final String currentTargetPath = targetPath + File.separator + asset;

                final String[] childAssets = assetManager.list(currentSourcePath);
                if(childAssets != null && childAssets.length > 0) {
                    copyAssets(assetManager, currentSourcePath, currentTargetPath);
                } else {
                    copyAsset(assetManager, currentSourcePath, currentTargetPath);
                }
            }
        } catch(Exception e) {
            Log.e("SungearStarter", Log.getStackTraceString(e));
        }
    }

    public static void copyAsset(AssetManager assetManager, String sourcePath, String targetPath) {
        InputStream in = null;
        OutputStream out = null;

        try {
            File targetFile = new File(targetPath);
            File parentDir = targetFile.getParentFile();

            if(parentDir != null && !parentDir.exists() && !parentDir.mkdirs()) {
                return;
            }

            in = assetManager.open(sourcePath);
            out = new FileOutputStream(targetPath);

            byte[] buffer = new byte[1024];
            int length;
            while((length = in.read(buffer)) > 0) {
                out.write(buffer, 0, length);
            }

            Log.i("SungearStarter", "Copied file from assets: " + sourcePath);
        } catch(Exception e) {
            Log.e("SungearStarter", Log.getStackTraceString(e));
        } finally {
            try {
                if(in != null) {
                    in.close();
                }
                if(out != null) {
                    out.close();
                }
            } catch(Exception e) {
                Log.e("SungearStarter", Log.getStackTraceString(e));
            }
        }
    }
}
