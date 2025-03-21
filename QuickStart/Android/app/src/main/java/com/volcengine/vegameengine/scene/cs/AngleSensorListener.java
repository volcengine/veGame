package com.volcengine.vegameengine.scene.cs;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.util.Log;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IODeviceManager;

public class AngleSensorListener implements SensorEventListener {
    private static final String TAG = "AngleSensorListener";
    static double sensitivityFactor = 50;
    private double timestamp = 0;
    private long lastSendEventTimestamp = 0;
    private double[] angle = new double[2];
    private static final double NS2S = 1.0 / 1_000_000_000.0;
    private int mWidth, mHeight;
    private long sensorDataInterval = 10;
    private double angleThreshold = 0.0008;

    private boolean enableSendEvent = true;

    public void setSize(int width, int height) {
        mWidth = width;
        mHeight = height;
    }

    void pause(){
        enableSendEvent = false;
    }

    void resume(){
        enableSendEvent = true;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();
        if (ioDeviceManager == null) {
            return;
        }
        if (timestamp != 0) {
            final double dT = (sensorEvent.timestamp - timestamp) * NS2S;

            // 使用过滤后的数据
            double deltaX = -sensorEvent.values[0] * dT;
            double deltaY = sensorEvent.values[1] * dT;

            long currentTime = System.currentTimeMillis();
            // 累积
            angle[0] += deltaX;
            angle[1] += deltaY;

            // 控制鼠标事件频率频率
            if (currentTime - lastSendEventTimestamp >= sensorDataInterval) {
                boolean isValidX = Math.abs(angle[0]) > angleThreshold;
                boolean isValidY = Math.abs(angle[1]) > angleThreshold;

                boolean needUploadEvent = true;
                double finalAngleX = angle[0];
                double finalAngleY = angle[1];

                if (!isValidX && !isValidY) {
                    // 都未超过阈值，不发送事件，保证画面稳定
                    // 并且不改变angle数组，保持angle数组的积累
                    needUploadEvent = false;
                } else if (!isValidX) {
                    finalAngleX = 0;
                } else if (!isValidY) {
                    finalAngleY = 0;
                }

                if (needUploadEvent) {
                    // 横屏状态下，只需要考虑Y轴、X轴旋转
                    double distanceX = finalAngleX / 3.1415926; // 水平转动陀螺仪的范围为0~360度，对应0~Width;
                    distanceX *= mWidth;
                    distanceX *= (sensitivityFactor * 0.1);
                    double distanceY = finalAngleY / 3.1415926; // 垂直转动陀螺仪的范围为0~360度，对应0~Height;
                    distanceY *= mHeight;
                    distanceY *= (sensitivityFactor * 0.1);

                    int finalX = (int) distanceX;
                    int finalY = (int) distanceY;

                    if(enableSendEvent) {
                        ioDeviceManager.sendInputMouseMove(finalX, finalY);
                        Log.e(TAG, String.format("the current degree is [%f, %f], [%d, %d], [%f]...", angle[0], angle[1], finalX, finalY, sensitivityFactor));
                        Monitor.monitor();
                    }
                    lastSendEventTimestamp = currentTime;
                    if (isValidX) {
                        angle[0] = 0;
                    }
                    if (isValidY) {
                        angle[1] = 0;
                    }
                }
            }
        } else {
            // 首次接受到旋转事件时，定位光标在屏幕中间
            ioDeviceManager.sendInputCursorPos(0.5f, 0.5f);
        }
        timestamp = sensorEvent.timestamp;
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {
    }

    public void reset() {
        timestamp = 0;
        angle = new double[2];
    }

    public long getSensorDataInterval() {
        return sensorDataInterval;
    }

    public void setSensorDataInterval(long sensorDataInterval) {
        if (sensorDataInterval < 5 || sensorDataInterval > 50) {
            return;
        }
        this.sensorDataInterval = sensorDataInterval;
    }
}
