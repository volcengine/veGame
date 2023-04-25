package com.volcengine.vegameengine.feature;

import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_LANDSCAPE;
import static android.content.pm.ActivityInfo.SCREEN_ORIENTATION_SENSOR_PORTRAIT;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.app.AppCompatActivity;
import androidx.appcompat.widget.LinearLayoutCompat;
import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IMessageChannel;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Map;

/**
 * 该类用于展示与消息通道{@link IMessageChannel}相关的功能接口
 */
public class MessageChannelActivity extends AppCompatActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();

    private ViewGroup mContainer;
    private GamePlayConfig mGamePlayConfig;
    private GamePlayConfig.Builder mBuilder;
    private IMessageChannel mMessageChannel;
    private SwitchCompat mSwShowOrHide;
    private LinearLayoutCompat mLlButtons;
    private Button mBtnAckMsg, mBtnUidAckMsg, mBtnTimeoutMsg, mBtnUidTimeoutMsg;


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
        mSwShowOrHide = findViewById(R.id.sw_show_or_hide);
        mLlButtons = findViewById(R.id.ll_buttons);
        mBtnAckMsg = findViewById(R.id.btn_ack_msg);
        mBtnUidAckMsg = findViewById(R.id.btn_uid_ack_msg);
        mBtnTimeoutMsg = findViewById(R.id.btn_timeout_msg);
        mBtnUidTimeoutMsg = findViewById(R.id.btn_uid_timeout_msg);

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mLlButtons.setVisibility(isChecked ? View.VISIBLE : View.GONE);
        });

        String channelUid = "com.bytedance.vemessagechannelprj.prj1";
        mBtnAckMsg.setOnClickListener(v -> {
            if (mMessageChannel != null) {
                /**
                 * 发送回执消息到云端游戏(当云端只有一个游戏注册消息通道时使用)
                 *
                 * @param payload 发送内容，size：60KB
                 * @param needAck 是否需要云端Ack回执
                 * @return 消息实体
                 */
                IMessageChannel.IChannelMessage ackMsg =
                        mMessageChannel.sendMessage("ackMsg", true);
                AcLog.d(TAG, "ackMsg: " + ackMsg);
            }
        });
        mBtnUidAckMsg.setOnClickListener(v -> {
            if (mMessageChannel != null) {
                /**
                 * 发送回执消息到云端游戏(当云端有多个游戏注册消息通道时使用，需要指定目标用户ID，即应用包名)
                 *
                 * @param payload        发送内容，size：60KB
                 * @param needAck        是否需要云端Ack回执
                 * @param destChannelUid 目标用户消息通道ID
                 * @return 消息实体
                 */
                IMessageChannel.IChannelMessage uidAckMsg =
                        mMessageChannel.sendMessage("uidAckMsg", true, channelUid);
                AcLog.d(TAG, "uidAckMsg: " + uidAckMsg);
            }
        });
        mBtnTimeoutMsg.setOnClickListener(v -> {
            if (mMessageChannel != null) {
                /**
                 * 发送超时消息到云端游戏(当云端只有一个游戏注册消息通道时使用)
                 *
                 * @param payload 发送内容，size：60KB
                 * @param timeout 消息超时时长，单位：ms，需要大于0；当小于等于0时，通过
                 *                  {@link com.volcengine.cloudphone.apiservice.IMessageChannel.IMessageReceiver#onError(int, String)}
                 *                  返回错误信息
                 * @return 消息实体
                 */
                IMessageChannel.IChannelMessage timeoutMsg =
                        mMessageChannel.sendMessage("timeoutMsg", 3000);
                AcLog.d(TAG, "timeoutMsg: " + timeoutMsg);
            }
        });
        mBtnUidTimeoutMsg.setOnClickListener(v -> {
            if (mMessageChannel != null) {
                /**
                 * 发送超时消息到云端游戏(当云端有多个游戏注册消息通道时使用，需要指定目标用户ID，即应用包名)
                 *
                 * @param payload        发送内容，size：60KB
                 * @param timeout        消息超时时长，单位：ms，需要大于0；当小于等于0时，通过
                 *                         {@link com.volcengine.cloudphone.apiservice.IMessageChannel.IMessageReceiver#onError(int, String)}
                 *                         返回错误信息
                 * @param destChannelUid 目标用户消息通道ID
                 * @return 消息实体
                 */
                IMessageChannel.IChannelMessage uidTimeoutMsg =
                        mMessageChannel.sendMessage("uidTimeoutMsg", 3000, channelUid);
                AcLog.d(TAG, "uidTimeoutMsg: " + uidTimeoutMsg);
            }
        });
    }

    private void initGamePlayConfig() {
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

        String gameId = "7212894215896849207";
        String roundId = "roundId_123";
        String userId = "userId_" + System.currentTimeMillis();

        mBuilder = new GamePlayConfig.Builder();
        mBuilder.userId(userId)
                .ak(ak)
                .sk(sk)
                .token(token)
                .container(mContainer)
                .roundId(roundId)
                .gameId(gameId)
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
        AcLog.e(TAG, "[onError] errorCode: " + i + ", errorMsg: " + s);
    }

    @Override
    public void onWarning(int i, String s) {
        AcLog.d(TAG, "[onWarning] errorCode: " + i + ", errorMsg: " + s);
    }

    @Override
    public void onNetworkChanged(int i) {
        AcLog.d(TAG, "[onNetworkChanged] network: " + i);
    }

    @Override
    public void onServiceInit() {
        AcLog.d(TAG, "[onServiceInit]");
        mMessageChannel = VeGameEngine.getInstance().getMessageChannel();
        if (mMessageChannel != null) {
            /**
             * 设置消息接收回调监听
             *
             * @param listener 消息接收回调监听器
             */
            mMessageChannel.setMessageListener(new IMessageChannel.IMessageReceiver() {
                /**
                 * 消息接收回调
                 *
                 * @param iChannelMessage 接收的消息实体
                 */
                @Override
                public void onReceiveMessage(IMessageChannel.IChannelMessage iChannelMessage) {
                    AcLog.d(TAG, "[onReceiveMessage] message: " + iChannelMessage);
                    Toast.makeText(MessageChannelActivity.this, "[onReceiveMessage] message: " + iChannelMessage, Toast.LENGTH_SHORT).show();
                }

                /**
                 * 发送消息结果回调
                 *
                 * @param b 是否发送成功
                 * @param s 消息ID
                 */
                @Override
                public void onSentResult(boolean b, String s) {
                    AcLog.d(TAG, "[onSentResult] success: " + b + ", mid: " + s);
                    Toast.makeText(MessageChannelActivity.this, "[onSentResult] success: " + b + ", mid: " + s, Toast.LENGTH_SHORT).show();
                }

                /**
                 * 已弃用，可忽略
                 */
                @Override
                public void ready() {
                    AcLog.d(TAG, "[ready]");
                }

                /**
                 * 错误信息回调
                 *
                 * @param i 错误码
                 * @param s 错误信息
                 */
                @Override
                public void onError(int i, String s) {
                    AcLog.d(TAG, "[onError] errorCode: " + i + ", errorMsg: " + s);
                    Toast.makeText(MessageChannelActivity.this, "[onError] errorCode: " + i + ", errorMsg: " + s, Toast.LENGTH_SHORT).show();
                }

                /**
                 * 云端游戏在线回调，建议在发送消息前监听该回调检查通道是否已连接
                 *
                 * @param s 云端游戏的用户ID
                 */
                @Override
                public void onRemoteOnline(String s) {
                    AcLog.d(TAG, "[onRemoteOnline] channelUid: " + s);
                    Toast.makeText(MessageChannelActivity.this, "[onRemoteOnline] channelUid: " + s, Toast.LENGTH_SHORT).show();
                }

                /**
                 * 云端游戏离线回调
                 *
                 * @param s 云端游戏的用户ID
                 */
                @Override
                public void onRemoteOffline(String s) {
                    AcLog.d(TAG, "[onRemoteOffline] channelUid: " + s);
                    Toast.makeText(MessageChannelActivity.this, "[onRemoteOffline] channelUid: " + s, Toast.LENGTH_SHORT).show();
                }
            });
        }
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
        AcLog.d(TAG, "[onPodExit] errorCode: " + i + ", errorMsg: " + s);
    }

    @Override
    public void onNetworkQuality(int i) {
        AcLog.d(TAG, "[onNetworkQuality] quality: " + i);
    }
}
