package com.volcengine.vegameengine.components.sensor;

import android.content.Context;
import android.hardware.Sensor;
import android.hardware.SensorEvent;
import android.hardware.SensorEventListener;
import android.hardware.SensorManager;
import android.util.Log;

import androidx.annotation.NonNull;

public class SensorController implements SensorEventListener {
    private static final String TAG = "SensorController";

    private SensorManager mSensorManager;

    private Sensor mGyroscope = null;

    private SensorEventListener mOutListener;

    public void setSensorEventListener(SensorEventListener listener){
        mOutListener = listener;
    }

    public void startSensor(@NonNull Context context){
        if(mGyroscope != null){
            return;
        }
        mSensorManager = (SensorManager) context.getSystemService(Context.SENSOR_SERVICE);
        if (mSensorManager == null) {
            Log.e(TAG, "failed to fetch the system sensor manager...");
            return;
        }

        mGyroscope = mSensorManager.getDefaultSensor(Sensor.TYPE_GYROSCOPE);

        mSensorManager.registerListener(this, mGyroscope, SensorManager.SENSOR_DELAY_UI);
    }

    public void stopSensor(){
        if(mGyroscope == null){
            return;
        }

        mSensorManager.unregisterListener(this);
        mGyroscope = null;
    }

    @Override
    public void onSensorChanged(SensorEvent sensorEvent) {

        if(mOutListener != null
            && sensorEvent.sensor.getType() == Sensor.TYPE_GYROSCOPE){
            mOutListener.onSensorChanged(sensorEvent);
        }
    }

    @Override
    public void onAccuracyChanged(Sensor sensor, int i) {

    }
}
