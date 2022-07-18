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

import android.app.Activity;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Bundle;
import android.util.Log;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;

import com.volcengine.vegameengine.util.ScreenUtil;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import java.util.Map;

public class GameActivity extends AppCompatActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();
    private ViewGroup mContainer;
    public static final String KEY_PARAM_GAME_ID = "gameId";
    public static final String KEY_ROUND_ID = "roundId";
    public static final String KEY_ClARITY_ID = "clarity_id";

    public VeGameEngine veGameEngine = VeGameEngine.getInstance();

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_play);
        mContainer = findViewById(R.id.container);
        intConfig();
    }

    private void intConfig() {
        GamePlayConfig.Builder builder = new GamePlayConfig.Builder();
        String userId = "userid" + System.currentTimeMillis();
        AcLog.d(TAG, "userId: " + userId);
        Intent intent = getIntent();
        builder.userId(userId) // 用户userid
                .ak("")  // 必填参数，申请云游戏服务临时 ak
                .sk("")  // 必填参数，申请云游戏服务临时 sk
                .token("") //必填参数，申请云游戏服务临时 token
                .container(mContainer)//必填参数，用来承载画面的 Container, 参数说明: layout 需要是FrameLayout或者FrameLayout的子类
                .roundId(intent.getStringExtra(KEY_ROUND_ID))//必填参数，自定义roundId
                .videoStreamProfileId(intent.getIntExtra(KEY_ClARITY_ID, 1)) // 选填参数，清晰度ID
                .gameId(intent.getStringExtra(KEY_PARAM_GAME_ID)) //必填 gameId
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

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
    }

    @Override
    public void onPlaySuccess(String roundId, int clarityId, Map<String, String> map, String gameId,
                              String reservedId) {
        AcLog.d(TAG, "roundId " + roundId + " clarityId " + clarityId + "extra:" + map +
                "gameId : " + gameId + " reservedId" + reservedId);
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
    }

    @Override
    public void onPodExit(int i, String s) {
        Log.d(TAG, "onPodExit" + i + " ,msg:" + s);
    }
}
