package com.volcengine.vegameengine.scene.cs;

import android.annotation.SuppressLint;
import android.os.Bundle;
import android.util.Log;
import android.view.MotionEvent;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.TextView;

import androidx.annotation.Nullable;
import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.androidcloud.common.model.BriefTouchEvent;
import com.volcengine.cloudcore.common.mode.KeySateType;
import com.volcengine.cloudcore.common.mode.MouseKey;
import com.volcengine.cloudcore.common.mode.QueueInfo;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IODeviceManager;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.base.BasePlayActivity;
import com.volcengine.vegameengine.components.rocker.KeyboardEventSender;
import com.volcengine.vegameengine.components.rocker.RockerView;
import com.volcengine.vegameengine.components.sensor.SensorController;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.Map;

public class CSDemoActivity extends BasePlayActivity implements IGamePlayerListener {
    private final String TAG = "CSDemoActivity";
    private FrameLayout mContainer;
    private RockerView mRockView;
    private TextView mOperationTip;
    private Button mSensorControllerButton;
    private SensorController mSensorController;
    private AngleSensorListener mAngleSensorListener = new AngleSensorListener();

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
        mRockView = findViewById(R.id.w_a_s_d_rocker);

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mOperationTip.setVisibility(isChecked ? View.VISIBLE : View.GONE);
        });

        // 长按按钮启用陀螺仪控制画面视角的功能
        // 释放按钮禁止陀螺仪控制画面视角的功能
        mSensorControllerButton = findViewById(R.id.sensor_controller);
        mSensorControllerButton.setOnTouchListener((view, motionEvent) -> {
            if(motionEvent.getAction() == MotionEvent.ACTION_DOWN){
                if(mSensorController == null){
                    mSensorController = new SensorController();
                    mSensorController.setSensorEventListener(mAngleSensorListener);
                }
                mSensorController.startSensor(getApplicationContext());
            } else if(motionEvent.getAction() == MotionEvent.ACTION_UP
                        || motionEvent.getAction() == MotionEvent.ACTION_CANCEL){
                mSensorController.stopSensor();
                mAngleSensorListener.reset();
            }
            return false;
        });
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
            mRockView.setOnRockerChangeListener(new KeyboardEventSender(ioDeviceManager));

            // 禁止显示游戏画面的内部View，执行触摸事件的默认处理逻辑
            ioDeviceManager.setInterceptTouchSend(true);

            // 设置外部监听器，
            // 将触摸事件转换为mouse move事件，
            // 将屏幕点击事件转换成鼠标左键点击事件
            ioDeviceManager.sendInputCursorPos(0.5f, 0.5f);
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
                        ioDeviceManager.sendInputCursorPos(startX, startY);
                        startTimeTouch = System.currentTimeMillis();
                    } else if (briefEvents.get(0).action == MotionEvent.ACTION_MOVE) {
                        if(System.currentTimeMillis() - startTimeTouch <= 500){
                            // 小于500毫秒一律当做点击
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
                        ioDeviceManager.sendInputMouseMove(finalX, finalY);
                        startX = briefTouchEvent.x;
                        startY = briefTouchEvent.y;
                    } else if(briefEvents.get(0).action == MotionEvent.ACTION_UP){
                        if(System.currentTimeMillis() - startTimeTouch <= 500){
                            // 小于500毫秒一律当做点击
                            ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.DOWN);
                            ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.UP);
                        }
                    }
                }

            });
        }
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
}
