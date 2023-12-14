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
import android.widget.Toast;

import com.blankj.utilcode.util.CrashUtils;
import com.blankj.utilcode.util.ProcessUtils;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.ICloudCoreManagerStatusListener;

public class InitApplication extends Application {

    private static final String TAG = "InitApplication";

    private static InitApplication instance = null;

    @Override
    public void onCreate() {
        super.onCreate();
        instance = this;
        CrashUtils.init(); //默认日志目录：/storage/emulated/0/Android/data/com.example.sdkdemo/files/crash/

        /**
         * 目前仅支持在主进程中初始化VeGameEngine
         */
        if (ProcessUtils.isMainProcess()) {
            initVeGameEngine();
        }
        else {
            Toast.makeText(this, "请在主进程进行初始化!", Toast.LENGTH_LONG).show();
        }
    }

    public static InitApplication getInstance() {
        return instance;
    }

    private void initVeGameEngine() {
        VeGameEngine gameEngine = VeGameEngine.getInstance();
        /**
         * 请使用prepare()方法来初始化VeGameEngine，init()方法已废弃。
         */
        gameEngine.prepare(this);
        gameEngine.addCloudCoreManagerListener(new ICloudCoreManagerStatusListener() {
            /**
             * 请在onPrepared()回调中监听VeGameEngine的生命周期，onInitialed()回调已废弃
             */
            @Override
            public void onInitialed() {

            }

            @Override
            public void onPrepared() {
                // SDK初始化是一个异步过程，在这个回调中监听初始化完成状态
                AcLog.d(TAG, "onPrepared :" + gameEngine.getStatus());
            }
        });
        VeGameEngine.setDebug(true);
    }
}
