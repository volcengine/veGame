package com.volcengine.vegameengine.scene.cs;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.CompoundButton;
import android.widget.FrameLayout;
import android.widget.SeekBar;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.androidcloud.common.model.BriefTouchEvent;
import com.volcengine.cloudcore.common.mode.KeyBoardKey;
import com.volcengine.cloudcore.common.mode.KeySateType;
import com.volcengine.cloudcore.common.mode.MouseKey;
import com.volcengine.cloudcore.common.mode.QueueInfo;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IODeviceManager;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.base.BasePlayActivity;
import com.volcengine.vegameengine.components.rocker.RockerView;
import com.volcengine.vegameengine.components.sensor.SensorController;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.Map;

// 轮盘交互 + 陀螺仪交互 + 连射UI
// 其中，陀螺仪交互通过隐藏按钮的方式禁用
public class CSDemoActivity extends BasePlayActivity implements IGamePlayerListener, Monitor.MonitorListener {
    private final String TAG = "CSDemoActivity";
    private FrameLayout mContainer;
    private RockerView mWSADRockView;
    private TextView mOperationTip;
    private SensorController mSensorController;
    private final AngleSensorListener mAngleSensorListener = new AngleSensorListener();
    private MouseMoveAndLeftClickSender mouseMoveAndLeftClickSender;
    // 左射击按钮，点击触发鼠标左键点击
    private View mLeftShotButton;
    // 右射击按钮，点击触发鼠标左键点击，长按滑动Rocker对应左键长按 + 鼠标滑动
    private RockerView mRightShotRockerView;
    // 右瞄准按钮，点击后，鼠标右键长按，再次点击，取消鼠标右键长按，
    private View mRightAimButton;
    boolean isMouseRightLongPress = false;
    // 右射击按钮映射屏幕旋转敏感度的seek bar
    private SeekBar mMouseMoveSensitivityController;
    // 跳跃按钮
    private TextView jumpButton;
    // 换弹按钮
    private TextView replaceBulletButton;
    // 检视按钮
    private TextView reviewButton;
    // 切换按钮
    private TextView switchButton;
    // B键按钮
    private TextView _BButton;
    // Esc按钮
    private TextView escButton;
    // 鼠标事件每秒发送个数统计视图
    private TextView monitorView;
    // 传感器速率提示View
//    private TextView sensorSpeedTip1, sensorSpeedTip2;
    // 传感器速率调整seek bar
//    private SeekBar sensorSpeedSeekBar;
    // 移动射击时自动关闭陀螺仪提示View
    private TextView autoCloseSensorWhenMovingShotTextView;
    // 移动射击时自动关闭陀螺仪开关
    private SwitchCompat autoCloseSensorWhenMovingShotSwitch;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_cs_demo);
        initView();
        initGamePlayConfig();
    }

    @SuppressLint("ClickableViewAccessibility")
    private void initView() {
        mContainer = findViewById(R.id.container);
        SwitchCompat mSwShowOrHide = findViewById(R.id.sw_show_or_hide);
        mOperationTip = findViewById(R.id.operation_tip);
        mWSADRockView = findViewById(R.id.w_a_s_d_rocker);
        mLeftShotButton = findViewById(R.id.left_shot_button);
        mRightShotRockerView = findViewById(R.id.right_shot_rocker);
        mRightAimButton = findViewById(R.id.right_aim_button);
        mMouseMoveSensitivityController = findViewById(R.id.mouse_move_speed_seek);
        jumpButton = findViewById(R.id.jump_button);
        replaceBulletButton = findViewById(R.id.replace_bullet_button);
        switchButton = findViewById(R.id.switch_button);
        reviewButton = findViewById(R.id.review_button);
        _BButton = findViewById(R.id.key_B_button);
        escButton = findViewById(R.id.key_Esc_button);
        monitorView = findViewById(R.id.mouse_event_monitor_tip);
//        sensorSpeedTip1 = findViewById(R.id.sensor_to_mouse_move_num_speed_tip);
//        sensorSpeedTip2 = findViewById(R.id.sensor_to_mouse_move_num_speed_tip_2);
//        sensorSpeedSeekBar = findViewById(R.id.sensor_to_mouse_move_num_speed);
        autoCloseSensorWhenMovingShotTextView = findViewById(R.id.auto_close_sensor_when_moving_shot_text);
        autoCloseSensorWhenMovingShotSwitch = findViewById(R.id.auto_close_sensor_when_moving_shot);
        autoCloseSensorWhenMovingShotSwitch.setOnCheckedChangeListener((buttonView, isChecked) -> {
            if(mouseMoveAndLeftClickSender != null) {
                mouseMoveAndLeftClickSender.enableAutoCloseSensorWhenMovingShot(isChecked);
            }
        });
        Monitor.listener = this;

//        sensorSpeedSeekBar.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
//            @Override
//            public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
//
//            }
//
//            @Override
//            public void onStartTrackingTouch(SeekBar seekBar) {
//
//            }
//
//            @Override
//            public void onStopTrackingTouch(SeekBar seekBar) {
//                int currentSensitivity = seekBar.getProgress();
//                mAngleSensorListener.setSensorDataInterval((int)currentSensitivity);
//            }
//        });

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mOperationTip.setVisibility(isChecked ? View.VISIBLE : View.GONE);
        });

        SwitchCompat swSensorControlMouseMove = findViewById(R.id.sw_sensor_control_mouse_move);
        swSensorControlMouseMove.setOnCheckedChangeListener((compoundButton, isChecked) -> {
            enableSensorControlMouseMove(isChecked);
        });
        boolean currentEnable = swSensorControlMouseMove.isChecked();
        enableSensorControlMouseMove(currentEnable);
    }

    private void enableSensorControlMouseMove(boolean enable){
        if(enable){
            // 启用陀螺仪控制画面视角的功能
            startSensor();

            autoCloseSensorWhenMovingShotTextView.setVisibility(View.VISIBLE);
            autoCloseSensorWhenMovingShotSwitch.setVisibility(View.VISIBLE);
            autoCloseSensorWhenMovingShotSwitch.setChecked(false);

//            sensorSpeedTip1.setVisibility(View.VISIBLE);
//            sensorSpeedTip2.setVisibility(View.VISIBLE);
//            sensorSpeedSeekBar.setVisibility(View.VISIBLE);
//            sensorSpeedSeekBar.setProgress((int)mAngleSensorListener.getSensorDataInterval());
        } else {
            // 禁止陀螺仪控制画面视角的功能
            stopSensor();

            autoCloseSensorWhenMovingShotTextView.setVisibility(View.GONE);
            autoCloseSensorWhenMovingShotSwitch.setVisibility(View.GONE);

//            sensorSpeedTip1.setVisibility(View.GONE);
//            sensorSpeedTip2.setVisibility(View.GONE);
//            sensorSpeedSeekBar.setVisibility(View.GONE);
        }
    }

    private void startSensor(){
        if (mSensorController == null) {
            mSensorController = new SensorController();
            mSensorController.setSensorEventListener(mAngleSensorListener);
        }
        mSensorController.startSensor(getApplicationContext());
    }

    private void stopSensor(){
        if(mSensorController != null){
            mSensorController.stopSensor();
        }
        mAngleSensorListener.reset();
    }

    private void initGamePlayConfig() {
        /**
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         * ak/sk/token用于用户鉴权，需要从火山官网上获取，具体步骤详见README[鉴权相关]。
         * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
         *
         * ak/sk/token/gameId的值从assets目录下的sts.json文件中读取，该目录及文件需要自行创建。
         * sts.json的格式形如
         * {
         *     "gameId": "your_game_id",
         *     "ak": "your_ak",
         *     "sk": "your_sk",
         *     "token": "your_token"
         * }
         */
        String ak = "", sk = "", token = "", gameId = "";  // 这里需要替换成你的 ak/sk/token/gameId
        String sts = AssetsUtil.getTextFromAssets(this.getApplicationContext(), "sts.json");
        try {
            JSONObject stsJObj = new JSONObject(sts);
            ak = stsJObj.getString("ak");
            sk = stsJObj.getString("sk");
            token = stsJObj.getString("token");
            gameId = stsJObj.getString("gameId");
        } catch (JSONException e) {
            Log.e(TAG, "failed to parse json : " + sts);
        }

        String roundId = "roundId_123";
        String userId = "userId_" + System.currentTimeMillis();

        GamePlayConfig.Builder mBuilder = new GamePlayConfig.Builder();
        mBuilder.userId(userId).ak(ak).sk(sk).token(token).container(mContainer).roundId(roundId).gameId(gameId);

        GamePlayConfig mGamePlayConfig = mBuilder.build();
        VeGameEngine.getInstance().start(mGamePlayConfig, this);
    }

    @Override
    public void onPlaySuccess(String s, int i, Map<String, String> map, String s1, String s2) {
        Log.e(TAG, "onPlaySuccess is invoked...");

        IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();

        int containerWidth = mContainer.getMeasuredWidth();
        int containerHeight = mContainer.getMeasuredHeight();
        mAngleSensorListener.setSize(containerWidth, containerHeight);

        if (ioDeviceManager != null) {
            mWSADRockView.setOnRockerChangeListener(new KeyboardEventSender(ioDeviceManager));

            // 禁止显示游戏画面的内部View，执行触摸事件的默认处理逻辑
            ioDeviceManager.setInterceptTouchSend(true);

            // 将触摸事件转换成鼠标事件
            convertTouchEventToMouseEvent(ioDeviceManager, containerWidth, containerHeight);

            mLeftShotButton.setOnClickListener(view -> {
                // 点击事件转换成鼠标左键点击
                ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.DOWN);
                ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.UP);
            });

            mRightAimButton.setOnClickListener(view -> {
                if (isMouseRightLongPress) {
                    isMouseRightLongPress = false;
                    // 释放鼠标右键
                    ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyRBUTTON_VALUE, KeySateType.UP);
                    mRightAimButton.setBackground(getDrawable(R.mipmap.aim_bg));
                } else {
                    isMouseRightLongPress = true;
                    // 按下鼠标右键
                    ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyRBUTTON_VALUE, KeySateType.DOWN);
                    mRightAimButton.setBackground(getDrawable(R.mipmap.aim_close_bg));
                }
            });

            // MouseMoveAndLeftClickSender封装了将按钮点击事件、长按事件转换为鼠标左键长按、鼠标滑动事件的逻辑
            mouseMoveAndLeftClickSender = new MouseMoveAndLeftClickSender(ioDeviceManager);
            mouseMoveAndLeftClickSender.setMaxMoveSize(containerWidth, containerHeight);
            mouseMoveAndLeftClickSender.setAngleSensorListener(mAngleSensorListener);
            mouseMoveAndLeftClickSender.enableAutoCloseSensorWhenMovingShot(autoCloseSensorWhenMovingShotSwitch.isChecked());

//            int currentSensitivity = SensitivityManager.getCurrentSensitivity();
            mMouseMoveSensitivityController.setProgress((int)AngleSensorListener.sensitivityFactor);
            mRightShotRockerView.setOnRockerLocationListener(mouseMoveAndLeftClickSender);
            mRightShotRockerView.setOnRockerChangeListener(mouseMoveAndLeftClickSender);

            mMouseMoveSensitivityController.setOnSeekBarChangeListener(new SeekBar.OnSeekBarChangeListener() {
                @Override
                public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
                }

                @Override
                public void onStartTrackingTouch(SeekBar seekBar) {

                }

                @Override
                public void onStopTrackingTouch(SeekBar seekBar) {
                    int currentSensitivity = seekBar.getProgress();
                    AngleSensorListener.sensitivityFactor = currentSensitivity;
                }
            });
        }

        jumpButton.setOnClickListener(view -> {
            // 按下空格键
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeySpace, KeySateType.DOWN);
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeySpace, KeySateType.UP);
        });
        replaceBulletButton.setOnClickListener(view -> {
            // 按下R键
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyR, KeySateType.DOWN);
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyR, KeySateType.UP);
        });
        reviewButton.setOnClickListener(view -> {
            // 按下F键
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyF, KeySateType.DOWN);
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyF, KeySateType.UP);
        });
        switchButton.setOnClickListener(view -> {
            // 按下Q键
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyQ, KeySateType.DOWN);
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyQ, KeySateType.UP);
        });
        _BButton.setOnClickListener(view -> {
            // 按下B键
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyB, KeySateType.DOWN);
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyB, KeySateType.UP);
        });
        escButton.setOnClickListener(view -> {
            // 按下ESC键
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyEscape, KeySateType.DOWN);
            ioDeviceManager.sendKeyboardKey(KeyBoardKey.KeyboardKeyEscape, KeySateType.UP);
        });
    }

    private void convertTouchEventToMouseEvent(IODeviceManager ioDeviceManager,
                                               int containerWidth, int containerHeight) {
        // 设置外部监听器
        // 创建BriefTouchListener实例，将触摸事件转换为mouse move事件，同时将屏幕点击事件转换成鼠标左键点击事件
        ioDeviceManager.setTouchListener(new IODeviceManager.BriefTouchListener() {
            private float startX;
            private float startY;
            private long startTimeTouch;

            @Override
            public void onBriefTouchEvent(List<BriefTouchEvent> briefEvents) {
                if (briefEvents == null || briefEvents.isEmpty()) {
                    return;
                }
                BriefTouchEvent briefTouchEvent = briefEvents.get(0);
                if (briefTouchEvent == null) {
                    return;
                }

                float currentX = briefTouchEvent.x;
                float currentY = briefTouchEvent.y;

                if (briefTouchEvent.action == MotionEvent.ACTION_DOWN) {
                    startX = currentX;
                    startY = currentY;
                    startTimeTouch = System.currentTimeMillis();
                } else if (briefEvents.get(0).action == MotionEvent.ACTION_MOVE) {
                    if (System.currentTimeMillis() - startTimeTouch <= 100) {
                        // 小于100毫秒一律当做点击
                        // 业务方可根据自身业务灵活实现相关逻辑
                        return;
                    }
                    currentX = currentX - startX;
                    currentY = currentY - startY;
                    int finalX = (int) (currentX * containerWidth);
                    int finalY = (int) (currentY * containerHeight);
                    if (finalX == 0 && finalY == 0) {
                        // 如果手指未移动则不发送，减少发送数据量，降低data channel压力
                        return;
                    }
                    Log.e(TAG, "the delta of mouse move is " + finalX + ", " + finalY + ";");

                    ioDeviceManager.sendInputMouseMove(finalX * MouseMoveAndLeftClickSender.SENSITIVITY_FACTOR, finalY * MouseMoveAndLeftClickSender.SENSITIVITY_FACTOR);
                    startX = briefTouchEvent.x;
                    startY = briefTouchEvent.y;
                } else if (briefEvents.get(0).action == MotionEvent.ACTION_UP) {
                    if (System.currentTimeMillis() - startTimeTouch <= 100) {
                        // 小于100毫秒一律当做点击
                        ioDeviceManager.sendInputCursorPos(currentX, currentY);
                        ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.DOWN);
                        ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.UP);
                    }
                }
            }
        });
    }

    @Override
    public void onError(int i, String s) {
        Log.e(TAG, "receive the error : " + i + ", " + s);
    }

    @Override
    public void onWarning(int i, String s) {

    }

    @Override
    public void onNetworkChanged(int i) {

    }

    @Override
    public void onServiceInit() {

    }

    @Override
    public void onQueueUpdate(List<QueueInfo> list) {

    }

    @Override
    public void onQueueSuccessAndStart(int i) {

    }

    @Override
    public void onDestroy(){
        super.onDestroy();
        VeGameEngine.getInstance().stop();
        // 禁止陀螺仪控制画面视角的功能
        if(mSensorController != null){
            mSensorController.stopSensor();
            mSensorController = null;
        }
        Monitor.listener = null;
    }

    @Override
    public void onMonitor(float num) {
        @SuppressLint("DefaultLocale")
        String tip = String.format("event num pre second : %f", num);
        monitorView.setText(tip);
    }
}
