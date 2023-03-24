package com.volcengine.vegameengine.util;

import android.content.Context;
import android.content.res.AssetManager;
import android.widget.Toast;

import com.blankj.utilcode.util.ToastUtils;

import java.io.ByteArrayOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.nio.charset.Charset;

/**
 * @date 2022/12/27
 * @desc
 */
public class AssetsUtil {
    private static final String ENCODING = "UTF-8";

    public static InputStream getFileFromAssets(Context context, String fileName) throws IOException {
        AssetManager am = context.getAssets();
        return am.open(fileName);
    }

    public static String getTextFromAssets(Context context, String fileName) {
        String result = null;
        try {
            InputStream in = getFileFromAssets(context, fileName);
            int length = in.available();
            byte[] buffer = new byte[length];
            in.read(buffer);
            result = new String(buffer, Charset.forName(ENCODING));
        } catch (IOException e) {
            e.printStackTrace();
        }
        return result;
    }

    public static boolean isAssetsFileExists(Context context, String fileName) throws IOException {
        AssetManager am = context.getAssets();
        String[] files = am.list("");
        boolean isFileExists = false;
        for (String file : files) {
            if (file.equals(fileName)) {
                isFileExists = true;
                break;
            }
        }
        return isFileExists;
    }
}