package com.volcengine.vegameengine.scene.cs;

import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.util.Log;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IODeviceManager;

public class AngleSensorListener implements SensorEventListener {
    private static final String TAG = "AngleSensorListener";
    private float timestamp = 0;
    private long lastSendEventTimestamp = 0;
    private float[] angle = new float[3];
    private static final float NS2S = 1.0f / 1_000_000_000.0f;
    private boolean isFirstTime = true;
    private int startX, startY;
    private int mWidth, mHeight;
    private float radiusThreshold;
    private int smoothCount;

    private long sensorDataInterval = 49;

    public void setSize(int width, int height){
        mWidth = width;
        mHeight = height;
        radiusThreshold = (float) Math.toRadians(0.1);
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();
        if(ioDeviceManager == null){
            return;
        }
        if (timestamp != 0) {
            final float dT = (sensorEvent.timestamp - timestamp) * NS2S;
            float deltaX = sensorEvent.values[0] * dT;
            float deltaY = sensorEvent.values[1] * dT;
            float deltaZ = sensorEvent.values[2] * dT;

            if(Math.abs(deltaX) > radiusThreshold) {
                smoothCount++;
                angle[0] += deltaX;
            }
            if(Math.abs(deltaY) > radiusThreshold) {
                smoothCount++;
                angle[1] += deltaY;
            }
            if(Math.abs(deltaZ) > radiusThreshold) {
                angle[2] += deltaZ;
            }

            float angleX = (float) Math.toDegrees(angle[0]);
            float angleY = (float) Math.toDegrees(angle[1]);
            float angleZ = (float) Math.toDegrees(angle[2]);

            // 横屏状态下，只需要考虑Y轴、X轴旋转
            deltaY = angleY / 360.0f; // 垂直转动陀螺仪的范围为0~180度，对应0~Height
            deltaX = -(angleX / 360.0f); // 水平转动陀螺仪的范围为0~360度，对应0~Width;

            int sensitivity = SensitivityManager.getCurrentSensitivity();
            int finalX = (int) (deltaX * mWidth) * sensitivity;
            int finalY = (int) (deltaY * mHeight) * sensitivity;

            if(isFirstTime){
                startX = finalX;
                startY = finalY;
                isFirstTime = false;
            } else {
                long currentTime = System.currentTimeMillis();
                if(currentTime - lastSendEventTimestamp >= sensorDataInterval) {
                    // 控制频率，最大不超100次/秒
                    ioDeviceManager.sendInputMouseMove(finalX - startX, finalY - startY);
                    lastSendEventTimestamp = currentTime;
                    Log.e(TAG, String.format("the current degree is [%d, %d], [%d, %d], [%d]...", finalX - startX, finalY - startY, finalX, finalY, sensitivity));
                    if(smoothCount > 0) {
                        Log.e("TAG", "the smooth count is " + smoothCount);
                    }
                    smoothCount = 0;
                    startX = finalX;
                    startY = finalY;
                    Monitor.monitor();
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
        angle = new float[3];
        isFirstTime = true;
    }

    public long getSensorDataInterval() {
        return sensorDataInterval;
    }

    public void setSensorDataInterval(long sensorDataInterval) {
        if(sensorDataInterval < 5 || sensorDataInterval > 50){
            return;
        }
        this.sensorDataInterval = sensorDataInterval;
    }
}
