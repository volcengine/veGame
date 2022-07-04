/**
 * Copyright (c) 2022 Volcengine
 * <p>
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 * <p>
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 * <p>
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

package com.volcengine.vegameengine;

import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;

import android.app.Activity;
import android.content.ClipData;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Build;
import android.os.Bundle;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.constraintlayout.widget.ConstraintLayout;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.CommonEvent;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.CameraId;
import com.volcengine.cloudcore.common.mode.LocalVideoStreamError;
import com.volcengine.cloudcore.common.mode.LocalVideoStreamState;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IClipBoardListener;
import com.volcengine.cloudphone.apiservice.IMessageChannel;
import com.volcengine.cloudphone.apiservice.StreamProfileChangeCallBack;
import com.volcengine.cloudphone.apiservice.outinterface.CameraManagerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.cloudphone.apiservice.outinterface.RemoteCameraRequestListener;
import com.volcengine.vegameengine.util.ScreenUtil;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class GameActivity extends AppCompatActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();

    private ViewGroup mContainer;
    private Bundle mBundle;
    public static final String KEY_PARAM_GAME_ID = "gameId";
    public static final String KEY_ROUND_ID = "roundId";
    public static final String KEY_ClARITY_ID = "clarity_id";

    private ConstraintLayout mContainers;

    private boolean mIsHideBtns = false;

    public VeGameEngine veGameEngine = VeGameEngine.getInstance();
    private IMessageChannel mMessageChannel;

    private Button btnClarity0, btnClarity1, btnClarity2, btnClarity3, btnClarity4, btnClarity5;
    private TextView tvInfo;
    private boolean isLand = false;
    private boolean isShowInfo = false;

    private boolean isOpenAcc = true;
    private boolean isOpenMagnetic = true;
    private boolean isOpenVibrator = true;
    private boolean isOpenOrientationSensor = true;
    private boolean isOpenGravity = true;
    private boolean isOpenGyroscopeSensor = true;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        mBundle = getIntent().getBundleExtra("bundle");
        setContentView(R.layout.activity_play);
        initView();
        initClarity();
        intConfig();
    }

    private void initClarity() {
        List<Button> list = Arrays.asList(btnClarity0, btnClarity1, btnClarity2, btnClarity3, btnClarity4, btnClarity5);
        for (int i = 0; i < list.size(); i++) {
            Button button = list.get(i);
            int id = i;
            button.setOnClickListener(v -> veGameEngine.getClarityService().switchVideoStreamProfileId(id));
        }
    }

    private void intConfig() {
        GamePlayConfig.Builder builder = new GamePlayConfig.Builder();
        String userId = "userid" + System.currentTimeMillis();
        AcLog.d(TAG, "userId: " + userId);
        builder.userId(userId) // 用户userid
                .ak("")  // 必填参数，申请云游戏服务临时 ak
                .sk("")  // 必填参数，申请云游戏服务临时 sk
                .token("") //必填参数，申请云游戏服务临时 token
                .container(mContainer)//必填参数，用来承载画面的 Container, 参数说明: layout 需要是FrameLayout或者FrameLayout的子类
                .roundId(mBundle.getString(KEY_ROUND_ID))//必填参数，自定义roundId
                .videoStreamProfileId(mBundle.getInt(KEY_ClARITY_ID)) // 选填参数，清晰度ID
                .gameId(mBundle.getString(KEY_PARAM_GAME_ID)) //必填 gameId
                .streamListener(GameActivity.this);


        GamePlayConfig phoneConfig = builder.build();
        // 初始化成功才可以调用
        veGameEngine.start(phoneConfig, GameActivity.this);
    }

    @Override
    protected void onSaveInstanceState(@NonNull Bundle outState) {
        outState.putString("key_uid", "user_id");
        super.onSaveInstanceState(outState);
    }

    @Override
    protected void onResume() {
        super.onResume();
        veGameEngine.resume();
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onPause() {
        super.onPause();
        veGameEngine.pause();
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onDestroy() {
        veGameEngine.stop();
        super.onDestroy();
    }

    public void onPlayEvent(CommonEvent commonEvent) {

    }

    @Override
    public void onPlaySuccess(String roundId, int clarityId, Map<String, String> map, String gameId,
                              String reservedId) {
        AcLog.d(TAG, "roundId " + roundId + " clarityId " + clarityId + "extra:" + map +
                "gameId : " + gameId + " reservedId" + reservedId);
        VeGameEngine.getInstance().getCameraManager().setRemoteRequestListener(new RemoteCameraRequestListener() {
            @Override
            public void onVideoStreamStartRequested(CameraId cameraId) {
                AcLog.d(TAG, "onVideoStreamStartRequested, cameraId :" + cameraId);
                VeGameEngine.getInstance().getCameraManager().startVideoStream(cameraId);
            }

            @Override
            public void onVideoStreamStopRequested() {
                AcLog.d(TAG, "onVideoStreamStopRequested ");
                VeGameEngine.getInstance().getCameraManager().stopVideoStream();
            }
        });
        VeGameEngine.getInstance().getCameraManager().setCameraManagerListener(new CameraManagerListener() {
            @Override
            public void onLocalVideoStateChanged(LocalVideoStreamState localVideoStreamState, LocalVideoStreamError errorCode) {
                AcLog.d(TAG, "LocalVideoStreamState" + localVideoStreamState.toString() + ",LocalVideoStreamError" + errorCode);
            }

            @Override
            public void onFirstCapture() {
                AcLog.d(TAG, "onFirstCapture");
            }
        });

        veGameEngine.getClarityService().setStreamProfileChangeListener(new StreamProfileChangeCallBack() {
            @Override
            public void onVideoStreamProfileChange(boolean isSuccess, int from, int to) {
                AcLog.d(TAG, "VideoStreamProfileChange  isSuccess " + isSuccess + "from " + from + "to " + to);
            }

            @Override
            public void onError(int i, String s) {
                AcLog.d(TAG, "onError - " + s);
            }
        });

        veGameEngine.getClipBoardServiceManager().setBoardSyncClipListener(new IClipBoardListener() {
            @Override
            public void onClipBoardMessageReceived(ClipData clipData) {
                AcLog.d(TAG, "clipBoard : " + clipData.toString());
            }
        });
        tvInfo.setText("roundId:" + roundId + "\n" + "streamProfile:" + clarityId);
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            tvInfo.setZ(Long.MAX_VALUE);
        }
    }


    @Override
    public void onError(int i, String s) {
        String msg = "onError:" + i + ", " + s;
        Toast.makeText(this, "code" + i + "msg" + s, Toast.LENGTH_SHORT).show();
        Log.e(TAG, msg);
    }

    @Override
    public void onWarning(int i, String s) {
    }

    @Override
    public void onNetworkChanged(int i) {
        Log.d(TAG, String.format("%d", i));
    }

    private void initView() {
        mContainer = findViewById(R.id.container);

        Button restartButton = findViewById(R.id.btn_restart);

        btnClarity0 = findViewById(R.id.btn_clarity_0);
        btnClarity1 = findViewById(R.id.btn_clarity_1);
        btnClarity2 = findViewById(R.id.btn_clarity_2);
        btnClarity3 = findViewById(R.id.btn_clarity_3);
        btnClarity4 = findViewById(R.id.btn_clarity_4);
        btnClarity5 = findViewById(R.id.btn_clarity_5);
        tvInfo = findViewById(R.id.tv_info);

        mContainers = findViewById(R.id.cl_container);

        findViewById(R.id.btn_show_info).setOnClickListener(v -> {
            isShowInfo = !isShowInfo;
            tvInfo.setVisibility(isShowInfo ? View.VISIBLE : View.GONE);
        });

        findViewById(R.id.btn_orientation).setOnClickListener(v -> {
            if (isLand) {
                onRotation(270);
            } else {
                onRotation(0);
            }
            isLand = !isLand;
        });

        findViewById(R.id.btn_show_or_hide).setOnClickListener(v -> {
            mIsHideBtns = !mIsHideBtns;
            mContainers.setVisibility(mIsHideBtns ? View.GONE : View.VISIBLE);
        });

        restartButton.setOnClickListener(v -> {
            veGameEngine.restart();
        });

        findViewById(R.id.btn_stop).setOnClickListener(v -> {
            finish();
        });

        findViewById(R.id.btn_pause).setOnClickListener(v -> {
            veGameEngine.pause();
        });

        findViewById(R.id.btn_resume).setOnClickListener(v -> {
            veGameEngine.resume();
        });
        findViewById(R.id.btn_release).setOnClickListener(v -> {
            veGameEngine.release();
        });

        findViewById(R.id.btn_throw_exception).setOnClickListener(v -> {
            throw new IllegalArgumentException("test");
        });


        findViewById(R.id.btn_acc).setOnClickListener(v -> {
            isOpenAcc = !isOpenAcc;
            veGameEngine.enableAccelSensor(isOpenAcc);
        });

        findViewById(R.id.btn_magnetic).setOnClickListener(v -> {
            isOpenMagnetic = !isOpenMagnetic;
            veGameEngine.enableMagneticSensor(isOpenMagnetic);
        });

        findViewById(R.id.btn_gravity).setOnClickListener(v -> {
            isOpenGravity = !isOpenGravity;
            veGameEngine.enableGravitySensor(isOpenGravity);
        });

        findViewById(R.id.btn_orientation_sensor).setOnClickListener(v -> {
            isOpenOrientationSensor = !isOpenOrientationSensor;
            veGameEngine.enableOrientationSensor(isOpenOrientationSensor);
        });

        findViewById(R.id.btn_gyroscope).setOnClickListener(v -> {
            isOpenGyroscopeSensor = !isOpenGyroscopeSensor;
            veGameEngine.enableGyroscopeSensor(isOpenGyroscopeSensor);
        });

        findViewById(R.id.btn_vibrator).setOnClickListener(v -> {
            isOpenVibrator = !isOpenVibrator;
            veGameEngine.enableVibrator(isOpenVibrator);
        });

        findViewById(R.id.btn_send_clipData).setOnClickListener(v -> {
            VeGameEngine.getInstance().getClipBoardServiceManager()
                    .sendClipBoardMessage(ClipData.newPlainText("test", "test data"));
        });

    }

    public static void startGame(String gameId, String roundId, int clarityId, Activity activity) {
        Intent intent = new Intent(activity, GameActivity.class);
        Bundle bundle = new Bundle();
        bundle.putString(GameActivity.KEY_PARAM_GAME_ID, gameId);
        bundle.putString(GameActivity.KEY_ROUND_ID, roundId);
        bundle.putInt(GameActivity.KEY_ClARITY_ID, clarityId);
        intent.putExtra("bundle", bundle);
        activity.startActivity(intent);
    }

    private void setRotation(int rotation) {
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

    @Override
    public void onFirstAudioFrame(String s) {
        Log.d(TAG, "onFirstAudioFrame " + s);
    }

    @Override
    public void onFirstRemoteVideoFrame(String s) {
        Log.d(TAG, "onFirstRemoteVideoFrame " + s);
    }

    @Override
    public void onStreamStarted() {
        Log.d(TAG, "onStreamStarted ");
    }

    @Override
    public void onStreamPaused() {
        Log.d(TAG, "onStreamPaused ");
    }

    @Override
    public void onStreamResumed() {
        Log.d(TAG, "onStreamResumed ");
    }

    @Override
    public void onStreamStats(StreamStats streamStats) {
        Log.d(TAG, " " + streamStats.getDecoderOutputFrameRate() + " " +
                streamStats.getStallCount() + " " +
                streamStats.getReceivedResolutionHeight() + " " +
                streamStats.getReceivedResolutionWidth() + " " +
                streamStats.getRendererOutputFrameRate() + " " +
                streamStats.getDecoderOutputFrameRate() + " " +
                streamStats.getReceivedAudioBitRate() + " " +
                streamStats.getReceivedVideoBitRate());
    }

    @Override
    public void onStreamConnectionStateChanged(int i) {
        Log.d(TAG, "onStreamConnectionStateChanged " + i);
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        AcLog.d(TAG, "onConfigurationChanged newConfig " + newConfig.toString());
        veGameEngine.rotate(newConfig.orientation);
    }

    @Override
    public void onDetectDelay(long l) {
        Log.d(TAG, "delay " + l);
    }

    @Override
    public void onRotation(int i) {
        Log.d(TAG, "rotation" + i);
        setRotation(i);
    }

    @Override
    public void onPodExit(int i, String s) {
        Log.d(TAG, "onPodExit" + i + " ,msg:" + s);
    }
}
