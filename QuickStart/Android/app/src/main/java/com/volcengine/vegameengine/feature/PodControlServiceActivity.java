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
import com.volcengine.cloudcore.common.mode.SessionMode;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.GamePodControlService;
import com.volcengine.cloudphone.apiservice.PodControlService;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.base.BasePlayActivity;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;


/**
 * 该类用于展示与实例控制{@link PodControlService}和{@link GamePodControlService}相关的功能接口
 */
public class PodControlServiceActivity extends BasePlayActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = "PodControlServiceActivity";

    private FrameLayout mContainer;
    private GamePlayConfig mGamePlayConfig;
    private GamePlayConfig.Builder mBuilder;
    private PodControlService mPodControlService;
    private GamePodControlService mGamePodControlService;
    private SwitchCompat mSwShowOrHide;
    private Button mBtnSwitchBackground, mBtnSwitchForeground, mBtnSetIdleTime,
            mBtnGetAutoRecycleTime, mBtnSetAutoRecycleTime, mBtnGetUserProfilePath, mBtnSetUserProfilePath,
            mBtnSwitchAFKMode, mBtnSwitchNormalMode;
    private LinearLayoutCompat mLlButtons;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_pod_control);
        initView();
        initGamePlayConfig();
    }

    private void initView() {
        mContainer = findViewById(R.id.container);
        mSwShowOrHide = findViewById(R.id.sw_show_or_hide);
        mLlButtons = findViewById(R.id.ll_buttons);
        mBtnSwitchBackground = findViewById(R.id.btn_switch_background);
        mBtnSwitchForeground = findViewById(R.id.btn_switch_foreground);
        mBtnSetIdleTime = findViewById(R.id.btn_set_idle_time);
        mBtnGetAutoRecycleTime = findViewById(R.id.btn_get_auto_recycle_time);
        mBtnSetAutoRecycleTime = findViewById(R.id.btn_set_auto_recycle_time);
        mBtnGetUserProfilePath = findViewById(R.id.btn_get_user_profile);
        mBtnSetUserProfilePath = findViewById(R.id.btn_set_user_profile);
        mBtnSwitchAFKMode = findViewById(R.id.btn_switch_afk_mode);
        mBtnSwitchNormalMode = findViewById(R.id.btn_switch_normal_mode);

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mLlButtons.setVisibility(isChecked ? View.VISIBLE : View.GONE);
        });

        /**
         * switchBackground(boolean on) -- 设置客户端应用或游戏切换前后台的状态
         *
         * @param on true -- 切后台
         *           false -- 切前台
         */
        mBtnSwitchBackground.setOnClickListener(view -> {
            if (mPodControlService != null) {
                mPodControlService.switchBackground(true);
            }
        });
        mBtnSwitchForeground.setOnClickListener(view -> {
            if (mPodControlService != null) {
                mPodControlService.switchBackground(false);
            }
        });

        /**
         * setIdleTime(long time) -- 设置客户端切后台之后，云端游戏的保活时间
         *
         * @param time 保活时长，单位秒
         */
        mBtnSetIdleTime.setOnClickListener(v -> {
            if (mPodControlService != null) {
                int idleTime = 10;
                mPodControlService.setIdleTime(idleTime);
            }
        });

        /**
         * setAutoRecycleTime(int time, SetAutoRecycleTimeCallback callback) -- 设置无操作回收服务时长
         *
         * @param time 无操作回收服务时长，单位秒
         * @param callback 设置无操作回收服务时长的回调
         * @return 0 -- 正常返回
         *        -1 -- 内部错误
         *        -2 -- time参数小于0
         */
        mBtnSetAutoRecycleTime.setOnClickListener(v -> {
            if (mPodControlService != null) {
                int autoRecycleTime = 20;
                mPodControlService.setAutoRecycleTime(autoRecycleTime, new PodControlService.SetAutoRecycleTimeCallback() {
                    @Override
                    public void onResult(int result, long time) {
                        showToast("[setAutoRecycleTimeResult] result: " + result + ", time: " + time);
                    }
                });
            }
        });

        /**
         * getAutoRecycleTime(GetAutoRecycleTimeCallback callback) -- 查询无操作回收服务时长
         *
         * @param callback 查询无操作回收服务时长的回调
         * @return 0 -- 正常返回
         *        -1 -- 内部错误
         */
        mBtnGetAutoRecycleTime.setOnClickListener(v -> {
            if (mPodControlService != null) {
                mPodControlService.getAutoRecycleTime(new PodControlService.GetAutoRecycleTimeCallback() {
                    @Override
                    public void onResult(int result, long time) {
                        showToast("[getAutoRecycleTimeResult] result: " + result + ", time: " + time);
                    }
                });
            }
        });

        /**
         * getUserProfilePath(GetUserProfilePathListener userProfilePathListener) -- 获取保存游戏云端配置文件的路径
         *
         * @param userProfilePathListener 获取保存游戏云端配置文件的路径的监听器
         * @return 0 -- 成功返回
         *         else -- 发生错误
         */
        mBtnGetUserProfilePath.setOnClickListener(v -> {
            if (mPodControlService != null) {
                mPodControlService.getUserProfilePath(list -> {
                    showToast(list.toString());
                });
            }
        });

        /**
         * setUserProfilePath(String[] userProfilePath) -- 设置保存游戏云端配置文件的路径
         *
         * @param userProfilePath 保存配置文件的路径列表
         */
        mBtnSetUserProfilePath.setOnClickListener(v -> {
            if (mPodControlService != null) {
                List<String> list = new ArrayList<>();
                list.add("/");
                mPodControlService.setUserProfilePath(list);
            }
        });

        /**
         * setSessionMode(int sessionMode) -- 设置挂机模式
         *
         * @param sessionMode 挂机模式，参见{@link SessionMode}
         *                    0 -- 正常模式(非挂机)
         *                    1 -- 挂机模式
         */
        mBtnSwitchAFKMode.setOnClickListener(v -> {
            if (mGamePodControlService != null) {
                mGamePodControlService.setSessionMode(SessionMode.SESSION_MODE_AFK);
            }
        });
        mBtnSwitchNormalMode.setOnClickListener(v -> {
            if (mGamePodControlService != null) {
                mGamePodControlService.setSessionMode(SessionMode.SESSION_MODE_NORMAL);
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
        mPodControlService = VeGameEngine.getInstance().getPodControlService();
        if (mPodControlService != null) {
            /**
             * setBackgroundSwitchListener(BackgroundSwitchListener listener) -- 设置云端应用切换前后台监听器
             */
            mPodControlService.setBackgroundSwitchListener(new PodControlService.BackgroundSwitchListener() {
                /**
                 * 云端应用切换前后台的回调
                 *
                 * @param on true -- 切换到后台
                 *          false -- 切换到前台
                 */
                @Override
                public void onBackgroundSwitched(boolean on) {
                    AcLog.d(TAG, "[onBackgroundSwitched] isBackground: " + on);
                    showToast("[onBackgroundSwitched] isBackground: " + on);
                }
            });
        }

        mGamePodControlService = VeGameEngine.getInstance().getGamePodControlService();
        if (mGamePodControlService != null) {
            /**
             * setListener(SessionModeListener listener) -- 设置切换挂机模式的监听器
             */
            mGamePodControlService.setListener(new GamePodControlService.SessionModeListener() {
                /**
                 * 切换挂机模式结果的回调
                 *
                 * @param mode 挂机模式，参见{@link SessionMode}
                 * @param result 切换结果
                 *               true -- 成功
                 *               false -- 失败
                 */
                @Override
                public void onResult(int mode, boolean result) {
                    AcLog.d(TAG, "[onResult] mode: " + mode + ", result: " + result);
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
