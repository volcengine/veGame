package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.ClipData;
import android.content.ClipboardManager;
import android.content.Context;
import android.content.Intent;
import android.net.Uri;
import android.os.SystemClock;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.TextView;
import android.widget.Toast;

import androidx.annotation.NonNull;

import com.volcengine.cloudphone.apiservice.IFileChannel;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.filechannel.FileUtil;

import java.io.BufferedReader;
import java.io.File;
import java.io.FileInputStream;
import java.io.FileOutputStream;
import java.io.IOException;
import java.io.InputStream;
import java.io.InputStreamReader;
import java.text.SimpleDateFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;
import java.util.concurrent.CountDownLatch;

public class FileChannelView extends ScrollView implements View.OnClickListener, Runnable, IFileChannel.IChannelStatusListener, IFileChannel.IReceiveDataListener {

    private EditText mFilePathEditText, mLogcatEditText;
    private IFileChannel mFileChannelImpl;
    private File mFile;
    private Context mContext;
    private SeekBar mSeekBar;
    private boolean isRunning = false;
    private final List<FileChannelView.LogcatInfo> mLogcatList = new ArrayList();
    private final CountDownLatch mCountDown = new CountDownLatch(1);
    private static final String TAG = "FileChannelView";

    public FileChannelView(Context context, IFileChannel iFileChannel) {
        super(context);
        this.mContext = context;
        this.mFileChannelImpl = iFileChannel;
        inflate(context, R.layout.dialog_file_channel, this);
        findViewById(R.id.btn_send_data).setOnClickListener(this);
        findViewById(R.id.btn_send_ack_data).setOnClickListener(this);
        findViewById(R.id.btn_get_status).setOnClickListener(this);
        findViewById(R.id.btn_choose_file).setOnClickListener(this);
        mSeekBar = findViewById(R.id.seekbar_timeout);
        mFilePathEditText = findViewById(R.id.et_file_path);
        mFilePathEditText.setText(mFile != null ? mFile.getAbsolutePath() : "");
        mFilePathEditText.setEnabled(false);
        mLogcatEditText = findViewById(R.id.et_logcat);
        mLogcatEditText.setCursorVisible(false);
        mLogcatEditText.setFocusable(false);
        mLogcatEditText.setFocusableInTouchMode(false);
        mLogcatEditText.setLongClickable(true);
        mLogcatEditText.setOnLongClickListener(new OnLongClickListener() {
            public boolean onLongClick(View view) {
                ClipboardManager clipboard = (ClipboardManager)context.getSystemService(Context.CLIPBOARD_SERVICE);
                ClipData clipData = ClipData.newPlainText((CharSequence)null, mLogcatEditText.getText());
                clipboard.setPrimaryClip(clipData);
                Toast.makeText(context, "Copy to Clipboard!", Toast.LENGTH_SHORT).show();
                return false;
            }
        });
        startLogcat();
        updateLogcat();
    }



    public void onDataReceived(@NonNull byte[] payload) {
//        Log.d(TAG, "onDataReceived: " + "#Receive payload " + payload.length + " bytes size of data.");
        addLog("duplex", "#Receive payload " + payload.length + " bytes size of data.");
    }

    public void onStateUpdated(int status) {
        addLog("duplex", "#Status change to " + getStatusString(status) + ".");
    }

    public void run() {
        Object os = null;

        try {
            Process exec = Runtime.getRuntime().exec("logcat -s duplex:D");
            InputStream is = exec.getInputStream();
            BufferedReader bufferedReader = new BufferedReader(new InputStreamReader(is));
            mCountDown.countDown();

            while(isRunning) {
                String line;
                while((line = bufferedReader.readLine()) != null) {
                    int index = line.indexOf("duplex  : ");
                    if (index >= 0) {
                        addLog("duplex", line.substring(index + "duplex  : ".length()));
                    }
                }

                SystemClock.sleep(333L);
            }
        } catch (Exception var15) {
            var15.printStackTrace();
        } finally {
            if (null != os) {
                try {
                    ((FileOutputStream)os).close();
                } catch (IOException var14) {
                }
            }

        }

    }

    public void startLogcat() {
        try {
            if (!isRunning) {
                isRunning = true;
                Runtime.getRuntime().exec("logcat -c");
                new Thread(this).start();
                waitForLogcatReady();
            }
        } catch (Exception e) {
            e.printStackTrace();
        }
    }

    private void waitForLogcatReady() {
        try {
            mCountDown.await();
        } catch (InterruptedException e){
            e.printStackTrace();
        }
    }


    private void updateLogcat() {
        ((Activity) mContext).runOnUiThread(() -> {
            StringBuilder sb = new StringBuilder();
            boolean first = true;

            for(int i = 0; i < mLogcatList.size(); ++i) {
                if (first) {
                    first = false;
                } else {
                    sb.append("\n");
                }

                sb.append((mLogcatList.get(i)).getMessage());
            }

            mLogcatEditText.setText(sb.toString());
            mLogcatEditText.setMovementMethod(ScrollingMovementMethod.getInstance());
            mLogcatEditText.setSelection(mLogcatEditText.getText().length(), mLogcatEditText.getText().length());
        });
    }

    private byte[] getPayload() throws Exception {
        byte[] data = null;

        try {
            if (mFile != null) {
                FileInputStream inputStream = new FileInputStream(mFile);
                byte[] bys = new byte[(int)mFile.length()];
                int offset = 0;

                int len;
                for(int remaining = (int)mFile.length(); (len = inputStream.read(bys, offset, Math.min(1024, remaining))) > 0; remaining -= len) {
                    offset += len;
                }

                data = bys;
            }

            return data;
        } catch (Exception exception) {
            throw exception;
        }
    }

    private void addLog(String tag, String message) {
        synchronized(this) {
            if (mLogcatList.size() > 2000) {
                mLogcatList.remove(0);
            }

            mLogcatList.add(new FileChannelView.LogcatInfo(tag, message, System.currentTimeMillis()));
            updateLogcat();
        }
    }

    private String getStatusString(int status) {
        switch(status) {
            case 1:
                return "IDLE";
            case 2:
                return "INITIALIZED";
            case 4:
                return "CONNECTING";
            case 8:
                return "CONNECTED";
            case 16:
                return "DISCONNECTED";
            default:
                return "UNKNOWN";
        }
    }

    public void setFile(File file) {
        this.mFile = file;
        this.mFilePathEditText.setText(this.mFile != null ? this.mFile.getAbsolutePath() : "");
    }

    @Override
    public void onClick(View view) {
        byte[] payload;
        if (view.getId() == R.id.btn_send_data) {
            try {
                if (mFile != null && mFile.exists()) {
                    payload = getPayload();
                    mFileChannelImpl.sendData(payload);
                } else {
                    addLog("duplex", "#The file does not exist.");
                }
            } catch (Exception var5) {
                addLog("duplex", "#" + var5.getMessage());
            }
        } else if (view.getId() == R.id.btn_send_ack_data) {
            try {
                if (mFile != null && mFile.exists()) {
                    payload = getPayload();
                    int timeout = mSeekBar.getProgress() * 1000;
                    mFileChannelImpl.sendData(payload, (long)timeout, new IFileChannel.ISendDataListener() {
                        public void onDataSend() {
                            addLog("duplex", "#Send payload and receive ACK success!!!");
                        }

                        public void onError(int err) {
                            addLog("duplex", "#Send payload failed - " + err);
                        }
                    });
                } else {
                    addLog("duplex", "#The file does not exist.");
                }
            } catch (Exception var4) {
                addLog("duplex", "#" + var4.getMessage());
            }
        } else if (view.getId() == R.id.btn_get_status) {
            addLog("duplex", "#Status is " + getStatusString(mFileChannelImpl.getStatus()) + ".");
        } else if (view.getId() == R.id.btn_choose_file) {
            Intent intent = new Intent("android.intent.action.GET_CONTENT");
            intent.setType("*/*");
            intent.addFlags(Intent.FLAG_GRANT_READ_URI_PERMISSION);
            ((Activity) mContext).startActivityForResult(Intent.createChooser(intent, "请选择文件"), 1);
        }
    }

    public void onActivityResult(int requestCode, int resultCode, Intent data) {
        if (requestCode == 1 && resultCode == Activity.RESULT_OK) {
            Uri uri = data.getData();
            if (uri != null) {
                try {
                    File file = FileUtil.from(mContext, uri);
                    if (file != null && file.exists()) {
                        setFile(file);
                    }
                } catch (IOException e) {
                    e.printStackTrace();
                } catch (Exception e) {
                    e.printStackTrace();
                }
            }
        }
    }

    private static class LogcatInfo {
        private final String tag;
        private final String message;
        private final long timestamp;

        public LogcatInfo(String tag, String message, long timestamp) {
            this.tag = tag;
            this.message = message;
            this.timestamp = timestamp;
        }

        public String getMessage() {
            return this.message;
        }

        public String getTimestamp() {
            Date date = new Date(this.timestamp);
            SimpleDateFormat sd = new SimpleDateFormat("yyyy-MM-dd HH:mm:ss");
            return sd.format(date);
        }

        public String toString() {
            return this.getMessage();
        }
    }

    public class FileChannelImpl implements IFileChannel {

        private IFileChannel mFileChannel;

        public FileChannelImpl(IFileChannel fileChannel) {
            mFileChannel = fileChannel;
        }

        @Override
        public void sendData(byte[] bytes) {
            if (mFileChannel != null) {
                mFileChannel.sendData(bytes);
            }
        }

        @Override
        public void sendData(byte[] bytes, long timeout, ISendDataListener listener) {
            if (mFileChannel != null) {
                mFileChannel.sendData(bytes, timeout, new IFileChannel.ISendDataListener() {
                    @Override
                    public void onDataSend() {
                        listener.onDataSend();
                    }

                    @Override
                    public void onError(int err) {
                        listener.onError(err);
                    }
                });
            }
        }

        @Override
        public void setReceiveDataListener(IReceiveDataListener listener) {
            if (mFileChannel != null) {
                mFileChannel.setReceiveDataListener(new IFileChannel.IReceiveDataListener() {
                    @Override
                    public void onDataReceived(byte[] payload) {
                        listener.onDataReceived(payload);
                    }
                });
            }
        }

        @Override
        public void setStatusListener(IChannelStatusListener listener) {
            if (mFileChannel != null) {
                mFileChannel.setStatusListener(new IFileChannel.IChannelStatusListener() {
                    @Override
                    public void onStateUpdated(int status) {
                        listener.onStateUpdated(status);
                    }
                });
            }
        }

        @Override
        public int getStatus() {
            if (mFileChannel != null) {
                return mFileChannel.getStatus();
            }
            return -1;
        }
    }
}
