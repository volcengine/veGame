package com.volcengine.vegameengine.base;

import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;

import androidx.appcompat.app.AppCompatActivity;

public class BasePlayActivity extends AppCompatActivity {

    /**
     * 调整Activity的显示方向
     *
     * @param rotation 旋转方向
     *                 0/180  -- 将Activity调整为竖屏显示
     *                 90/270 -- 将Activity调整为横屏显示
     */
    protected void setRotation(int rotation) {
        switch (rotation) {
            case 0:
            case 180:
                setRequestedOrientation(SCREEN_ORIENTATION_SENSOR_PORTRAIT);
                break;
            case 90:
            case 270:
                setRequestedOrientation(SCREEN_ORIENTATION_SENSOR_LANDSCAPE);
                break;
        }
    }

}
