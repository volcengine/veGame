/**
 * Copyright (c) 2022 Volcengine
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
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

import androidx.annotation.NonNull;

import com.blankj.utilcode.util.CrashUtils;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.plugin.CpuABIType;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.ICloudCoreManagerStatusListener;
import com.volcengine.cloudphone.apiservice.outinterface.InitListener;

import org.jetbrains.annotations.NotNull;

public class InitApplication extends Application {

    private static final String TAG = "TAG_INIT";

    @Override
    public void onCreate() {
        super.onCreate();
        CrashUtils.init(); //默认日志目录：/storage/emulated/0/Android/data/com.example.sdkdemo/files/crash/
        VeGameEngine.setDebug(true);
        VeGameEngine.setLogger(new AcLog.ILogger() {
            @Override
            public void onVerbose(String s, String s1) {
                Log.v(s, s1);
            }

            @Override
            public void onDebug(String s, String s1) {
                Log.d(s, s1);
            }

            @Override
            public void onInfo(String s, String s1) {
                Log.i(s, s1);
            }

            @Override
            public void onWarn(String s, String s1) {
                Log.w(s, s1);
            }

            @Override
            public void onError(String s, String s1) {
                Log.e(s, s1);
            }

            @Override
            public void onError(String s, String s1, Throwable throwable) {
                Log.e(s, s1);
            }
        });
        VeGameEngine cloudPhoneManager = VeGameEngine.getInstance();
        VeGameEngine.getInstance().addCloudCoreManagerListener(new ICloudCoreManagerStatusListener() {
            @Override
            public void onInitialed() {
                AcLog.d(TAG, "onInitialed :" + cloudPhoneManager.getStatus());
            }

            @Override
            public void onStarted() {
                AcLog.d(TAG, "onStarted : " + cloudPhoneManager.getStatus());
            }

            @Override
            public void onPaused() {
                AcLog.d(TAG, "onPaused : " + cloudPhoneManager.getStatus());
            }

            @Override
            public void onResumed() {
                AcLog.d(TAG, "onResumed : " + cloudPhoneManager.getStatus());
            }

            @Override
            public void onStopped(boolean b) {
                AcLog.d(TAG, "onStopped : " + cloudPhoneManager.getStatus());
            }

            @Override
            public void onEnd() {
                AcLog.d(TAG, "onEnd : " + cloudPhoneManager.getStatus());
            }
        });
        VeGameEngine.getInstance().init(this, new GsonConverter(), CpuABIType.ABI_armeabi_v7a, new InitListener() {
            @Override
            public void initSuccess() {
                Log.d(TAG, "init success");
                Log.d(TAG, "serviceDeviceId:" + VeGameEngine.getInstance().getServiceDeviceId());
            }

            @Override
            public void initFail(int i, @NonNull @NotNull String s) {
                Log.d(TAG, "init fail" + s);
            }
        });
        Log.d(TAG, "deviceId" + VeGameEngine.getInstance().getDeviceId());
    }
}
