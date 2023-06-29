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

import static com.volcengine.vegameengine.util.Feature.FEATURE_AUDIO;
import static com.volcengine.vegameengine.util.Feature.FEATURE_CAMERA;
import static com.volcengine.vegameengine.util.Feature.FEATURE_CLIPBOARD;
import static com.volcengine.vegameengine.util.Feature.FEATURE_FILE_CHANNEL;
import static com.volcengine.vegameengine.util.Feature.FEATURE_FILE_CHANNEL_EXT;
import static com.volcengine.vegameengine.util.Feature.FEATURE_LOCATION;
import static com.volcengine.vegameengine.util.Feature.FEATURE_MULTI_USER;
import static com.volcengine.vegameengine.util.Feature.FEATURE_POD_CONTROL;
import static com.volcengine.vegameengine.util.Feature.FEATURE_PROBE_NETWORK;
import static com.volcengine.vegameengine.util.Feature.FEATURE_UNCLASSIFIED;

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
import com.volcengine.cloudcore.common.mode.Role;
import com.volcengine.cloudphone.apiservice.IClipBoardListener;
import com.volcengine.cloudphone.apiservice.StreamProfileChangeCallBack;
import com.volcengine.cloudphone.apiservice.outinterface.CameraManagerListener;
import com.volcengine.cloudphone.apiservice.outinterface.RemoteCameraRequestListener;
import com.volcengine.vegameengine.feature.AudioServiceView;
import com.volcengine.vegameengine.feature.CamaraManagerView;
import com.volcengine.vegameengine.feature.ClipBoardServiceManagerView;
import com.volcengine.vegameengine.feature.FileChannelExtView;
import com.volcengine.vegameengine.feature.FileChannelView;
import com.volcengine.vegameengine.feature.LocationServiceView;
import com.volcengine.vegameengine.feature.MultiUserManagerView;
import com.volcengine.vegameengine.feature.PodControlServiceView;
import com.volcengine.vegameengine.feature.ProbeNetworkView;
import com.volcengine.vegameengine.feature.UnclassifiedView;
import com.volcengine.vegameengine.util.AssetsUtil;
import com.volcengine.vegameengine.util.DialogUtils;
import com.volcengine.vegameengine.util.ScreenUtil;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.androidcloud.common.model.StreamStats;
import com.volcengine.cloudcore.common.mode.LocalStreamStats;
import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener;
import com.volcengine.cloudphone.apiservice.outinterface.IStreamListener;

import org.json.JSONException;
import org.json.JSONObject;

import java.util.Map;

public class GameActivity extends AppCompatActivity
        implements IGamePlayerListener, IStreamListener {

    private final String TAG = getClass().getSimpleName();
    private ViewGroup mContainer;
    public static final String KEY_PARAM_GAME_ID = "gameId";
    public static final String KEY_ROUND_ID = "roundId";
    public static final String KEY_ClARITY_ID = "clarityId";
    public static final String KEY_RESERVED_ID = "reservedId";
    public static final String KEY_FEATURE_ID = "featureId";
    private ConstraintLayout mContainers;

    private boolean mIsHideButtons = false;
    public VeGameEngine veGameEngine = VeGameEngine.getInstance();
    DialogUtils.DialogWrapper mDialogWrapper;
    FileChannelView mFileChannelView;
    private GamePlayConfig mGamePlayConfig;

    private Button btnAudio, btnCamera, btnClarity, btnClipBoard, btnFileChannel, btnGround, btnLocation;
    private Button btnMessageChannel, btnPodControl, btnRotation, btnSensor, btnUnclassified;
    private Button btnProbeNetwork, btnLocalInput, btnPadConsole, btnMultiUser, btnFileChannelExt;
    private TextView tvInfo;
    private boolean isLand = false;
    private boolean isShowInfo = false;
    private long lastBackPress;

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

        /**
         * ak/sk/token的值从assets目录下的sts.json文件中读取，该目录及文件需要自行创建。
         * sts.json的格式形如
         * {
         *     "ak": "your_ak",
         *     "sk": "your_sk",
         *     "token": "your_token"
         * }
         */
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


        // ak, sk, token: 请通过火山引擎申请ak获得，详情见https://www.volcengine.com/docs/6512/75577
        builder.userId(userId) // 用户userid
                .ak(ak) // 必填 ACEP ak
                .sk(sk)  // 必填 ACEP sk
                .token(token) // 必填 ACEP token
                .container(mContainer)//必填参数，用来承载画面的 Container, 参数说明: layout 需要是FrameLayout或者FrameLayout的子类
                .roundId(intent.getStringExtra(KEY_ROUND_ID))//必填参数，自定义roundId
                .videoStreamProfileId(intent.getIntExtra(KEY_ClARITY_ID, 1)) // 选填参数，清晰度ID
                .gameId(intent.getStringExtra(KEY_PARAM_GAME_ID)) //必填, gameId
                .enableAcceleratorSensor(true)
                .enableGravitySensor(true)
                .enableGyroscopeSensor(true)
                .enableMagneticSensor(true)
                .enableOrientationSensor(true)
                .enableVibrator(true)
                .enableLocationService(true)
                .enableLocalKeyboard(true)
                .keyBoardEnable(true)
                .enableFileChannel(true)
                .role(Role.PLAYER)
                .roomType(0)
                .reservedId(intent.getStringExtra(KEY_RESERVED_ID))
                .streamListener(GameActivity.this);

        mGamePlayConfig = builder.build();
        // 初始化成功才可以调用
        veGameEngine.start(mGamePlayConfig, GameActivity.this);
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
        if (mDialogWrapper != null) {
            mDialogWrapper.release();
            mDialogWrapper = null;
        }
        if (mFileChannelView != null) {
            mFileChannelView = null;
        }
        super.onDestroy();
    }

    @Override
    public void finish() {
        veGameEngine.stop();
        super.finish();
    }

    @Override
    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        super.onActivityResult(requestCode, resultCode, data);
        if (mFileChannelView != null) {
            mFileChannelView.onActivityResult(requestCode, resultCode, data);
        }
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
        tvInfo = findViewById(R.id.tv_info);
    }

    /**
     * 初始化功能界面
     */
    private void initFeatures() {
        mContainers = findViewById(R.id.cl_container);
        btnAudio = findViewById(R.id.btn_audio);
        btnCamera = findViewById(R.id.btn_camera);
        btnClarity = findViewById(R.id.btn_clarity);
        btnClipBoard = findViewById(R.id.btn_clipboard);
        btnFileChannel = findViewById(R.id.btn_file_channel);
        btnFileChannelExt = findViewById(R.id.btn_file_channel_ext);
        btnGround = findViewById(R.id.btn_ground);
        btnLocation = findViewById(R.id.btn_location);
        btnMessageChannel = findViewById(R.id.btn_message_channel);
        btnPodControl = findViewById(R.id.btn_pod_control);
        btnPadConsole = findViewById(R.id.btn_pad_console);
        btnRotation = findViewById(R.id.btn_orientation);
        btnSensor = findViewById(R.id.btn_sensor);
        btnUnclassified = findViewById(R.id.btn_unclassified);
        btnProbeNetwork = findViewById(R.id.btn_probe_network);
        btnLocalInput = findViewById(R.id.btn_local_input);
        btnMultiUser = findViewById(R.id.btn_multi_user);

        findViewById(R.id.btn_show_info).setOnClickListener(v -> {
            isShowInfo = !isShowInfo;
            tvInfo.setVisibility(isShowInfo ? View.VISIBLE : View.GONE);
        });

        findViewById(R.id.btn_show_or_hide).setOnClickListener(v -> {
            mIsHideButtons = !mIsHideButtons;
            mContainers.setVisibility(mIsHideButtons ? View.GONE : View.VISIBLE);
        });

        btnRotation.setOnClickListener(view -> {
            if (isLand) {
                setRotation(270);
            } else {
                setRotation(0);
            }
            isLand = !isLand;
        });

        switch (getIntent().getIntExtra(KEY_FEATURE_ID, -1)) {
            case FEATURE_AUDIO: // 音频
                btnAudio.setVisibility(View.VISIBLE);
                btnAudio.setOnClickListener(view -> {
                    if (veGameEngine.getAudioService() != null) {
                        mDialogWrapper = DialogUtils.wrapper(
                                new AudioServiceView(this, veGameEngine.getAudioService()));
                        mDialogWrapper.show();
                    } else {
                        AcLog.d(TAG, "AudioService is null!");
                    }
                });
                break;
            case FEATURE_CAMERA: // 相机
                if (veGameEngine.getCameraManager() != null) {
                    new CamaraManagerView(this, veGameEngine.getCameraManager(), btnCamera);
                } else {
                    AcLog.d(TAG, "CameraManager is null!");
                }
                break;
            case FEATURE_CLIPBOARD: // 剪切板
                if (veGameEngine.getClipBoardServiceManager() != null) {
                    new ClipBoardServiceManagerView(this, veGameEngine.getClipBoardServiceManager(), btnClipBoard);
                } else {
                    AcLog.d(TAG, "ClipBoardServiceManager is null!");
                }
                break;
            case FEATURE_FILE_CHANNEL: // 文件通道
                btnFileChannel.setVisibility(View.VISIBLE);
                btnFileChannel.setOnClickListener(view -> {
                    if (veGameEngine.getFileChannel() != null) {
                        mFileChannelView = new FileChannelView(this, veGameEngine.getFileChannel());
                        mDialogWrapper = DialogUtils.wrapper(mFileChannelView);
                        mDialogWrapper.show();
                    } else {
                        AcLog.d(TAG, "FileChannel is null!");
                    }
                });
                break;
            case FEATURE_FILE_CHANNEL_EXT: // 大文件通道
                if (veGameEngine.getFileChannelExt() != null) {
                    new FileChannelExtView(this, veGameEngine.getFileChannelExt(), btnFileChannelExt);
                }
                else {
                    AcLog.d(TAG, "FileChannelExt is null!");
                }
                break;
            case FEATURE_LOCATION: // 定位服务
                if (veGameEngine.getLocationService() != null) {
                    new LocationServiceView(this, veGameEngine.getLocationService(), btnLocation);
                } else {
                    AcLog.d(TAG, "LocationService is null!");
                }
                break;
            case FEATURE_MULTI_USER: // 多用户
                if (veGameEngine.getMultiUserService() != null) {
                    new MultiUserManagerView(this, veGameEngine.getMultiUserService(), btnMultiUser);
                } else {
                    AcLog.d(TAG, "MultiUserService is null!");
                }
                break;
            case FEATURE_POD_CONTROL: // Pod控制
                if (veGameEngine.getPodControlService() != null) {
                    new PodControlServiceView(this, veGameEngine.getPodControlService(), btnPodControl);
                } else {
                    AcLog.d(TAG, "PodControlService is null!");
                }
                break;
            case FEATURE_PROBE_NETWORK: // 网络探测
                btnProbeNetwork.setVisibility(View.VISIBLE);
                btnProbeNetwork.setOnClickListener(view -> {
                    final ProbeNetworkView dialog = new ProbeNetworkView(this, v -> veGameEngine.probeInterrupt());
                    dialog.showProbeNetworkDialogForGame(mGamePlayConfig);
                });
                break;
            case FEATURE_UNCLASSIFIED: // 其他
                new UnclassifiedView(this, btnUnclassified);
                break;
            default:
                break;
        }
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

    /**
     * SDK内部产生的错误回调
     *
     * @param i 错误码
     * @param s 错误详情
     */
    @Override
    public void onError(int i, String s) {
        String msg = "onError:" + i + ", " + s;
        Toast.makeText(this, "code" + i + "msg" + s, Toast.LENGTH_SHORT).show();
        Log.e(TAG, msg);
    }

    /**
     * SDK内部产生的警告回调
     *
     * @param i 警告码
     * @param s 警告详情
     */
    @Override
    public void onWarning(int i, String s) {
        Log.d(TAG, "warn: code " + i + ", msg" + s);
    }

    /**
     * 网络连接类型和状态切换回调
     *
     * @param i 当前的网络类型
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
    public void onNetworkChanged(int i) {
        Log.d(TAG, String.format("%d", i));
    }

    /**
     * 加入房间前回调，用于获取并初始化各个功能服务，例如设置各种事件监听回调。
     */
    @Override
    public void onServiceInit() {
        initFeatures();
    }

    /**
     * 收到音频首帧时的回调
     *
     * @param s 远端实例音频流的ID
     */
    @Override
    public void onFirstAudioFrame(String s) {
        Log.d(TAG, "onFirstAudioFrame " + s);
    }

    /**
     * 收到视频首帧时的回调
     *
     * @param s 远端实例视频流的ID
     */
    @Override
    public void onFirstRemoteVideoFrame(String s) {
        Log.d(TAG, "onFirstRemoteVideoFrame " + s);
    }

    /**
     * 开始播放的回调
     */
    @Override
    public void onStreamStarted() {
        Log.d(TAG, "onStreamStarted ");
    }

    /**
     * 暂停播放后的回调，调用pause()后会触发
     */
    @Override
    public void onStreamPaused() {
        Log.d(TAG, "onStreamPaused ");
    }

    /**
     * 恢复播放后的回调，调用resume()或muteAudio(false)后回触发
     */
    @Override
    public void onStreamResumed() {
        Log.d(TAG, "onStreamResumed ");
    }

    /**
     * 周期为2秒的音视频网络状态的回调，可用于内部数据分析或监控
     *
     * @param streamStats 远端视频流的性能状态
     */
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
        AcLog.d(TAG, "onNetworkQuality() called with: quality = [" + quality + "]");
    }

    /**
     * 周期为2秒的本地推送的音视频流的状态回调
     *
     * @param localStreamStats 本地音视频流的性能状态
     */
    @Override
    public void onLocalStreamStats(LocalStreamStats localStreamStats) {
        AcLog.d(TAG, "LocalStreamStats" + localStreamStats);
    }

    /**
     * 视频流连接状态改变回调
     *
     * @param i 连接状态
     *          1 -- 连接断开
     *          2 -- 首次连接
     *          3 -- 首次连接成功
     *          4 -- 连接断开后重新连接中
     *          5 -- 连接断开后重新连接成功
     *          6 -- 连接断开超过10秒，仍然会继续重连
     */
    @Override
    public void onStreamConnectionStateChanged(int i) {
        Log.d(TAG, "onStreamConnectionStateChanged " + i);
    }

    @Override
    public void onConfigurationChanged(@NonNull Configuration newConfig) {
        super.onConfigurationChanged(newConfig);
        AcLog.d(TAG, "onConfigurationChanged newConfig " + newConfig.orientation);
        VeGameEngine.getInstance().rotate(newConfig.orientation);
    }

    /**
     * 操作延迟回调
     *
     * @param l 操作延迟的具体值，单位:毫秒
     */
    @Override
    public void onDetectDelay(long l) {
        Log.d(TAG, "delay " + l);
    }


    /**
     * 客户端旋转回调
     *
     * @param i 旋转方向
     *          0, 180 -- 竖屏
     *         90, 270 -- 横屏
     */
    @Override
    public void onRotation(int i) {
        Log.d(TAG, "rotation" + i);
        setRotation(i);
    }

    /**
     * 远端实例退出回调
     *
     * @param i 退出的原因码
     * @param s 退出的原因详情
     */
    @Override
    public void onPodExit(int i, String s) {
        Log.d(TAG, "onPodExit" + i + " ,msg:" + s);
    }

    @Override
    public void onBackPressed() {
        long current = System.currentTimeMillis();
        if (current - lastBackPress < 1000L) {
            super.onBackPressed();
        } else {
            Toast.makeText(this, getString(R.string.back_again_to_exit), Toast.LENGTH_SHORT).show();
            lastBackPress = current;
        }
    }

    public static void startGame(
            String gameId,
            String roundId,
            int clarityId,
            String reservedId,
            Activity activity,
            int featureId) {
        Intent intent = new Intent(activity, GameActivity.class);
        intent.putExtra(GameActivity.KEY_PARAM_GAME_ID, gameId);
        if (roundId.isEmpty() || roundId.equals("")) roundId = "123";
        intent.putExtra(GameActivity.KEY_ROUND_ID, roundId);
        intent.putExtra(GameActivity.KEY_ClARITY_ID, clarityId);
        intent.putExtra(GameActivity.KEY_FEATURE_ID, featureId);
        intent.putExtra(GameActivity.KEY_RESERVED_ID, reservedId);
        activity.startActivity(intent);
    }
}
