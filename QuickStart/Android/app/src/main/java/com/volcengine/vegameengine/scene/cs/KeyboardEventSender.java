package com.volcengine.vegameengine.scene.cs;

import android.util.Log;

import androidx.annotation.IntRange;
import androidx.annotation.NonNull;

import com.volcengine.cloudcore.common.mode.KeyBoardKey;
import com.volcengine.cloudcore.common.mode.KeySateType;
import com.volcengine.cloudphone.apiservice.IODeviceManager;
import com.volcengine.vegameengine.components.rocker.Constant;
import com.volcengine.vegameengine.components.rocker.RockerView;

/**
 * W、A、S、D 4个按键单独或组合发送的发送器
 */
public class KeyboardEventSender implements RockerView.OnRockerChangeListener {
    private static final String TAG = "KeyboardEventSender";
    private static final int ROCK_DOWN = 0;
    private static final int ROCK_UP = 1;
    private static final int A_KEY = 0;
    private static final int W_KEY = 1;
    private static final int S_KEY = 2;
    private static final int D_KEY = 3;
    private static final int ALL_KEY = 4;

    // 模拟键盘发送
    private final IODeviceManager ioDeviceManager;
 
    // A、W、S、D4个键是否被按下
    private final boolean[] currentKeyDownStates = new boolean[ALL_KEY];

    public KeyboardEventSender(@NonNull IODeviceManager _ioDeviceManager) {
        ioDeviceManager = _ioDeviceManager;
    }

    @Override
    public void onRock(int action, int direction) {
        String actionName = action == ROCK_DOWN ? "ROCK_DOWN" : "ROCK_UP";
        if(direction != Constant.DIRECTION_NONE) {
            Log.e(TAG, "the rock action is " + actionName + "; the direction is " + getDirectionNameByDirection(direction));
        }
        if (action == ROCK_DOWN) {
            sendKeyboardDownEvent(direction);
        } else if(action == ROCK_UP){
            sendAllKeyboardUp();
        }
    }

    private void sendKeyboardDownEvent(int direction) {
        if (direction == Constant.DIRECTION_NONE) {
            sendAllKeyboardUp();
        } else if (direction == Constant.DIRECTION_UP) {
            // 发送W键的按下事件
            sendKeyboardDown(W_KEY);

            // 除了W键，其他键都UP
            sendAllKeyboardUp(W_KEY);
        } else if(direction == Constant.DIRECTION_RIGHT_UP){
            // 发送W + D键的按下事件
            sendKeyboardDown(W_KEY);
            sendKeyboardDown(D_KEY);

            // 除了W键与D键，其他键都UP
            sendAllKeyboardUp(W_KEY, D_KEY);
        } else if(direction == Constant.DIRECTION_RIGHT){
            // 发送D键的按下事件
            sendKeyboardDown(D_KEY);

            // 除了D键，其他键都UP
            sendAllKeyboardUp(D_KEY);
        } else if(direction == Constant.DIRECTION_RIGHT_DOWN){
            // 发送D + S键的按下事件
            sendKeyboardDown(D_KEY);
            sendKeyboardDown(S_KEY);

            // 除了D键与S键，其他键都UP
            sendAllKeyboardUp(D_KEY, S_KEY);
        } else if(direction == Constant.DIRECTION_DOWN){
            // 发送S键的按下事件
            sendKeyboardDown(S_KEY);

            // 除了S键，其他键都UP
            sendAllKeyboardUp(S_KEY);
        } else if(direction == Constant.DIRECTION_LEFT_DOWN){
            // 发送S + A键的按下事件
            sendKeyboardDown(S_KEY);
            sendKeyboardDown(A_KEY);

            // 除了S键与A键，其他键都UP
            sendAllKeyboardUp(S_KEY, A_KEY);
        } else if(direction == Constant.DIRECTION_LEFT){
            // 发送A键的按下事件
            sendKeyboardDown(A_KEY);

            // 除了A键，其他键都UP
            sendAllKeyboardUp(A_KEY);
        } else if(direction == Constant.DIRECTION_LEFT_UP){
            // 发送W + A键的按下事件
            sendKeyboardDown(W_KEY);
            sendKeyboardDown(A_KEY);

            // 除了S键与A键，其他键都UP
            sendAllKeyboardUp(W_KEY, A_KEY);
        }
    }

    private void sendKeyboardDown(@IntRange(from = A_KEY, to = D_KEY) int key){
        if (currentKeyDownStates[key]) {
            return;
        }
        sendKeyboardEvent(key, KeySateType.DOWN);
        currentKeyDownStates[key] = true;
    }

    private void sendAllKeyboardUp(int... excludeKeys) {
        for (int i = 0; i < ALL_KEY; i++) {
            if (!currentKeyDownStates[i]) {
                continue;
            }
            boolean isSkip = false;

            if (excludeKeys != null) {
                for (int excludeKey : excludeKeys) {
                    if (i == excludeKey) {
                        isSkip = true;
                        break;
                    }
                }
            }

            if (isSkip) {
                continue;
            }

            sendKeyboardEvent(i, KeySateType.UP);
            currentKeyDownStates[i] = false;
        }
    }

    private void sendKeyboardEvent(@IntRange(from = A_KEY, to = D_KEY) int key,
                                   @KeySateType int keyState) {
        Log.e(TAG, "send the keyboard event: " + getKeyName(key) + "; key state is " + getKeyState(keyState));
        switch (key) {
            case A_KEY:
                ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyA, keyState);
                Monitor.monitor();
                break;
            case W_KEY:
                ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyW, keyState);
                Monitor.monitor();
                break;
            case S_KEY:
                ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyS, keyState);
                Monitor.monitor();
                break;
            case D_KEY:
                ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyD, keyState);
                Monitor.monitor();
                break;
        }
    }

    private static String getKeyName(@IntRange(from = A_KEY, to = D_KEY) int key){
        if(key == A_KEY){
            return "A";
        } else if(key == W_KEY){
            return "W";
        } else if(key == D_KEY){
            return "D";
        } else if(key == S_KEY){
            return "S";
        } else {
            return "NULL";
        }
    }

    private static String getKeyState(@KeySateType int keyState){
        if(keyState == KeySateType.DOWN){
            return "Down";
        }else if(keyState == KeySateType.UP){
            return "Up";
        }else {
            return "UnKnow";
        }
    }

    private static String getDirectionNameByDirection(int direction) {
        if (direction == Constant.DIRECTION_RIGHT_UP) {
            return "DIRECTION_RIGHT_UP";
        } else if (direction == Constant.DIRECTION_RIGHT) {
            return "DIRECTION_RIGHT";
        } else if (direction == Constant.DIRECTION_RIGHT_DOWN) {
            return "DIRECTION_RIGHT_DOWN";
        } else if (direction == Constant.DIRECTION_DOWN) {
            return "DIRECTION_DOWN";
        } else if (direction == Constant.DIRECTION_LEFT_DOWN) {
            return "DIRECTION_LEFT_DOWN";
        } else if (direction == Constant.DIRECTION_LEFT) {
            return "DIRECTION_LEFT";
        } else if (direction == Constant.DIRECTION_LEFT_UP) {
            return "DIRECTION_LEFT_UP";
        } else if (direction == Constant.DIRECTION_UP) {
            return "DIRECTION_UP";
        } else {
            return "DIRECTION_NONE";
        }
    }
}
