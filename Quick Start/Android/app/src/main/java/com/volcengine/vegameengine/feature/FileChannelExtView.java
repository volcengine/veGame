package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
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
                 */
                @Override
                public void onStart(File file) {
                    Log.d(TAG, "IReceiveFileListener.onStart() - " + file.getAbsolutePath());
                    mReceiveFile = file;
                    mTestView.printLog("IReceiveFileListener.onStart() - " + file.getAbsolutePath());
                }

                /**
                 * 接收文件进度更新回调
                 *
                 * @param file 云端实例发送的文件
                 */
                @Override
                public void onProgress(File file, int progress) {
                    Log.d(TAG, "IReceiveFileListener.onProgress() - " + progress);
                    mReceiveFile = file;
                    mTestView.printLog("IReceiveFileListener.onProgress() - " + progress);
                }

                /**
                 * 接收文件完成回调
                 *
                 * @param file 云端实例发送的文件
                 */
                @Override
                public void onComplete(File file) {
                    Log.d(TAG, "IReceiveFileListener.onComplete() - " + file.getAbsolutePath());
                    mReceiveFile = file;
                    mTestView.printLog("IReceiveFileListener.onComplete() - " + file.getAbsolutePath());
                }

                /**
                 * 接收文件取消回调
                 *
                 * @param file 云端实例发送的文件
                 */
                @Override
                public void onCancel(File file) {
                    Log.d(TAG, "IReceiveFileListener.onCancel() - " + file.getAbsolutePath());
                    mTestView.printLog("IReceiveFileListener.onCancel() - " + file.getAbsolutePath());
                }

                /**
                 * 接收文件失败回调
                 *
                 * @param file 云端实例发送的文件
                 * @param err  接收文件失败的错误码
                 */
                @Override
                public void onError(File file, int err) {
                    Log.d(TAG, "IReceiveFileListener.onError() - " + err);
                    mTestView.printLog("IReceiveFileListener.onError() - " + err);
                }
            });
        }
    }

    public class TestView extends ScrollView implements View.OnClickListener, IFileChannelExt.ISendFileListener {

        private Button mStartSendFileButton;
        private Button mStopSendFileButton;
        private Button mStopReceiveFileButton;

        private EditText mFilePathEditText;
        private EditText mLogConsoleEditText;

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

            mStartSendFileButton.setOnClickListener(this);
            mStopSendFileButton.setOnClickListener(this);
            mStopReceiveFileButton.setOnClickListener(this);

            mFilePathEditText = findViewById(R.id.et_file_path);
            File file = new File(getContext().getExternalCacheDir(), "1.mp4");
//            File file = new File(Environment.getExternalStorageDirectory(), "/tencent/ams/cache");
//            file = new File(file, "meta.dat");
            mFilePathEditText.setText(file.getAbsolutePath());

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
                        mFileChannelExt.startSendFile(file, this);
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
            }
        }

        /**
         * 发送文件开始回调
         *
         * @param file 发送的本地文件
         */
        @Override
        public void onStart(File file) {
            Log.d(TAG, "ISendFileListener.onStart() - " + file.getAbsolutePath());
            ((Activity) mContext).runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                sb.append(mLogConsoleEditText.getText().toString());
                sb.append("\r\n");
                sb.append("ISendFileListener.onStart() - " + file.getAbsolutePath());
                setText(sb.toString());
            });
        }

        /**
         * 发送文件进度更新回调
         *
         * @param file 发送的本地文件
         */
        @Override
        public void onProgress(File file, int progress) {
            Log.d(TAG, "ISendFileListener.onProgress() - " + progress);
            ((Activity) mContext).runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                sb.append(mLogConsoleEditText.getText().toString());
                sb.append("\r\n");
                sb.append("ISendFileListener.onProgress() - " + progress);
                setText(sb.toString());
            });
        }

        /**
         * 发送文件完成回调
         *
         * @param file 发送的本地文件
         */
        @Override
        public void onComplete(File file) {
            Log.d(TAG, "ISendFileListener.onComplete() - " + file.getAbsolutePath());
            ((Activity) mContext).runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                sb.append(mLogConsoleEditText.getText().toString());
                sb.append("\r\n");
                sb.append("ISendFileListener.onComplete() - " + file.getAbsolutePath());
                setText(sb.toString());
            });
        }

        /**
         * 发送文件取消回调
         *
         * @param file 发送的本地文件
         */
        @Override
        public void onCancel(File file) {
            Log.d(TAG, "ISendFileListener.onCancel() - " + file.getAbsolutePath());
            ((Activity) mContext).runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                sb.append(mLogConsoleEditText.getText().toString());
                sb.append("\r\n");
                sb.append("ISendFileListener.onCancel() - " + file.getAbsolutePath());
                setText(sb.toString());
            });
        }

        /**
         * 发送文件失败回调
         *
         * @param file 发送的本地文件
         * @param err  发送文件失败的错误码
         */
        @Override
        public void onError(File file, int err) {
            Log.d(TAG, "ISendFileListener.onError() - " + err);
            ((Activity) mContext).runOnUiThread(() -> {
                StringBuilder sb = new StringBuilder();
                sb.append(mLogConsoleEditText.getText().toString());
                sb.append("\r\n");
                sb.append("ISendFileListener.onError() - " + err);
                setText(sb.toString());
            });
        }

        private void setText(String textString) {
            mLogConsoleEditText.setText(textString);
            mLogConsoleEditText.setMovementMethod(ScrollingMovementMethod.getInstance());
            mLogConsoleEditText.setSelection(mLogConsoleEditText.getText().length(), mLogConsoleEditText.getText().length());
        }
    }

}
