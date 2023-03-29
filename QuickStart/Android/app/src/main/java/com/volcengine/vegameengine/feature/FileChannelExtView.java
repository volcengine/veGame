package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
import android.text.TextUtils;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.Toast;

import com.volcengine.cloudphone.apiservice.IFileChannelExt;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

import java.io.File;
import java.util.HashMap;
import java.util.Map;

public class FileChannelExtView {

    private static final String TAG = "FileChannelExt";
    private IFileChannelExt mFileChannelExt;
    private Context mContext;
    private DialogUtils.DialogWrapper mDialogWrapper;
    private File mReceiveFile = null;
    private TestView mTestView;

    public FileChannelExtView(Context context, IFileChannelExt iFileChannelExt, Button button) {
        this.mContext = context;
        this.mFileChannelExt = iFileChannelExt;
        mTestView = new TestView(mContext);
        mDialogWrapper = DialogUtils.wrapper(mTestView);
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());

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
                    Log.d(TAG, "IReceiveFileListener.onStart() - " + file.getAbsolutePath());
                    mReceiveFile = file;
                    mTestView.printLog("IReceiveFileListener.onStart() - " + file.getAbsolutePath());
                    mTestView.printLog(options);
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
                    Log.d(TAG, "IReceiveFileListener.onProgress() - " + progress);
                    mReceiveFile = file;
                    mTestView.printLog("IReceiveFileListener.onProgress() - " + progress);
                    mTestView.printLog(options);
                }

                /**
                 * 接收文件完成回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 */
                @Override
                public void onComplete(File file, Map<String, String> options) {
                    Log.d(TAG, "IReceiveFileListener.onComplete() - " + file.getAbsolutePath());
                    mReceiveFile = file;
                    mTestView.printLog("IReceiveFileListener.onComplete() - " + file.getAbsolutePath());
                    mTestView.printLog(options);
                }

                /**
                 * 接收文件取消回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 */
                @Override
                public void onCancel(File file, Map<String, String> options) {
                    Log.d(TAG, "IReceiveFileListener.onCancel() - " + file.getAbsolutePath());
                    mTestView.printLog("IReceiveFileListener.onCancel() - " + file.getAbsolutePath());
                    mTestView.printLog(options);
                }

                /**
                 * 接收文件失败回调
                 *
                 * @param file 云端实例发送的文件
                 * @param options 附带的options参数
                 * @param err  接收文件失败的错误码
                 */
                @Override
                public void onError(File file, Map<String, String> options, int err) {
                    Log.d(TAG, "IReceiveFileListener.onError() - " + err);
                    mTestView.printLog("IReceiveFileListener.onError() - " + err);
                    mTestView.printLog(options);
                }
            });
        }
    }

    private String getErrorMessage(int err) {
        switch (err) {
            case 71001: return "ERROR_FILE_CHANNEL_FILE_NOT_FOUND";
            case 71002: return "ERROR_FILE_CHANNEL_FILE_NO_ACCESS";
            case 71003: return "ERROR_FILE_CHANNEL_NETWORK_ABORT";
            case 71004: return "ERROR_FILE_CHANNEL_TIMEOUT";
            case 71005: return "ERROR_FILE_CHANNEL_STORAGE_NOT_ENOUGH";
            case 71006: return "ERROR_FILE_CHANNEL_INTERNAL_ERROR";
            default: return "UNKNOWN_ERROR";
        }
    }

    public class TestView extends ScrollView implements View.OnClickListener, IFileChannelExt.ISendFileListener {

        private Button mStartSendFileButton;
        private Button mStopSendFileButton;
        private Button mStopReceiveFileButton;
        private Button mAddOptionButton;
        private Button mCheckOptionButton;
        private Button mClearOptionsButton;

        private EditText mFilePathEditText;
        private EditText mLogConsoleEditText;
        private EditText mKeyOptionEditText;
        private EditText mValueOptionEditText;

        private Map<String, String> mOptions = new HashMap<>();

        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_file_channel_ext, this);
            setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            initView();
        }

        private void initView() {
            mStartSendFileButton = findViewById(R.id.btn_start_send_file);
            mStopSendFileButton = findViewById(R.id.btn_stop_send_file);
            mStopReceiveFileButton = findViewById(R.id.btn_stop_receive_file);
            mAddOptionButton = findViewById(R.id.btn_add_option);
            mCheckOptionButton = findViewById(R.id.btn_check_options);
            mClearOptionsButton = findViewById(R.id.btn_clear_options);

            mStartSendFileButton.setOnClickListener(this);
            mStopSendFileButton.setOnClickListener(this);
            mStopReceiveFileButton.setOnClickListener(this);
            mAddOptionButton.setOnClickListener(this);
            mCheckOptionButton.setOnClickListener(this);
            mClearOptionsButton.setOnClickListener(this);

            mFilePathEditText = findViewById(R.id.et_file_path);
            File file = new File(getContext().getExternalCacheDir(), "1.mp4");
//            File file = new File(Environment.getExternalStorageDirectory(), "/tencent/ams/cache");
//            file = new File(file, "meta.dat");
            mFilePathEditText.setText(file.getAbsolutePath());

            mKeyOptionEditText = findViewById(R.id.et_option_key);
            mValueOptionEditText = findViewById(R.id.et_option_value);

            mLogConsoleEditText = findViewById(R.id.et_log_console);
            mLogConsoleEditText.setKeyListener(null);
            mLogConsoleEditText.setCursorVisible(false);
            mLogConsoleEditText.setFocusable(false);
            mLogConsoleEditText.setFocusableInTouchMode(false);
        }

        public void printLog(String log) {
            ((Activity) mContext).runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                sb.append(mLogConsoleEditText.getText().toString());
                sb.append("\r\n");
                sb.append(log);
                setText(sb.toString());
            });
        }

        public void printLog(Map<String, String> options) {
            StringBuilder sb = new StringBuilder();
            for (String key : options.keySet()) {
                sb.append(key + " - " + options.get(key));
                sb.append("\r\n");
            }
            printLog(sb.toString());
        }

        @Override
        public void onClick(View view) {
            /**
             * startSendFile(File file, ISendFileListener listener) -- 开始发送本地文件到远端实例
             *
             * @param file 发送的本地文件
             * @param listener 发送文件的进度及结果监听器
             *
             * stopSendFile(File file) -- 停止发送本地文件到远端实例
             *
             * @param file 发送的本地文件
             *
             * stopReceiveFile(File file) -- 停止接收远端实例发送到本地的文件
             *
             * @param file 远端实例发送的文件
             */
            if (view.getId() == R.id.btn_start_send_file) {
                if (mFileChannelExt != null) {
                    String filePathString = mFilePathEditText.getText().toString();
                    File file = new File(filePathString);
                    if (file.exists()) {
                        mFileChannelExt.startSendFile(file, mOptions, this);
                    }
                    else {
                        Toast.makeText(mContext, "文件不存在", Toast.LENGTH_SHORT).show();
                    }
                }
            } else if (view.getId() == R.id.btn_stop_send_file) {
                if (mFileChannelExt != null) {
                    String filePathString = mFilePathEditText.getText().toString();
                    File file = new File(filePathString);
                    if (file.exists()) {
                        mFileChannelExt.stopSendFile(file);
                    }
                    else {
                        Toast.makeText(mContext, "文件不存在", Toast.LENGTH_SHORT).show();
                    }
                }
            } else if (view.getId() == R.id.btn_stop_receive_file) {
                if (mFileChannelExt != null && mReceiveFile != null) {
                    mFileChannelExt.stopReceiveFile(mReceiveFile);
                }
            } else if (view.getId() == R.id.btn_add_option) {
                String key = mKeyOptionEditText.getText().toString();
                String value = mValueOptionEditText.getText().toString();
                if (!TextUtils.isEmpty(key) && !TextUtils.isEmpty(value)) {
                    mOptions.put(key, value);
                    mKeyOptionEditText.setText("");
                    mValueOptionEditText.setText("");
                    mCheckOptionButton.setText("Check all options(" + mOptions.size() + ")");
                }
            } else if (view.getId() == R.id.btn_check_options) {
                StringBuilder sb = new StringBuilder();
                if (mOptions.size() > 0) {
                    for (String key : mOptions.keySet()) {
                        sb.append(key + " - " + mOptions.get(key));
                        sb.append("\r\n");
                    }
                } else {
                    sb.append("no any key-value in options");
                }
                printLog(sb.toString());
            } else if (view.getId() == R.id.btn_clear_options) {
                mOptions.clear();
                mCheckOptionButton.setText("Check all options(" + mOptions.size() + ")");
            }
        }

        /**
         * 发送文件开始回调
         *
         * @param file 发送的本地文件
         * @param options 附带的options参数
         */
        @Override
        public void onStart(File file, Map<String, String> options) {
            Log.d(TAG, "ISendFileListener.onStart() - " + file.getAbsolutePath());
            printLog("ISendFileListener.onStart() - " + file.getAbsolutePath());
            printLog(options);
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
            Log.d(TAG, "ISendFileListener.onProgress() - " + progress);
            printLog("ISendFileListener.onProgress() - " + progress);
            printLog(options);
        }

        /**
         * 发送文件完成回调
         *
         * @param file 发送的本地文件
         * @param options 附带的options参数
         */
        @Override
        public void onComplete(File file, Map<String, String> options) {
            Log.d(TAG, "ISendFileListener.onComplete() - " + file.getAbsolutePath());
            printLog("ISendFileListener.onComplete() - " + file.getAbsolutePath());
            printLog(options);
        }

        /**
         * 发送文件取消回调
         *
         * @param file 发送的本地文件
         * @param options 附带的options参数
         */
        @Override
        public void onCancel(File file, Map<String, String> options) {
            Log.d(TAG, "ISendFileListener.onCancel() - " + file.getAbsolutePath());
            printLog("ISendFileListener.onCancel() - " + file.getAbsolutePath());
            printLog(options);
        }

        /**
         * 发送文件失败回调
         *
         * @param file 发送的本地文件
         * @param options 附带的options参数
         * @param err  发送文件失败的错误码
         */
        @Override
        public void onError(File file, Map<String, String> options, int err) {
            Log.d(TAG, "ISendFileListener.onError() - " + err);
            printLog("ISendFileListener.onError() - " + err + " " + getErrorMessage(err));
            printLog(options);
        }

        private void setText(String textString) {
            mLogConsoleEditText.setText(textString);
            mLogConsoleEditText.setMovementMethod(ScrollingMovementMethod.getInstance());
            mLogConsoleEditText.setSelection(mLogConsoleEditText.getText().length(), mLogConsoleEditText.getText().length());
        }
    }

}
