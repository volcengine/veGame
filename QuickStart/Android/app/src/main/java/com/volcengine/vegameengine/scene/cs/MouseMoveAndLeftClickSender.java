package com.volcengine.vegameengine.scene.cs;

import android.util.Log;

import androidx.annotation.NonNull;

import com.volcengine.cloudcore.common.mode.KeySateType;
import com.volcengine.cloudcore.common.mode.MouseKey;
import com.volcengine.cloudphone.apiservice.IODeviceManager;
import com.volcengine.vegameengine.components.rocker.RockerView;

import java.util.Locale;

/**
 * 鼠标滑动与鼠标左键长按事件发送器
 */
public class MouseMoveAndLeftClickSender implements RockerView.OnRockerChangeListener, RockerView.OnRockerLocationListener {
    private final static String TAG = "MouseMoveAndLeftClickSender";
    final static int SENSITIVITY_FACTOR = 10;
    private static final int ROCK_DOWN = 0;
    private static final int ROCK_UP = 1;

    // 最大移动距离，等于屏幕尺寸
    private int maxMoveDistanceX = 0;
    private int maxMoveDistanceY = 0;

    // 当前移动的距离
    private int currentMoveDistanceX = 0;
    private int currentMoveDistanceY = 0;

    // 是否设置了鼠标光标位置
    private boolean hasSetMouseCursorPos = false;

    // 模拟鼠标事件发送
    private final IODeviceManager ioDeviceManager;

    // 移动射击启用时，自动关闭陀螺仪
    private AngleSensorListener angleSensorListener;
    private boolean autoCloseSensorWhenMovingShot;

    public MouseMoveAndLeftClickSender(@NonNull IODeviceManager _ioDeviceManager) {
        ioDeviceManager = _ioDeviceManager;
    }

    public void setAngleSensorListener(AngleSensorListener listener){
        angleSensorListener = listener;
    }

    public void enableAutoCloseSensorWhenMovingShot(boolean enable){
        autoCloseSensorWhenMovingShot = enable;
    }

    public void setMaxMoveSize(int x, int y) {
        if (maxMoveDistanceX != x) {
            maxMoveDistanceX = x;
            currentMoveDistanceX = maxMoveDistanceX / 2;
        }

        if(maxMoveDistanceY != y) {
            maxMoveDistanceY = y;
            currentMoveDistanceY = maxMoveDistanceY / 2;
        }
    }

    @Override
    public void onRock(int action, int direction) {
        if (action == ROCK_DOWN) {
            if(!hasSetMouseCursorPos){
                hasSetMouseCursorPos = true;
                ioDeviceManager.sendInputCursorPos((float) currentMoveDistanceX / (float)(maxMoveDistanceX),
                                                     (float) currentMoveDistanceY / (float)(maxMoveDistanceY));
                ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.DOWN);
                Monitor.monitor();
            }

            if(autoCloseSensorWhenMovingShot
                && angleSensorListener !=null){
                angleSensorListener.pause();
            }

        } else if (action == ROCK_UP) {
            ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.UP);
            Monitor.monitor();
            hasSetMouseCursorPos = false;
            currentMoveDistanceX = 0;
            currentMoveDistanceY = 0;

            if(angleSensorListener !=null){
                angleSensorListener.resume();
            }
        }
    }

    @Override
    public void onRockLocationUpdated(float x, float y, float diskRadius) {

        int deltaX = (int)x - currentMoveDistanceX;
        int deltaY = (int)y - currentMoveDistanceY;

        if (deltaX == 0 && deltaY == 0) {
            // 如果手指未移动则不发送，减少发送数据量，降低data channel压力
            return;
        }

        currentMoveDistanceX = (int)x;
        currentMoveDistanceY = (int)y;

        if(!hasSetMouseCursorPos){
            // 确保touch up后，不再发送move事件
            return;
        }

        ioDeviceManager.sendInputMouseMove((int)(deltaX * SENSITIVITY_FACTOR), (int)(deltaY * SENSITIVITY_FACTOR));
        String log = String.format(Locale.ENGLISH, "deltaX: %d, deltaY: %d, x: %f, y: %f ", deltaX, deltaY, x, y);
        Log.e(TAG, log);
        Monitor.monitor();
    }
}
