package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.widget.LinearLayout;
import android.widget.ScrollView;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.vegameengine.R;

public class SensorView extends ScrollView {

    private boolean isOpenAcc = true;
    private boolean isOpenMagnetic = true;
    private boolean isOpenVibrator = true;
    private boolean isOpenOrientationSensor = true;
    private boolean isOpenGravity = true;
    private boolean isOpenGyroscopeSensor = true;

    public SensorView(Context context) {
        super(context);
//        setOrientation(VERTICAL);
        inflate(context, R.layout.dialog_sensor, this);

        findViewById(R.id.btn_acc).setOnClickListener(v -> {
            isOpenAcc = !isOpenAcc;
            VeGameEngine.getInstance().enableAccelSensor(isOpenAcc);
        });

        findViewById(R.id.btn_magnetic).setOnClickListener(v -> {
            isOpenMagnetic = !isOpenMagnetic;
            VeGameEngine.getInstance().enableMagneticSensor(isOpenMagnetic);
        });

        findViewById(R.id.btn_gravity).setOnClickListener(v -> {
            isOpenGravity = !isOpenGravity;
            VeGameEngine.getInstance().enableGravitySensor(isOpenGravity);
        });

        findViewById(R.id.btn_orientation_sensor).setOnClickListener(v -> {
            isOpenOrientationSensor = !isOpenOrientationSensor;
            VeGameEngine.getInstance().enableOrientationSensor(isOpenOrientationSensor);
        });

        findViewById(R.id.btn_gyroscope).setOnClickListener(v -> {
            isOpenGyroscopeSensor = !isOpenGyroscopeSensor;
            VeGameEngine.getInstance().enableGyroscopeSensor(isOpenGyroscopeSensor);
        });

        findViewById(R.id.btn_vibrator).setOnClickListener(v -> {
            isOpenVibrator = !isOpenVibrator;
            VeGameEngine.getInstance().enableVibrator(isOpenVibrator);
        });
    }
}
