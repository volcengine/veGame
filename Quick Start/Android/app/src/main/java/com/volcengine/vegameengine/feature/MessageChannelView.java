package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.os.Handler;
import android.text.TextUtils;
import android.text.method.ScrollingMovementMethod;
import android.util.Log;
import android.view.View;
import android.view.ViewGroup;
import android.widget.AdapterView;
import android.widget.Button;
import android.widget.CheckBox;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.ScrollView;
import android.widget.Spinner;
import android.widget.TextView;
import android.widget.Toast;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.cloudphone.apiservice.IMessageChannel;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

import java.util.HashMap;

public class MessageChannelView {

    private final IMessageChannel mMessageChannel;
    private static final String TAG = "MessageChannelView";
    private DialogUtils.DialogWrapper mDialogWrapper;

    private HashMap<String, Boolean> channelMap;
    private HashMap<String, Integer> numOfMessageOnChannelMap;
    private StringBuilder log;
    private Handler mHandler;

    public MessageChannelView(Context context, IMessageChannel messageChannel, Button button) {
        channelMap = new HashMap<>();
        numOfMessageOnChannelMap = new HashMap<>();
        log = new StringBuilder();
        this.mMessageChannel = messageChannel;
        mDialogWrapper = DialogUtils.wrapper(new TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends ScrollView {

        int selectedIndex = 0;
        Context _context;
        Spinner spinner;
        EditText etPayload, etChannelUid, etTimeout;
        CheckBox cbAck, cbTimeout;
        Button btnSend;
        TextView tvDisplayPanel, tvStatsInfo;

        final int TYPE_NOT_SELECT = 0;
        final int TYPE_ACK = 1;
        final int TYPE_TIMEOUT = 2;
        final int TYPE_ACK_MultiChannel = 3;
        final int TYPE_TIMEOUT_MultiChannel = 4;

        public TestView(Context context) {
            super(context);
            _context = context;
            inflate(context, R.layout.dialog_message_channel, this);
            setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            initView();
            btnSend.setOnClickListener(view -> {
                if (mMessageChannel == null) {
                    Log.d(TAG, "TestView2: " + "mMessageChannel == null");
                    return;
                }

                switch (selectedIndex) {
                    case TYPE_NOT_SELECT:
                        Toast.makeText(_context, "请选择发送消息类型", Toast.LENGTH_SHORT).show();
                        break;
                    case TYPE_ACK: // sendMessage(String payload, boolean needAck)
                        sendMessageByTypeAck();
                        break;
                    case TYPE_TIMEOUT: // sendMessage(String payload, long timeout)
                        sendMessageByTypeTimeout();
                        break;
                    case TYPE_ACK_MultiChannel: // sendMessage(String payload, boolean needAck, String destChannelUid)
                        sendMessageByTypeAckMultiChannel();
                        break;
                    case TYPE_TIMEOUT_MultiChannel: // sendMessage(String payload, long timeout, String destChannelUid)
                        sendMessageByTypeTimeoutMultiChannel();
                        break;
                    default:
                        break;
                }
            });
        }

        private void sendMessageByTypeAck() {
            IMessageChannel.IChannelMessage message1 =
                    mMessageChannel.sendMessage(etPayload.getText().toString(), cbAck.isChecked());
            AcLog.d(TAG, "data: " + message1);
        }

        private void sendMessageByTypeTimeout() {
            if (cbTimeout.isChecked()) { // 选中超时checkBox--begin
                int timeout = 0;
                if (!TextUtils.isEmpty(etTimeout.getText())) {
                    timeout = Integer.parseInt(etTimeout.getText().toString());
                }
                IMessageChannel.IChannelMessage message2 =
                        mMessageChannel.sendMessage(etPayload.getText().toString(), timeout);
                AcLog.d(TAG, "data: " + message2);

            } // 选中超时checkBox--end
            else { // 未选中超时checkBox
                Toast.makeText(_context, "请勾选超时框", Toast.LENGTH_SHORT).show();
            }
        }

        private void sendMessageByTypeAckMultiChannel() {
            if (!TextUtils.isEmpty(etChannelUid.getText())) { // 填写ChannelUid--begin
                int channelEndUid = Integer.parseInt(etChannelUid.getText().toString());
                for (int i = 1; i <= channelEndUid; i++) {
                    String channelUid = "com.bytedance.vemessagechannelprj.prj" + i;
                    IMessageChannel.IChannelMessage message3 =
                            mMessageChannel.sendMessage(etPayload.getText().toString(), cbAck.isChecked(), channelUid);
                    AcLog.d(TAG, "data: " + message3);
                }
            } // 填写ChannelUid--end
            else { // 未填写ChannelUid--begin
                Toast.makeText(_context, "请输入通道ID", Toast.LENGTH_SHORT).show();
            }
        }

        private void sendMessageByTypeTimeoutMultiChannel() {
            if (cbTimeout.isChecked()) { // 选中超时checkBox--begin
                int timeout2 = 0;
                if (!TextUtils.isEmpty(etTimeout.getText())) {
                    timeout2 = Integer.parseInt(etTimeout.getText().toString());
                }

                if (!TextUtils.isEmpty(etChannelUid.getText())) { // 填写ChannelUid--begin
                    int channelEndUid = Integer.parseInt(etChannelUid.getText().toString());
                    for (int i = 1; i <= channelEndUid; i++) {
                        String channelUid = "com.bytedance.vemessagechannelprj.prj" + i;
                        IMessageChannel.IChannelMessage message4 =
                                mMessageChannel.sendMessage(etPayload.getText().toString(), timeout2, channelUid);
                        AcLog.d(TAG, "data: " + message4);
                    }
                } // 填写ChannelUid--end
                else { // 未填写ChannelUid
                    Toast.makeText(_context, "请输入通道ID", Toast.LENGTH_SHORT).show();
                }
            } // 选中超时checkBox--end
            else { // 未选中超时checkBox
                Toast.makeText(_context, "请勾选超时框", Toast.LENGTH_SHORT).show();
            }
        }

        private void initView() {
            spinner = findViewById(R.id.spinner_send_message2);
            initSpinner(spinner);
            etPayload = findViewById(R.id.et_send_payload2);
            etChannelUid = findViewById(R.id.et_channel_uid2);
            cbAck = findViewById(R.id.cb_ack2);
            cbTimeout = findViewById(R.id.cb_timeout2);
            etTimeout = findViewById(R.id.et_timeout2);
            btnSend = findViewById(R.id.btn_send_message2);
            tvDisplayPanel = findViewById(R.id.display_panel2);
            tvDisplayPanel.setMovementMethod(ScrollingMovementMethod.getInstance());
            tvStatsInfo = findViewById(R.id.stats_info2);
            tvStatsInfo.setMovementMethod(ScrollingMovementMethod.getInstance());
        }

        private void initSpinner(Spinner spinner) {
            spinner.setSelection(0);
            spinner.setOnItemSelectedListener(new AdapterView.OnItemSelectedListener() {
                @Override
                public void onItemSelected(AdapterView<?> adapterView, View view, int i, long l) {
//                    Log.i(TAG, "onItemSelected: select type " + i);`
                    selectedIndex = i;
                }

                @Override
                public void onNothingSelected(AdapterView<?> adapterView) {

                }
            });
        }
    }
}
