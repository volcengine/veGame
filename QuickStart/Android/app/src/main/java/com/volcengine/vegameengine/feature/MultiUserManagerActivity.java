package com.volcengine.vegameengine.feature;

import android.content.res.Configuration;
import android.os.Bundle;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.LinearLayoutCompat;
import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudcore.common.mode.QueueInfo;
import com.volcengine.cloudcore.common.mode.Role;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;
import com.volcengine.cloudphone.apiservice.outinterface.MultiUserService;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.base.BasePlayActivity;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.ScreenUtil;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.List;
import java.util.Locale;
import java.util.Map;

/**
 * 该类用于展示与多用户{@link MultiUserService}相关的功能接口
 *
 * 多用户功能适用于多人游戏及转移游戏控制权的场景。
 * 需要指定roomType(房间类型)、role(玩家角色)以及reservedId(资源预锁定ID)，
 * 可以通过{@link com.volcengine.cloudgame.GamePlayConfig.Builder}来设置。
 *
 * (1) roomType有三种，0--单房间单用户(默认)，1--单房间多用户(不可转移游戏控制权)，
 * 2--单房间多用户(可转移游戏控制权)。
 * (2) role有两种，操作者{@link Role#PLAYER}和观看者{@link Role#VIEWER}，
 * 当roomType为1或2时，指定玩家角色才有效，如不指定玩家角色，则默认为观看者。
 * (3) reservedId可以在{@link IGamePlayerListener#onPlaySuccess(String, int, Map, String, String)}
 * 回调中获取，或者调用服务端PreAllocateResource接口获取。要保证进入同一游戏的用户使用相同的reservedId。
 *
 * 指定roomType和role后，可以在游戏中调用
 * {@link MultiUserService#changeRole(String, Role, MultiUserService.ChangeRoleCallBack)}接口
 * 将观看者设置为操作者，以转移游戏控制权。
 *
 * 注意: 进入游戏的观看者也会占用实例资源，建议控制观看者数量。
 */
public class MultiUserManagerActivity extends BasePlayActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = "MultiUserManagerActivity";

    private FrameLayout mContainer;
    private GamePlayConfig mGamePlayConfig;
    private GamePlayConfig.Builder mBuilder;
    private MultiUserService mMultiUserManager;
    private Role mRole = Role.PLAYER;
    private SwitchCompat mSwShowOrHide;
    private LinearLayoutCompat mLlButtons;
    private RadioGroup mRgChooseRole;
    private EditText mEtUserId;
    private Button mBtnUpdateRole, mBtnGetCurrentRole;
    private TextView mTvCurrentRole;

    @Override
    protected void onCreate(@Nullable Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        ScreenUtil.adaptHolePhone(this);
        setContentView(R.layout.activity_multi_user);
        initView();
        initGamePlayConfig();
    }

    private void initView() {
        mContainer = findViewById(R.id.container);
        mSwShowOrHide = findViewById(R.id.sw_show_or_hide);
        mLlButtons = findViewById(R.id.ll_buttons);

        mSwShowOrHide.setOnCheckedChangeListener((buttonView, isChecked) -> {
            mLlButtons.setVisibility(isChecked ? View.VISIBLE : View.GONE);
        });

        mRgChooseRole = findViewById(R.id.rg_choose_role);
        mRgChooseRole.setOnCheckedChangeListener((group, checkedId) -> {
            if (checkedId == R.id.rb_player) {
                mRole = Role.PLAYER;
            } else if (checkedId == R.id.rb_viewer) {
                mRole = Role.VIEWER;
            }
        });

        /**
         * changeRole(String userId, Role role, ChangeRoleCallback callback) -- 修改用户的角色
         *
         * @param userId 用户ID
         * @param role 用户角色
         *             VIEWER(0) -- 观看者
         *             PLAYER(1) -- 操作者
         * @param callback 用户角色修改的回调
         */
        mBtnUpdateRole = findViewById(R.id.btn_update_role);
        mEtUserId = findViewById(R.id.et_userId);
        mBtnUpdateRole.setOnClickListener(v -> {
            mMultiUserManager.changeRole(mEtUserId.getText().toString(), mRole, (userId, role, result) -> {
                showToast("result " + result + ", userId " + userId + ", role " + role);
                AcLog.d(TAG, "change result " + userId + ", role " + role + "result " + result);
            });
        });

        /**
         * getCurrentRole() -- 获取当前用户的角色
         */
        mBtnGetCurrentRole = findViewById(R.id.btn_get_current_role);
        mTvCurrentRole = findViewById(R.id.tv_current_role);
        mBtnGetCurrentRole.setOnClickListener(v -> {
            mTvCurrentRole.setText(mMultiUserManager.getCurrentRole().toString());
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
                .role(mRole)
                .roomType(2)
                .reservedId("")
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
        AcLog.d(TAG, "[onPlaySuccess] roundId: " + roundId + ", clarityId: " + clarityId + ", extra:" + extraMap +
                ", gameId : " + gameId + ", reservedId" + reservedId);
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
        mMultiUserManager = VeGameEngine.getInstance().getMultiUserService();
        if (mMultiUserManager != null) {
            mMultiUserManager.setRoomListener(new MultiUserService.RoomListener() {
                /**
                 * 用户角色发生变化时的回调
                 *
                 * @param userId 发生变化的用户ID
                 */
                @Override
                public void onPlayerChanged(String userId) {
                    AcLog.d(TAG, "[onPlayerChanged] userId: " + userId);
                    showToast("onPlayerChanged" + userId);
                }

                /**
                 * 用户加入房间并收到首帧后，获得用户角色的回调
                 *
                 * @param role 用户获得的角色
                 * @param reason 0 -- 成功，获得角色与请求的一致
                 *               else -- 失败的具体原因
                 * @param playerUid 当前的操作者ID
                 */
                @Override
                public void onJoinRoomRoleResult(Role role, int reason, String playerUid) {
                    AcLog.d(TAG, "[onJoinRoomRoleResult] role: " + role + ", reason: " + reason + ", playerUid: " + playerUid);
                    showToast(String.format(Locale.getDefault(), "onJoinRoomRoleResult role: %d, reason : %d, playerUid %S",
                            role.id, reason, playerUid));
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
     * 同时，需要在 {@link MultiUserManagerActivity#onConfigurationChanged(Configuration)} 回调中，
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

    /**ro
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
