package com.bytedance.vemessagechannelprj;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;

import android.Manifest;
import android.content.Context;
import android.content.pm.PackageManager;
import android.os.Bundle;
import android.os.Handler;
import android.os.IBinder;
import android.os.Looper;
import android.os.Message;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.TextView;
import android.widget.Toast;

import com.bytedance.vemsgchannel.api.Constant;
import com.bytedance.vemsgchannel.api.IFileTransportResultListener;
import com.bytedance.vemsgchannel.api.IGeneralDataHandler;
import com.bytedance.vemsgchannel.api.IMessageHandler;
import com.bytedance.vemsgchannel.api.VeMessage;
import com.bytedance.vemsgchannel.api.VeMessageChannelClient;
import com.bytedance.vemsgchannel.utils.DigestUtils;


public class MainActivity extends AppCompatActivity {
    private static final String TAG = "VeMessageChannel";
    private Button mBtnInit;
    private Button mBtnDestroy;
    private Button mBtnGetExtra;
    private TextView mTvInitResult;
    private TextView mTvSendResult;
    private TextView mTvOnReceive;
    private TextView mTvRecvMessage;
    private TextView mTvOnStatusChanged;
    private TextView mTxGetExtra;
    private EditText mEtMessage;

    private EditText mEtMessageNoNeedAck;
    private EditText mEtStreamAddr;
    private Button mBtnSendMessage;

    private Button mBtnSendMessageNoNeedAck;
    private Button mBtnSendStream;
    private Button mBtnCancelSendStream;
    private EditText mEtTimeout;
    private Button mBtnCloudRuntime;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        initView();
        initConnectClickEvent();
        initSendMessageClickEvent();
        initSendStreamClickEvent();
        checkPermission();

        VeMessageChannelClient.getInstance().setDebug(true);
        //点击空白处隐藏键盘
        ((ViewGroup) findViewById(android.R.id.content)).getChildAt(0).setOnClickListener(new View.OnClickListener() {
            @Override
            public void onClick(View view) {
                hideSoftInput(view.getWindowToken());
            }
        });
    }

    private void initView() {
        mBtnInit = findViewById(R.id.btnInit);
        mBtnDestroy = findViewById(R.id.btnDestroy);
        mBtnGetExtra = findViewById(R.id.btnGetExtra);
        mTxGetExtra = findViewById(R.id.tvGetExtra);
        mTvInitResult = findViewById(R.id.tvInitResult);
        mTvSendResult = findViewById(R.id.tvSendResult);
        mTvOnReceive = findViewById(R.id.tvOnReceive);
        mTvOnStatusChanged = findViewById(R.id.tvOnStateChanged);
        mEtMessage = findViewById(R.id.etMessage);
        mEtStreamAddr = findViewById(R.id.etStreamAddr);
        mEtMessageNoNeedAck = findViewById(R.id.etMessageNoNeedAck);
        mBtnSendStream = findViewById(R.id.btnSendStream);
        mBtnCancelSendStream = findViewById(R.id.btnCancelSendStream);
        mBtnSendMessage = findViewById(R.id.btnSendMessage);
        mBtnSendMessageNoNeedAck = findViewById(R.id.btnSendMessageNoNeedAck);
        mEtTimeout = findViewById(R.id.etTimeout);
        mTvRecvMessage = findViewById(R.id.tvRecvMessage);
        mBtnCloudRuntime = findViewById(R.id.btnCloudRuntime);
        mBtnCloudRuntime.setOnClickListener(v -> {
            showToast(VeMessageChannelClient.getInstance().isCloudRuntime() + "");
        });
    }


    @Override
    protected void onResume() {
        super.onResume();
    }

    /**
     * 隐藏软键盘
     */
    private void hideSoftInput(IBinder token) {
        if (token != null) {
            InputMethodManager manager = (InputMethodManager) getSystemService(Context.INPUT_METHOD_SERVICE);
            manager.hideSoftInputFromWindow(token, InputMethodManager.HIDE_NOT_ALWAYS);
        }
    }


    private void initSendStreamClickEvent() {
        // 发送文件
        mBtnSendStream.setOnClickListener(v ->  {
            String filePath = mEtStreamAddr.getText().toString();
            if (TextUtils.isEmpty(filePath)) {
                showToast("文件路径为空");
                return;
            }

            /**
             * 发送大文件
             *
             * @param filePath 本地文件绝对路径.
             * @return 0-成功，1-失败，2-未连接
             * @apiNote VeGameSDK/VePhoneSDK version >= 1.19.0
             *
             */
            int res = VeMessageChannelClient.getInstance().sendFile(filePath);
            showToast("文件发送" + (res == 0 ? "成功" : "失败 " + res));
        });

        // 取消发送文件
        mBtnCancelSendStream.setOnClickListener(v -> {
            String filePath = mEtStreamAddr.getText().toString();
            if (TextUtils.isEmpty(filePath)) {
                showToast("文件路径为空");
                return;
            }

            /**
             * 取消发送中文件
             *
             * @param filePath 本地文件绝对路径.
             * @return 0-成功，1-失败，2-未连接
             * @apiNote VeGameSDK/VePhoneSDK version >= 1.19.0
             *
             */
            int res = VeMessageChannelClient.getInstance().cancelSendFile(filePath);
            showToast("取消" + (res == 0 ? "成功" : "失败 " + res));
        });
    }


    private void initSendMessageClickEvent() {
        // 发送回执消息
        mBtnSendMessage.setOnClickListener(v ->  {
            long timeout = Integer.parseInt(mEtTimeout.getText().toString());
            /**
             * 发送消息至客户端
             * @param message 消息本体
             * @param needSendResult 该条消息是否需要发送结果回调
             * @param timeout 必须大于0，通过{@link IMessageHandler#onSendResult(int, String)}回调成功或失败
             */
            VeMessageChannelClient.getInstance().sendMessageToClient(mEtMessage.getText().toString(), true, timeout);
        });

        // 发送无回执消息
        mBtnSendMessageNoNeedAck.setOnClickListener(v -> {
            /**
             * 发送消息至客户端
             * @param message 消息本体
             * @param needSendResult 该条消息是否需要发送结果回调，默认30000ms超时
             */
            VeMessageChannelClient.getInstance().sendMessageToClient(mEtMessageNoNeedAck.getText().toString(), false);
        });
    }

    private void initConnectClickEvent() {
        // 初始化消息通道
        mBtnInit.setOnClickListener(v -> initMessageChannel());

        // 销毁消息通道
        mBtnDestroy.setOnClickListener(v -> VeMessageChannelClient.getInstance().destroy());

        // 获取客户端在启动游戏时发送到云端的通用信息数据(即通过extra字段发送的数据)
        mBtnGetExtra.setOnClickListener(v -> {
            VeMessageChannelClient.getInstance().requestGeneralData(Constant.GENERAL_CMD_GET_EXTRA,
                    new IGeneralDataHandler() {
                @Override
                public void onGeneralDataBack(int result, String data) {
                    mTxGetExtra.setText("result: " + result + ", extra_data : " + data);
                }
            });
        });
    }


    static final int M_INIT_SUCCESS = 1;
    static final int M_ON_FAILED = 2;
    static final int M_ON_REC_ACK = 3;
    static final int M_ON_REC_MSG = 4;
    static final int M_ON_STATUS_CHANGE = 5;

    private final Handler mainUiHandler = new Handler(Looper.getMainLooper()) {
        @Override
        public void handleMessage(@NonNull Message msg) {
            super.handleMessage(msg);
            switch (msg.what) {
                case M_INIT_SUCCESS:
                    mTvInitResult.setText(msg.obj.toString());
                    showToast("MCC初始化成功");
                    break;
                case M_ON_FAILED:
                    mTvInitResult.setText(msg.obj.toString());
                    showToast(msg.obj.toString());
                    break;
                case M_ON_REC_ACK:
                    mTvSendResult.setText("messageId: " + msg.obj);
                    mTvRecvMessage.append(" onSendResult messageId: " + msg.obj + "\n\n");
                    showToast(msg.obj.toString());
                    break;
                case M_ON_REC_MSG:
                    mTvOnReceive.setText((String) msg.obj);
                    mTvRecvMessage.append(" onReceivedMessage " + (String) msg.obj + "\n\n");
                    showToast(msg.obj.toString());
                    break;
                case M_ON_STATUS_CHANGE:
                    mTvOnStatusChanged.setText((String) msg.obj);
                    showToast(msg.obj.toString());
                    break;
                default:
                    break;
            }
        }
    };

    private void initMessageChannel() {
        // accessKey用于建立连接时鉴权，可以使用MCC提供的工具生成包名对应的MD5信息
        String accessKey = DigestUtils.md5Hex(getPackageName());
        Log.d(TAG, "accessKey: " + accessKey);

        /**
         * 初始化消息通道
         *
         * @param context 上下文信息，建议填写ApplicationContext
         * @param channelUid 消息通道对应的用户ID，建立连接时的唯一标识，当前选用应用包名
         * @param accessKey 鉴权信息
         * @param handler 初始化结果回调
         */
        VeMessageChannelClient.getInstance().init(getApplicationContext(), getPackageName(), accessKey, new IMessageHandler() {
            /**
             * 初始化成功回调
             */
            @Override
            public void onInitSuccess() {
                Log.d(TAG, "onInitSuccess");
                Message m = Message.obtain();
                m.what = M_INIT_SUCCESS;
                m.obj = "Success";
                mainUiHandler.sendMessage(m);
            }

            /**
             * 运行过程中发生的失败回调
             *
             * @param errorCode 失败对应的错误码
             *                  1 -- 与服务连接失败
             *                  2 -- 与服务断连
             *                  3 -- 消息发送失败
             *                  4 -- SDK 未在云端环境运行
             *                  5 -- 鉴权失败
             *                  6 -- 消息格式错误
             * @param message 失败对应的错误描述
             */
            @Override
            public void onFailed(int errorCode, String message) {
                Log.d(TAG, "onFailed errorCode: " + errorCode + " message: " + message);
                Message m = Message.obtain();
                m.what = M_ON_FAILED;
                m.obj = "Failed: code: " + errorCode + " message: " + message;
                mainUiHandler.sendMessage(m);
            }

            /**
             * 消息发送结果回调
             *
             * @param resultCode 消息发送的结果
             * @param messageId 消息ID
             */
            @Override
            public void onSendResult(int resultCode, String messageId) {
                Message m = Message.obtain();
                m.what = M_ON_REC_ACK;
                m.obj = messageId + ", result : " + resultCode;
                mainUiHandler.sendMessage(m);
                Log.d(TAG, "onSendResult messageId: " + messageId + ", result : " + resultCode);
            }

            /**
             * 收到对端发送过来的消息
             *
             * @param userId 消息发送端的用户ID
             * @param msg 消息本体
             */
            @Override
            public void onReceivedMessage(String userId, VeMessage msg) {
                Message m = Message.obtain();
                m.what = M_ON_REC_MSG;
                m.obj = "UserId: " + userId + ", msg: " + msg.toString();
                mainUiHandler.sendMessage(m);
                Log.d(TAG, "onReceive userId: " + userId + " msg.text: "
                        + msg.text + " msg.mid: " + msg.mid + " msg.ts: " + msg.timeMillis);
            }

            /**
             * 通道连接状态变化回调
             *
             * @param state 通道连接状态码
             *              1 -- 已连接，初始化完成、与客户端之间通道连接成功时触发
             *              2 -- 已断连，与客户端之间通道连接断开时触发
             *              3 -- 通道停止工作
             */
            @Override
            public void onConnectionStateChanged(int state) {
                Message m = Message.obtain();
                m.what = M_ON_STATUS_CHANGE;
                m.obj = "ConnectionState: " + state;
                mainUiHandler.sendMessage(m);
                Log.d(TAG, "ConnectionState: " + state);
            }
        });

        /**
         * 添加文件发送/接收结果监听
         *
         * @param listener 接收大文件传输结果回调
         *
         * 回调结果
         *    0 -- 发送/接收文件 成功
         *    1 -- 发送/接收文件 失败
         *    2 -- 发送/接收文件 取消
         *    5 -- 表示本次文件发送由云端游戏发起
         *    6 -- 表示本次文件发送由客户端发起
         */
        VeMessageChannelClient.getInstance().setFileTransportResultListener(new IFileTransportResultListener() {
            /**
             * 云端游戏发起，调用大文件发送接口{@link VeMessageChannelClient#sendFile(String)}结果回调
             *
             * @param result 见回调结果
             * @param file 文件绝对路径，客户端保存文件的绝对路径
             * @param md5 文件md5校验
             *
             */
            @Override
            public void onSendFileResult(int result, String file, String md5) {
                Message m = Message.obtain();
                m.what = M_ON_REC_ACK;
                m.obj = "onSendFileResult() result: " + result + ", file: " + file + ", md5:" + md5;
                mainUiHandler.sendMessage(m);
                Toast.makeText(MainActivity.this, "onSendFileResult() result: " + result + ", file: " + file + ", md5:" + md5, Toast.LENGTH_LONG).show();
            }


            /**
             * 客户端发起，客户端发送大文件至云端游戏结果回调
             *
             * @param result 见回调结果
             * @param file 文件绝对路径，云端游戏保存文件的绝对路径
             * @param md5 文件md5校验
             */
            @Override
            public void onReceiveFileResult(int result, String file, String md5) {
                Message m = Message.obtain();
                m.what = M_ON_REC_MSG;
                m.obj = "onReceiveFileResult() result: " + result + ", file: " + file + ", md5:" + md5;
                mainUiHandler.sendMessage(m);
                Toast.makeText(MainActivity.this, "onReceiveFileResult() result: " + result + ", file: " + file + ", md5:" + md5, Toast.LENGTH_LONG).show();
            }
        });
    }

    private static final int REQUEST_EXTERNAL_STORAGE = 1;
    private static final String[] PERMISSIONS_STORAGE = {
            "android.permission.READ_EXTERNAL_STORAGE",
            "android.permission.WRITE_EXTERNAL_STORAGE"
    };

    private void checkPermission() {
        //检查权限（NEED_PERMISSION）是否被授权 PackageManager.PERMISSION_GRANTED表示同意授权
        if (ActivityCompat.checkSelfPermission(this, Manifest.permission.WRITE_EXTERNAL_STORAGE)
                != PackageManager.PERMISSION_GRANTED) {
            if (ActivityCompat.shouldShowRequestPermissionRationale(this, Manifest.permission
                    .WRITE_EXTERNAL_STORAGE)) {
                showToast("请给予相关权限，否则无法正常使用本应用！");
            }
            //申请权限
            ActivityCompat.requestPermissions(this, PERMISSIONS_STORAGE, REQUEST_EXTERNAL_STORAGE);
        } else {
            showToast("已授权成功！");
        }
    }

    @Override
    protected void onDestroy() {
        super.onDestroy();
        VeMessageChannelClient.getInstance().destroy();
    }

    private void showToast(String msg) {
        Toast.makeText(this, msg, Toast.LENGTH_SHORT).show();
    }
}