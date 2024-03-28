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

// 外部触摸事件来实现鼠标右键 + 滑动控制视角的交互效果
public class CSDemoWithOutterTouchListenerActivity extends BasePlayActivity implements IGamePlayerListener {
    private final String TAG = "CSDemoActivity";
    private FrameLayout mContainer;
    private TextView mOperationTip;
    private Button mAimButton;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_cs_demo_with_out_touch_listener);
        initView();
        initGamePlayConfig();
    }

    @SuppressLint("ClickableViewAccessibility")
    private void initView() {
        mContainer = findViewById(R.id.container);
        SwitchCompat mSwShowOrHide = findViewById(R.id.sw_show_or_hide);
        mOperationTip = findViewById(R.id.operation_tip);
        mAimButton = findViewById(R.id.aim_button);

        mAimButton.setOnTouchListener(new View.OnTouchListener() {
            private final TouchEventToMouseMoveConverter converter = new TouchEventToMouseMoveConverter();

            @Override
            public boolean onTouch(View view, MotionEvent motionEvent) {
                IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();

                int containerWidth = mContainer.getMeasuredWidth();
                int containerHeight = mContainer.getMeasuredHeight();

                float x = motionEvent.getX();
                float y = motionEvent.getY();

                // 坐标归一化
                x /= containerWidth;
                y /= containerHeight;

                Log.e(TAG, String.format("the normalized coordination is [%f, %f]", x, y));

                if(motionEvent.getAction() == MotionEvent.ACTION_DOWN){
                    // 立即发送鼠标右键
                    if(ioDeviceManager != null) {
                        ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyRBUTTON_VALUE, KeySateType.DOWN);
                        ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyRBUTTON_VALUE, KeySateType.UP);
                    }

                    converter.prepare(x, y, containerWidth, containerHeight);
                } else if(motionEvent.getAction() == MotionEvent.ACTION_MOVE){
                    // 将触摸事件转换成鼠标move事件
                    // convert方法内部会进行判断，长按500毫秒后才会转换
                    converter.convert(x, y);
                } else {
                    converter.reset();
                }
                return true;
            }
        });

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mOperationTip.setVisibility(isChecked ? View.VISIBLE : View.GONE);
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

        if (ioDeviceManager != null) {

            // 禁止显示游戏画面的内部View，执行触摸事件的默认处理逻辑
            ioDeviceManager.setInterceptTouchSend(true);

            // 设置外部监听器，
            // 将触摸事件转换为mouse move事件，
            // 将屏幕点击事件转换成鼠标左键点击事件
            ioDeviceManager.sendInputCursorPos(0.5f, 0.5f);
            ioDeviceManager.setTouchListener(new IODeviceManager.BriefTouchListener() {
                TouchEventToMouseMoveConverter converter = new TouchEventToMouseMoveConverter();

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
                        converter.prepare(currentX, currentY, containerWidth, containerHeight);
                    } else if (briefEvents.get(0).action == MotionEvent.ACTION_MOVE) {
                        converter.convert(currentX, currentY);
                    } else if(briefEvents.get(0).action == MotionEvent.ACTION_UP){
                        if(System.currentTimeMillis() - converter.startTimeTouch <= 500){
                            // 小于500毫秒一律当做点击
                            ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.DOWN);
                            ioDeviceManager.sendInputMouseKey(MouseKey.MouseKeyLBUTTON_VALUE, KeySateType.UP);
                        }
                        converter.reset();
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

    // 将触摸事件转换成鼠标move事件的转换器
    private class TouchEventToMouseMoveConverter{
        private float startX;
        private float startY;
        private long startTimeTouch;

        // 准备后，需要经过多少毫秒才开始转换
        private long duration = 500;

        private int containerWidth, containerHeight;

        private boolean isPrePared = false;

        private void prepare(float x, float y, int width, int height){
            if(isPrePared){
                return;
            }
            isPrePared = true;
            startX = x;
            startY = y;
            startTimeTouch = System.currentTimeMillis();

            IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();
            ioDeviceManager.sendInputCursorPos(startX, startY);

            containerWidth = width;
            containerHeight = height;
        }

        private void convert(float x, float y){
            if(System.currentTimeMillis() - startTimeTouch <= duration){
                return;
            }
            float currentX = x - startX;
            float currentY = y - startY;
            int finalX = (int) (currentX * containerWidth);
            int finalY = (int) (currentY * containerHeight);
            if (finalX == 0 && finalY == 0) {
                // 如果手指未移动则不发送，减少发送数据量，降低data channel压力
                return;
            }
            Log.e(TAG, "the delta of mouse move is " + finalX + ", " + finalY + ";");
            IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();
            ioDeviceManager.sendInputMouseMove(finalX, finalY);
            startX = x;
            startY = y;
        }

        private void reset(){
            startTimeTouch = 0;
            startX = 0;
            startY = 0;
            containerWidth = 0;
            containerHeight = 0;
            isPrePared = false;
        }
    }
}
