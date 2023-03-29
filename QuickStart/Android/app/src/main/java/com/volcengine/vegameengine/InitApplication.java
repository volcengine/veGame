/**
 * Copyright (c) 2022 Volcengine
 * <p>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p>
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * <p>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.volcengine.vegameengine;

import android.app.Application;
import android.util.Log;

import com.blankj.utilcode.util.CrashUtils;
import com.blankj.utilcode.util.ProcessUtils;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.ICloudCoreManagerStatusListener;

public class InitApplication extends Application {

    private static final String TAG = "TAG_INIT";

    private static InitApplication instance = null;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        CrashUtils.init(); //默认日志目录：/storage/emulated/0/Android/data/com.example.sdkdemo/files/crash/

        if (ProcessUtils.isMainProcess()) {
            VeGameEngine gameEngine = VeGameEngine.getInstance();
            gameEngine.init(this);
            gameEngine.addCloudCoreManagerListener(new ICloudCoreManagerStatusListener() {
                @Override
                public void onInitialed() {
                    AcLog.d(TAG, "onInitialed :" + gameEngine.getStatus());
                }
            });
            VeGameEngine.setDebug(true);
            VeGameEngine.setLogger(new AcLog.ILogger() {
                @Override
                public void onVerbose(String TAG, String message) {
                    Log.v(TAG, message);
                }

                @Override
                public void onDebug(String TAG, String message) {
                    Log.d(TAG, message);
                }

                @Override
                public void onInfo(String TAG, String message) {
                    Log.i(TAG, message);
                }

                @Override
                public void onWarn(String TAG, String message) {
                    Log.w(TAG, message);
                }

                @Override
                public void onError(String TAG, String message) {
                    Log.e(TAG, message);
                }

                @Override
                public void onError(String TAG, String message, Throwable throwable) {
                    Log.e(TAG, message);
                    if (throwable != null) {
                        throwable.printStackTrace();
                    }
                }
            });

            Log.d(TAG, "deviceId" + gameEngine.getDeviceId());
        }
    }

    public static InitApplication getInstance() {
        return instance;
    }
}
