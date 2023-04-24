package com.volcengine.vegameengine.feature;

import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.ViewGroup;
import android.view.WindowManager;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Map;

public class MessageChannelActivity extends AppCompatActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();

    private ViewGroup mContainer;
    private GamePlayConfig mGamePlayConfig;
    private GamePlayConfig.Builder mBuilder;


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_message_channel);
        initView();
        initGamePlayConfig();
    }

    private void initView() {
        mContainer = findViewById(R.id.container);
    }

    private void initGamePlayConfig() {
        mBuilder = new GamePlayConfig.Builder();
        String userId = "userId_" + System.currentTimeMillis();

        String ak = "", sk = "", token = "";  // 这里需要替换成你的 ak/sk/token
        String sts = AssetsUtil.getTextFromAssets(this.getApplicationContext(), "sts.json");
        try {
            JSONObject stsJObj = new JSONObject(sts);
            ak = stsJObj.getString("ak");
            sk = stsJObj.getString("sk");
            token = stsJObj.getString("token");
        } catch (JSONException e) {
            e.printStackTrace();
        }

        mBuilder.userId(userId)
                .ak(ak)
                .sk(sk)
                .token(token)
                .container(mContainer)
                .roundId("roundId_123")
                .gameId("7212894215896849207")
                .streamListener(this);

        mGamePlayConfig = mBuilder.build();
        VeGameEngine.getInstance().start(mGamePlayConfig, this);
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
    protected void onResume() {
        super.onResume();
        VeGameEngine.getInstance().resume();
        getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onPause() {
        super.onPause();
        VeGameEngine.getInstance().pause();
        getWindow().clearFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
    }

    @Override
    public void finish() {
        VeGameEngine.getInstance().stop();
        super.finish();
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        AcLog.d(TAG, "[onConfigurationChanged] newConfig: " + newConfig.orientation);
        VeGameEngine.getInstance().rotate(newConfig.orientation);
    }

    @Override
    public void onPlaySuccess(String roundId, int clarityId, Map<String, String> extraMap, String gameId, String reservedId) {
        AcLog.d(TAG, "[onPlaySuccess] ");
        AcLog.d(TAG, "roundId " + roundId + " clarityId " + clarityId + "extra:" + extraMap +
                "gameId : " + gameId + " reservedId" + reservedId);
    }

    @Override
    public void onError(int i, String s) {
        AcLog.d(TAG, "[onError] errorCode: " + i + ", errorMessage: " + s);
    }

    @Override
    public void onWarning(int i, String s) {
        AcLog.d(TAG, "[onWarning] errorCode: " + i + ", errorMessage: " + s);
    }

    @Override
    public void onNetworkChanged(int i) {
        AcLog.d(TAG, "[onNetworkChanged] network: " + i);
    }

    @Override
    public void onServiceInit() {
        AcLog.d(TAG, "[onServiceInit]");
    }

    @Override
    public void onFirstAudioFrame(String s) {
        AcLog.d(TAG, "[onFirstAudioFrame] audioStreamId: " + s);
    }

    @Override
    public void onFirstRemoteVideoFrame(String s) {
        AcLog.d(TAG, "[onFirstRemoteVideoFrame] videoStreamId: " + s);
    }

    @Override
    public void onStreamStarted() {
        AcLog.d(TAG, "[onStreamStarted]");
    }

    @Override
    public void onStreamPaused() {
        AcLog.d(TAG, "[onStreamPaused]");
    }

    @Override
    public void onStreamResumed() {
        AcLog.d(TAG, "[onStreamResumed]");
    }

    @Override
    public void onStreamStats(StreamStats streamStats) {
        AcLog.d(TAG, "[onStreamStats] streamStats: " + streamStats);
    }

    @Override
    public void onLocalStreamStats(LocalStreamStats localStreamStats) {
        AcLog.d(TAG, "[onLocalStreamStats] localStreamStats: " + localStreamStats);
    }

    @Override
    public void onStreamConnectionStateChanged(int i) {
        AcLog.d(TAG, "[onStreamConnectionStateChanged] connectionState: " + i);
    }

    @Override
    public void onDetectDelay(long l) {
        AcLog.d(TAG, "[onDetectDelay] detectDelay: " + l);
    }

    @Override
    public void onRotation(int i) {
        AcLog.d(TAG, "[onRotation] rotation: " + i);
        setRotation(i);
    }

    @Override
    public void onPodExit(int i, String s) {
        AcLog.d(TAG, "[onPodExit] errorCode: " + i + ", errorMessage: " + s);
    }

    @Override
    public void onNetworkQuality(int i) {
        AcLog.d(TAG, "[onNetworkQuality] quality: " + i);
    }
}
