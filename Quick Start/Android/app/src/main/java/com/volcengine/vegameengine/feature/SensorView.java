package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.ScrollView;
import android.widget.Toast;

import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

public class SensorView {

    private DialogUtils.DialogWrapper mDialogWrapper;

    public SensorView(Context context, Button button) {
        mDialogWrapper = DialogUtils.wrapper(new TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private static class TestView extends ScrollView {
        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_sensor, this);

            SwitchCompat switchCompatAccelSensor = findViewById(R.id.switch_enable_accelerator);
            switchCompatAccelSensor.setOnCheckedChangeListener((buttonView, isChecked) -> {
                VeGameEngine.getInstance().enableAccelSensor(isChecked);
                if (isChecked) {
                    Toast.makeText(context, "加速度传感器已开启", Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(context, "加速度传感器已关闭", Toast.LENGTH_SHORT).show();
                }
            });

            SwitchCompat switchCompatMagnetic = findViewById(R.id.switch_enable_magnetic);
            switchCompatMagnetic.setOnCheckedChangeListener((buttonView, isChecked) -> {
                VeGameEngine.getInstance().enableMagneticSensor(isChecked);
                if (isChecked) {
                    Toast.makeText(context, "磁力传感器已开启", Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(context, "磁力传感器已关闭", Toast.LENGTH_SHORT).show();
                }
            });

            SwitchCompat switchCompatGravity = findViewById(R.id.switch_enable_gravity);
            switchCompatGravity.setOnCheckedChangeListener((buttonView, isChecked) -> {
                VeGameEngine.getInstance().enableGravitySensor(isChecked);
                if (isChecked) {
                    Toast.makeText(context, "重力传感器已开启", Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(context, "重力传感器已关闭", Toast.LENGTH_SHORT).show();
                }
            });

            SwitchCompat switchCompatOrientation = findViewById(R.id.switch_enable_orientation);
            switchCompatOrientation.setOnCheckedChangeListener((buttonView, isChecked) -> {
                VeGameEngine.getInstance().enableOrientationSensor(isChecked);
                if (isChecked) {
                    Toast.makeText(context, "方向传感器已开启", Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(context, "方向传感器已关闭", Toast.LENGTH_SHORT).show();
                }
            });

            SwitchCompat switchCompatGyroscope = findViewById(R.id.switch_enable_gyroscope);
            switchCompatGyroscope.setOnCheckedChangeListener((buttonView, isChecked) -> {
                VeGameEngine.getInstance().enableGyroscopeSensor(isChecked);
                if (isChecked) {
                    Toast.makeText(context, "陀螺仪已开启", Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(context, "陀螺仪已关闭", Toast.LENGTH_SHORT).show();
                }
            });

            SwitchCompat switchCompatVibrator = findViewById(R.id.switch_enable_vibrator);
            switchCompatVibrator.setOnCheckedChangeListener((buttonView, isChecked) -> {
                VeGameEngine.getInstance().enableVibrator(isChecked);
                if (isChecked) {
                    Toast.makeText(context, "振动已开启", Toast.LENGTH_SHORT).show();
                }
                else {
                    Toast.makeText(context, "振动已关闭", Toast.LENGTH_SHORT).show();
                }
            });
        }
    }


}
