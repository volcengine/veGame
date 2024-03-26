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
    private float[] angle = new float[3];
    private static final float NS2S = 1.0f / 1000000000.0f;
    private boolean isFirstTime = true;
    private int startX, startY;
    private int mWidth, mHeight;

    public void setSize(int width, int height){
        mWidth = width;
        mHeight = height;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {
        IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();
        if(ioDeviceManager == null){
            return;
        }
        if (timestamp != 0) {
            final float dT = (sensorEvent.timestamp - timestamp) * NS2S;
            angle[0] += sensorEvent.values[0] * dT;
            angle[1] += sensorEvent.values[1] * dT;
            angle[2] += sensorEvent.values[2] * dT;

            float angleX = (float) Math.toDegrees(angle[0]);
            float angleY = (float) Math.toDegrees(angle[1]);
            float angleZ = (float) Math.toDegrees(angle[2]);

            // 横屏状态下，只需要考虑Y轴、X轴旋转
            float deltaY = angleY / 180.0f; // 垂直转动陀螺仪的范围为0~180度，对应0~Height
            float deltaX = angleX / 360.0f; // 水平转动陀螺仪的范围为0~360度，对应0~Width;
            int finalX = (int) (deltaX * mWidth);
            int finalY = (int) (deltaY * mHeight);

            if(isFirstTime){
                startX = finalX;
                startY = finalY;
                isFirstTime = false;
            } else {
                ioDeviceManager.sendInputMouseMove(-(finalX - startX), finalY - startY);
                startX = finalX;
                startY = finalY;
                Log.e(TAG, String.format("the current degree is [%d, %d]...", -(finalX - startX), finalY - startY));
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
}
