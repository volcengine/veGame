package com.volcengine.vegameengine.feature;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.view.ViewGroup;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.LinearLayoutCompat;
import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudcore.common.mode.QueueInfo;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IMessageChannel;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.base.BasePlayActivity;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.Map;

/**
 * 该类用于展示与消息通道{@link IMessageChannel}相关的功能接口
 */
public class MessageChannelActivity extends BasePlayActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();

    private FrameLayout mContainer;
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
            e.printStackTrace();
        }

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

    /**
     * 播放成功回调
     *
     * @param roundId 当次游戏生命周期标识符
     * @param clarityId 当前游戏画面的清晰度，首帧渲染到画面时触发该回调
     * @param extraMap 自定义的扩展参数
     * @param gameId 游戏ID
     * @param reservedId 资源预锁定ID
     */
    @Override
    public void onPlaySuccess(String roundId, int clarityId, Map<String, String> extraMap, String gameId, String reservedId) {
        AcLog.d(TAG, "[onPlaySuccess] roundId " + roundId + " clarityId " + clarityId + "extra:" + extraMap +
                "gameId : " + gameId + " reservedId" + reservedId);
    }

    /**
     * SDK内部产生的错误回调
     *
     * @param errorCode 错误码
     * @param errorMessage 错误详情
     */
    @Override
    public void onError(int errorCode, String errorMessage) {
        AcLog.e(TAG, "[onError] errorCode: " + errorCode + ", errorMessage: " + errorMessage);
        Toast.makeText(this, "[onError] errorCode: " + errorCode + ", errorMessage: " + errorMessage, Toast.LENGTH_SHORT).show();
    }

    /**
     * SDK内部产生的警告回调
     *
     * @param warningCode 警告码
     * @param warningMessage 警告详情
     */
    @Override
    public void onWarning(int warningCode, String warningMessage) {
        AcLog.d(TAG, "[onWarning] warningCode: " + warningCode + ", warningMessage: " + warningMessage);
    }

    /**
     * 网络连接类型和状态切换回调
     *
     * @param networkType 当前的网络类型
     *         -1 -- 网络连接类型未知
     *          0 -- 网络连接已断开
     *          1 -- 网络类型为 LAN
     *          2 -- 网络类型为 Wi-Fi（包含热点）
     *          3 -- 网络类型为 2G 移动网络
     *          4 -- 网络类型为 3G 移动网络
     *          5 -- 网络类型为 4G 移动网络
     *          6 -- 网络类型为 5G 移动网络
     */
    @Override
    public void onNetworkChanged(int networkType) {
        AcLog.d(TAG, "[onNetworkChanged] networkType: " + networkType);
    }

    /**
     * 加入房间前回调，用于获取并初始化各个功能服务，例如设置各种事件监听回调。
     */
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

                @Override
                public void onReceiveBinaryMessage(IMessageChannel.IChannelBinaryMessage iChannelBinaryMessage) {

                }

                /**
                 * 发送消息结果回调
                 *
                 * @param success 是否发送成功
                 * @param messageId 消息ID
                 */
                @Override
                public void onSentResult(boolean success, String messageId) {
                    AcLog.d(TAG, "[onSentResult] success: " + success + ", messageId: " + messageId);
                    Toast.makeText(MessageChannelActivity.this, "[onSentResult] success: " + success + ", messageId: " + messageId, Toast.LENGTH_SHORT).show();
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
                 * @param errorCode 错误码
                 * @param errorMessage 错误信息
                 */
                @Override
                public void onError(int errorCode, String errorMessage) {
                    AcLog.d(TAG, "[onError] errorCode: " + errorCode + ", errorMessage: " + errorMessage);
                    Toast.makeText(MessageChannelActivity.this, "[onError] errorCode: " + errorCode + ", errorMessage: " + errorMessage, Toast.LENGTH_SHORT).show();
                }

                /**
                 * 云端游戏在线回调，建议在发送消息前监听该回调检查通道是否已连接
                 *
                 * @param channelUid 云端游戏的用户ID
                 */
                @Override
                public void onRemoteOnline(String channelUid) {
                    AcLog.d(TAG, "[onRemoteOnline] channelUid: " + channelUid);
                    Toast.makeText(MessageChannelActivity.this, "[onRemoteOnline] channelUid: " + channelUid, Toast.LENGTH_SHORT).show();
                }

                /**
                 * 云端游戏离线回调
                 *
                 * @param channelUid 云端游戏的用户ID
                 */
                @Override
                public void onRemoteOffline(String channelUid) {
                    AcLog.d(TAG, "[onRemoteOffline] channelUid: " + channelUid);
                    Toast.makeText(MessageChannelActivity.this, "[onRemoteOffline] channelUid: " + channelUid, Toast.LENGTH_SHORT).show();
                }
            });
        }
    }

    /**
     * 排队信息更新回调
     *
     * @param queueInfoList 当前的排队队列信息
     */
    @Override
    public void onQueueUpdate(List<QueueInfo> queueInfoList) {
        AcLog.d(TAG, "[onQueueUpdate] list: " + queueInfoList);
    }

    /**
     * 排队结束，开始申请资源的回调
     *
     * @param remainTime 当用户排到第0位时申请服务的等待时间，超过时间未进入会被移出队列
     */
    @Override
    public void onQueueSuccessAndStart(int remainTime) {
        AcLog.d(TAG, "[onQueueSuccessAndStart] remainTime: " + remainTime);
    }

    /**
     * 收到音频首帧时的回调
     *
     * @param audioStreamId 远端实例音频流的ID
     */
    @Override
    public void onFirstAudioFrame(String audioStreamId) {
        AcLog.d(TAG, "[onFirstAudioFrame] audioStreamId: " + audioStreamId);
    }

    /**
     * 收到视频首帧时的回调
     *
     * @param videoStreamId 远端实例视频流的ID
     */
    @Override
    public void onFirstRemoteVideoFrame(String videoStreamId) {
        AcLog.d(TAG, "[onFirstRemoteVideoFrame] videoStreamId: " + videoStreamId);
    }

    /**
     * 开始播放的回调
     */
    @Override
    public void onStreamStarted() {
        AcLog.d(TAG, "[onStreamStarted]");
    }

    /**
     * 暂停播放后的回调，调用{@link VeGameEngine#pause()}后会触发
     */
    @Override
    public void onStreamPaused() {
        AcLog.d(TAG, "[onStreamPaused]");
    }

    /**
     * 恢复播放后的回调，调用{@link VeGameEngine#resume()} 或 VeGameEngine#muteAudio(false) 后会触发
     */
    @Override
    public void onStreamResumed() {
        AcLog.d(TAG, "[onStreamResumed]");
    }

    /**
     * 周期为2秒的音视频网络状态的回调，可用于内部数据分析或监控
     *
     * @param streamStats 远端视频流的性能状态
     */
    @Override
    public void onStreamStats(StreamStats streamStats) {
        AcLog.d(TAG, "[onStreamStats] streamStats: " + streamStats);
    }

    /**
     * 周期为2秒的本地推送的音视频流的状态回调
     *
     * @param localStreamStats 本地音视频流的性能状态
     */
    @Override
    public void onLocalStreamStats(LocalStreamStats localStreamStats) {
        AcLog.d(TAG, "[onLocalStreamStats] localStreamStats: " + localStreamStats);
    }

    /**
     * 视频流连接状态变化
     *
     * @param state 视频流连接状态
     *              1 -- 连接断开
     *              2 -- 首次连接，正在连接中
     *              3 -- 首次连接成功
     *              4 -- 连接断开后，重新连接中
     *              5 -- 连接断开后，重新连接成功
     *              6 -- 连接断开超过10秒，但仍然会继续连接
     *              7 -- 连接失败，不会继续连接
     */
    @Override
    public void onStreamConnectionStateChanged(int state) {
        AcLog.d(TAG, "[onStreamConnectionStateChanged] connectionState: " + state);
    }

    /**
     * 操作延迟回调
     *
     * @param elapse 操作延迟的具体值，单位:毫秒
     */
    @Override
    public void onDetectDelay(long elapse) {
        AcLog.d(TAG, "[onDetectDelay] detectDelay: " + elapse);
    }

    /**
     * 客户端的旋转回调
     *
     * 远端实例通过该回调向客户端发送视频流的方向(横屏或竖屏)，为保证视频流方向与Activity方向一致，
     * 需要在该回调中根据rotation参数，调用 {@link BasePlayActivity#setRotation(int)} 来调整Activity的方向，
     * 0/180需将Activity调整为竖屏，90/270则将Activity调整为横屏；
     * 同时，需要在 {@link MessageChannelActivity#onConfigurationChanged(Configuration)} 回调中，
     * 根据当前Activity的方向，调用 {@link VeGameEngine#rotate(int)} 来调整视频流的方向。
     *
     * @param rotation 旋转方向
     *          0, 180 -- 竖屏
     *         90, 270 -- 横屏
     */
    @Override
    public void onRotation(int rotation) {
        AcLog.d(TAG, "[onRotation] rotation: " + rotation);
        setRotation(rotation);
    }

    /**
     * 远端实例退出回调
     *
     * @param reasonCode 退出的原因码
     * @param reasonMessage 退出的原因详情
     */
    @Override
    public void onPodExit(int reasonCode, String reasonMessage) {
        AcLog.d(TAG, "[onPodExit] reasonCode: " + reasonCode + ", reasonMessage: " + reasonMessage);
    }

    /**
     * 周期为2秒的游戏中的网络质量回调
     *
     * @param quality 网络质量评级
     *                0 -- 网络状况未知，无法判断网络质量
     *                1 -- 网络状况极佳，能够高质量承载当前业务
     *                2 -- 当前网络状况良好，能够较好地承载当前业务
     *                3 -- 当前网络状况有轻微劣化，但不影响正常使用
     *                4 -- 当前网络质量欠佳，会影响当前业务的主观体验
     *                5 -- 当前网络已经无法承载当前业务的媒体流，需要采取相应策略，
     *                      比如降低媒体流的码率或者更换网络
     *                6 -- 当前网络完全无法正常通信
     */
    @Override
    public void onNetworkQuality(int quality) {
        AcLog.d(TAG, "[onNetworkQuality] quality: " + quality);
    }
}
