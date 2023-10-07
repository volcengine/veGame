package com.volcengine.vegameengine.feature;

import android.content.res.Configuration;
import android.os.Bundle;
import android.os.Environment;
import android.view.View;
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
import com.volcengine.cloudphone.apiservice.IFileChannelExt;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.base.BasePlayActivity;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.io.File;
import java.util.HashMap;
import java.util.List;
import java.util.Map;


/**
 * 该类用于展示与大文件通道{@link IFileChannelExt}相关的功能接口，
 */
public class FileChannelExtActivity extends BasePlayActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = "FileChannelExtActivity";

    private FrameLayout mContainer;
    private GamePlayConfig mGamePlayConfig;
    private GamePlayConfig.Builder mBuilder;
    private IFileChannelExt mFileChannelExt;
    private SwitchCompat mSwShowOrHide;
    private LinearLayoutCompat mLlButtons;
    private Button mBtnStartSendFile, mBtnStopSendFile, mBtnStopReceiveFile;
    private File mSendFile, mReceiveFile;
    private Map<String, String> mOptions = new HashMap<>(); // 发送文件时附带的参数


    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_file_channel_ext);
        initView();
        initGamePlayConfig();
    }

    private void initView() {
        mContainer = findViewById(R.id.container);
        mSwShowOrHide = findViewById(R.id.sw_show_or_hide);
        mLlButtons = findViewById(R.id.ll_buttons);
        mBtnStartSendFile = findViewById(R.id.btn_start_send_file);
        mBtnStopSendFile = findViewById(R.id.btn_stop_send_file);
        mBtnStopReceiveFile = findViewById(R.id.btn_stop_receive_file);

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mLlButtons.setVisibility(isChecked ? View.VISIBLE : View.GONE);
        });

        mBtnStartSendFile.setOnClickListener(view -> {
            if (mFileChannelExt != null) {
                mSendFile = new File(Environment.getExternalStorageDirectory(), "Download/test.jpg");
                if (mSendFile.exists()) {
                    /**
                     * startSendFile(File file, map<String, String> options, ISendFileListener listener) -- 开始发送本地文件到远端实例
                     *
                     * @param file 发送的本地文件，远端实例的接收目录为/sdcard/download/
                     * @param listener 发送文件的进度及结果监听器
                     *
                     */
                    mFileChannelExt.startSendFile(mSendFile, mOptions, new IFileChannelExt.ISendFileListener() {
                        /**
                         * 发送文件开始回调
                         *
                         * @param file 发送的本地文件
                         * @param options 附带的options参数
                         */
                        @Override
                        public void onStart(File file, Map<String, String> options) {
                            AcLog.d(TAG, "[ISendFileListener#onStart] file: "
                                    + file.getAbsolutePath() + ", options: "  + options);
                            showToast("[ISendFileListener#onStart] file: "
                                    + file.getAbsolutePath() + ", options: "  + options);
                        }

                        /**
                         * 发送文件进度更新回调
                         *
                         * @param file 发送的本地文件
                         * @param options 附带的options参数
                         * @param progress 发送文件的进度，取值范围[0, 100]
                         */
                        @Override
                        public void onProgress(File file, Map<String, String> options, int progress) {
                            AcLog.d(TAG, "[ISendFileListener#onProgress] file: "
                                    + file.getAbsolutePath() + ", options: "  + options +
                                    ", progress: " + progress);
                            showToast("[ISendFileListener#onProgress] file: "
                                    + file.getAbsolutePath() + ", options: "  + options +
                                    ", progress: " + progress);
                        }

                        /**
                         * 发送文件完成回调
                         *
                         * @param file 发送的本地文件
                         * @param options 附带的options参数
                         */
                        @Override
                        public void onComplete(File file, Map<String, String> options) {
                            AcLog.d(TAG, "[ISendFileListener#onComplete] file: "
                                    + file.getAbsolutePath() + ", options: "  + options);
                            showToast("[ISendFileListener#onComplete] file: "
                                    + file.getAbsolutePath() + ", options: "  + options);
                        }

                        /**
                         * 发送文件取消回调
                         *
                         * @param file 发送的本地文件
                         * @param options 附带的options参数
                         */
                        @Override
                        public void onCancel(File file, Map<String, String> options) {
                            AcLog.d(TAG, "[ISendFileListener#onCancel] file: "
                                    + file.getAbsolutePath() + ", options: "  + options);
                            showToast("[ISendFileListener#onCancel] file: "
                                    + file.getAbsolutePath() + ", options: "  + options);
                        }

                        /**
                         * 发送文件失败回调
                         *
                         * @param file 发送的本地文件
                         * @param options 附带的options参数
                         * @param errorCode 发送文件失败的错误码
                         */
                        @Override
                        public void onError(File file, Map<String, String> options, int errorCode) {
                            AcLog.d(TAG, "[ISendFileListener#onError] file: " + file.getAbsolutePath() +
                                    ", options: "  + options + ", errorCode: " + errorCode);
                            showToast("[ISendFileListener#onError] file: " + file.getAbsolutePath() +
                                    ", options: "  + options + ", errorCode: " + errorCode);
                        }
                    });
                }
                else {
                    showToast("文件不存在");
                }
            }
        });

        mBtnStopSendFile.setOnClickListener(view -> {
            if (mFileChannelExt != null) {
                if (mSendFile.exists()) {
                    /**
                     * stopSendFile(File file) -- 停止发送本地文件到远端实例
                     *
                     * @param file 发送的本地文件
                     */
                    mFileChannelExt.stopSendFile(mSendFile);
                }
                else {
                    showToast("文件不存在");
                }
            }
        });

        mBtnStopReceiveFile.setOnClickListener(view -> {
            if (mFileChannelExt != null) {
                /**
                 * stopReceiveFile(File file) -- 停止接收远端实例发送到本地的文件
                 *
                 * @param file 远端实例发送的文件
                 */
                mFileChannelExt.stopReceiveFile(mReceiveFile);
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
        mFileChannelExt = VeGameEngine.getInstance().getFileChannelExt();
        /**
         * setReceiveFileListener(IReceiveFileListener listener) -- 设置接收文件回调监听器
         */
        if (mFileChannelExt != null) {
            mFileChannelExt.setReceiveFileListener(new IFileChannelExt.IReceiveFileListener() {
                /**
                 * 接收文件开始回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 */
                @Override
                public void onStart(File file, Map<String, String> options) {
                    mReceiveFile = file;
                    AcLog.d(TAG, "[IReceiveFileListener#onStart] file: " +
                            file.getAbsolutePath() + ", options: " + options);
                    showToast("[IReceiveFileListener#onStart] file: " +
                            file.getAbsolutePath() + ", options: " + options);
                }

                /**
                 * 接收文件进度更新回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 * @param progress 接收文件的进度，取值范围[0, 100]
                 */
                @Override
                public void onProgress(File file, Map<String, String> options, int progress) {
                    mReceiveFile = file;
                    AcLog.d(TAG, "[IReceiveFileListener#onProgress] file: " +
                            file.getAbsolutePath() + ", options: " + options + ", progress: " + progress);
                    showToast("[IReceiveFileListener#onProgress] file: " +
                            file.getAbsolutePath() + ", options: " + options + ", progress: " + progress);
                }

                /**
                 * 接收文件完成回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 */
                @Override
                public void onComplete(File file, Map<String, String> options) {
                    mReceiveFile = file;
                    AcLog.d(TAG, "[IReceiveFileListener#onComplete] file: " +
                            file.getAbsolutePath() + ", options: " + options);
                    showToast("[IReceiveFileListener#onComplete] file: " +
                            file.getAbsolutePath() + ", options: " + options);
                }

                /**
                 * 接收文件取消回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 */
                @Override
                public void onCancel(File file, Map<String, String> options) {
                    mReceiveFile = file;
                    AcLog.d(TAG, "[IReceiveFileListener#onCancel] file: " +
                            file.getAbsolutePath() + ", options: " + options);
                    showToast("[IReceiveFileListener#onCancel] file: " +
                            file.getAbsolutePath() + ", options: " + options);
                }

                /**
                 * 接收文件失败回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 * @param errorCode 接收文件失败的错误码
                 */
                @Override
                public void onError(File file, Map<String, String> options, int errorCode) {
                    mReceiveFile = file;
                    AcLog.d(TAG, "[IReceiveFileListener#onError] file: " +
                            file.getAbsolutePath() + ", options: " + options +
                            ", errorCode: " + errorCode);
                    showToast("[IReceiveFileListener#onError] file: " +
                            file.getAbsolutePath() + ", options: " + options +
                            ", errorCode: " + errorCode);
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
     * 同时，需要在 {@link ClarityServiceActivity#onConfigurationChanged(Configuration)} 回调中，
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
