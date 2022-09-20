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

import com.volcengine.cloudcore.common.mode.CameraId;
import com.volcengine.cloudcore.common.mode.LocalVideoStreamError;
import com.volcengine.cloudcore.common.mode.LocalVideoStreamState;
import com.volcengine.cloudphone.apiservice.IClipBoardListener;
import com.volcengine.cloudphone.apiservice.IMessageChannel;
import com.volcengine.cloudphone.apiservice.StreamProfileChangeCallBack;
import com.volcengine.cloudphone.apiservice.outinterface.CameraManagerListener;
import com.volcengine.cloudphone.apiservice.outinterface.RemoteCameraRequestListener;
import com.volcengine.vegameengine.feature.AudioServiceView;
import com.volcengine.vegameengine.feature.CamaraManagerView;
import com.volcengine.vegameengine.feature.ClarityServiceView;
import com.volcengine.vegameengine.feature.ClipBoardServiceManagerView;
import com.volcengine.vegameengine.feature.GamePadServiceView;
import com.volcengine.vegameengine.feature.GroundManagerView;
import com.volcengine.vegameengine.feature.LocationServiceView;
import com.volcengine.vegameengine.feature.MessageChannelView;
import com.volcengine.vegameengine.feature.PodControlServiceView;
import com.volcengine.vegameengine.feature.SensorView;
import com.volcengine.vegameengine.feature.UnclassifiedView;
import com.volcengine.vegameengine.util.DialogUtils;
import com.volcengine.vegameengine.util.ScreenUtil;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;

import java.util.Arrays;
import java.util.List;
import java.util.Map;

public class GameActivity extends AppCompatActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();
    private ViewGroup mContainer;
    public static final String KEY_PARAM_GAME_ID = "gameId";
    public static final String KEY_ROUND_ID = "roundId";
    public static final String KEY_ClARITY_ID = "clarity_id";
    private ConstraintLayout mContainers;

    private boolean mIsHideButtons = false;
    public VeGameEngine veGameEngine = VeGameEngine.getInstance();
    DialogUtils.DialogWrapper mDialogWrapper;

    private Button btnAudio, btnCamera, btnClarity, btnClipBoard, btnFileChannel, btnGround, btnLocation;
    private Button btnMessageChannel, btnPodControl, btnRotation, btnSensor, btnUnclassified;
    private TextView tvInfo;
    private boolean isLand = false;
    private boolean isShowInfo = false;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_play);
        mContainer = findViewById(R.id.container);
        initView();
        initConfig();
    }

    private void initConfig() {
        GamePlayConfig.Builder builder = new GamePlayConfig.Builder();
        String userId = "userid" + System.currentTimeMillis();
        AcLog.d(TAG, "userId: " + userId);
        Intent intent = getIntent();
        String ak = "AKTPMjcwM2UyOTQ5MmJiNDk4Y2FmODVlOWQ3N2QyMzQ4YWE";
        String sk = "Qd6HIiRCbZT59xQYxJwXzm9ihglPHK4eh4/M2fGME9DQK/Y7+i8L9zoN99j32wyIQ54HSWReGjqjeo7x8BAMzQ==";
        String token = "STS2eyJMVEFjY2Vzc0tleUlkIjoiQUtMVFl6ZzFOMlUyTlRGbVpqZGhORGsyWm1FMU56ZG1ZVFEyTVdGak1EaGlaVGMiLCJBY2Nlc3NLZXlJZCI6IkFLVFBNamN3TTJVeU9UUTVNbUppTkRrNFkyRm1PRFZsT1dRM04yUXlNelE0WVdFIiwiU2lnbmVkU2VjcmV0QWNjZXNzS2V5IjoiU1UxTnZhcFJUTlkzMHVtckR3c2ZTRFFPRDNJUmNNb1lwd2dpVG0vMFptazJJbVlPQy9ZdklNYXk3clpjcUFYTUtwdzcwclFOVTlzQWhHdFdHTU1Ddm9ZMzRqYXNTTFIxSTJabGtEbXphK2xhc1JIRDRDY21ONWsrajJaRUF6T0MiLCJFeHBpcmVkVGltZSI6MTcyMzMyMzE0NywiUG9saWN5U3RyaW5nIjoie1wiU3RhdGVtZW50XCI6W3tcIkVmZmVjdFwiOlwiQWxsb3dcIixcIkFjdGlvblwiOltcIipcIl0sXCJSZXNvdXJjZVwiOltcIipcIl19XX0iLCJTaWduYXR1cmUiOiIzMWE5OGI4YTc1MTFjMTQwMDczOTQ4Yzg0OWI3NjMxMWRkYmZhODQyYTk2ZTg3NDFhNTE2MmQ2Zjk1NWQ2MDM0In0=";

        // ak, sk， token。请通过火山引擎申请ak获得，详情见https://www.volcengine.com/docs/6512/75577
        builder.userId(userId) // 用户userid
                .ak(ak) // 必填 ACEP ak
                .sk(sk)  // 必填 ACEP sk
                .token(token) // acep session
                .container(mContainer)//必填参数，用来承载画面的 Container, 参数说明: layout 需要是FrameLayout或者FrameLayout的子类
                .roundId(intent.getStringExtra(KEY_ROUND_ID))//必填参数，自定义roundId
                .videoStreamProfileId(intent.getIntExtra(KEY_ClARITY_ID, 1)) // 选填参数，清晰度ID
                .gameId(intent.getStringExtra(KEY_PARAM_GAME_ID)) //必填 gameId
                .streamListener(GameActivity.this);

        GamePlayConfig gamePlayConfig = builder.build();
        // 初始化成功才可以调用
        veGameEngine.start(gamePlayConfig, GameActivity.this);
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

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onPlaySuccess(String roundId, int clarityId, Map<String, String> extraMap, String gameId,
                              String reservedId) {
        AcLog.d(TAG, "roundId " + roundId + " clarityId " + clarityId + "extra:" + extraMap +
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
//        tvInfo.setText("roundId:" + roundId + "\n" + "streamProfile:" + clarityId);
        tvInfo.setText(String.format(
                "roundId: %s\nstreamProfile: %s\nextraMap: %s\ngameId: %s\nreservedId: %s\n",
                roundId,
                clarityId,
                extraMap,
                gameId,
                reservedId
        ));
        if (Build.VERSION.SDK_INT >= Build.VERSION_CODES.LOLLIPOP) {
            tvInfo.setZ(Long.MAX_VALUE);
        }
    }

    private void initView() {
        mContainer = findViewById(R.id.container);
        mContainers = findViewById(R.id.cl_container);
        tvInfo = findViewById(R.id.tv_info);

        btnAudio = findViewById(R.id.btn_audio);
        btnCamera = findViewById(R.id.btn_camera);
        btnClarity = findViewById(R.id.btn_clarity);
        btnClipBoard = findViewById(R.id.btn_clipboard);
        btnFileChannel = findViewById(R.id.btn_file_channel);
        btnGround = findViewById(R.id.btn_ground);
        btnLocation = findViewById(R.id.btn_location);
        btnMessageChannel = findViewById(R.id.btn_message_channel);
        btnPodControl = findViewById(R.id.btn_pod_control);
        btnRotation = findViewById(R.id.btn_orientation);
        btnSensor = findViewById(R.id.btn_sensor);
        btnUnclassified = findViewById(R.id.btn_unclassified);

        findViewById(R.id.btn_show_info).setOnClickListener(v -> {
            isShowInfo = !isShowInfo;
            tvInfo.setVisibility(isShowInfo ? View.VISIBLE : View.GONE);
        });

        findViewById(R.id.btn_show_or_hide).setOnClickListener(v -> {
            mIsHideButtons = !mIsHideButtons;
            mContainers.setVisibility(mIsHideButtons ? View.GONE : View.VISIBLE);
        });

        btnAudio.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new AudioServiceView(this, veGameEngine.getAudioService()));
            mDialogWrapper.show();
        });

        btnCamera.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new CamaraManagerView(this, veGameEngine.getCameraManager()));
            mDialogWrapper.show();
        });

        btnClarity.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new ClarityServiceView(this, veGameEngine.getClarityService()));
            mDialogWrapper.show();
        });

        btnClipBoard.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new ClipBoardServiceManagerView(this, veGameEngine.getClipBoardServiceManager()));
            mDialogWrapper.show();
        });
        //TODO:添加文件通道接口
        btnFileChannel.setOnClickListener(view -> {});

        btnGround.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new GroundManagerView(this, veGameEngine.getGameGroundSwitchManager()));
            mDialogWrapper.show();
        });

        btnLocation.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new LocationServiceView(this, veGameEngine.getLocationService()));
            mDialogWrapper.show();
        });

        btnMessageChannel.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new MessageChannelView(this, veGameEngine.getMessageChannel()));
            mDialogWrapper.show();
        });

        btnPodControl.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(
                    new PodControlServiceView(this, veGameEngine.getPodControlService()));
            mDialogWrapper.show();
        });

        btnRotation.setOnClickListener(view -> {
            if (isLand) {
                onRotation(270);
            } else {
                onRotation(0);
            }
            isLand = !isLand;
        });

        btnSensor.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(new SensorView(this));
            mDialogWrapper.show();
        });

        btnUnclassified.setOnClickListener(view -> {
            mDialogWrapper = DialogUtils.wrapper(new UnclassifiedView(this));
            mDialogWrapper.show();
        });
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
    public void onError(int i, String s) {
        String msg = "onError:" + i + ", " + s;
        Toast.makeText(this, "code" + i + "msg" + s, Toast.LENGTH_SHORT).show();
        Log.e(TAG, msg);
    }

    @Override
    public void onWarning(int i, String s) {
        Log.d(TAG, "warn: code " + i + ", msg" + s);
    }

    @Override
    public void onNetworkChanged(int i) {
        Log.d(TAG, String.format("%d", i));
    }


    public static void startGame(
            String gameId,
            String roundId,
            int clarityId,
            Activity activity) {
        Intent intent = new Intent(activity, GameActivity.class);
        intent.putExtra(GameActivity.KEY_PARAM_GAME_ID, gameId);
        if (roundId.isEmpty() || roundId.equals("")) roundId="123";
        intent.putExtra(GameActivity.KEY_ROUND_ID, roundId);
        intent.putExtra(GameActivity.KEY_ClARITY_ID, clarityId);
        activity.startActivity(intent);
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
    public void onLocalStreamStats(LocalStreamStats localStreamStats) {
        AcLog.d(TAG, "LocalStreamStats" + localStreamStats);
    }

    @Override
    public void onStreamConnectionStateChanged(int i) {
        Log.d(TAG, "onStreamConnectionStateChanged " + i);
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        AcLog.d(TAG, "onConfigurationChanged newConfig " + newConfig);
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
