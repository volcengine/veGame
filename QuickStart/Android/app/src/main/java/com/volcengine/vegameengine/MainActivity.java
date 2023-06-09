package com.volcengine.vegameengine;

import static com.volcengine.vegameengine.util.PrefUtilsKt.getPrefBoolean;
import static com.volcengine.vegameengine.util.PrefUtilsKt.putPrefBoolean;

import android.content.Intent;
import android.graphics.Color;
import android.os.Bundle;
import android.text.SpannableStringBuilder;
import android.text.TextPaint;
import android.text.method.LinkMovementMethod;
import android.text.style.ClickableSpan;
import android.view.View;
import android.view.Window;
import android.widget.TextView;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AlertDialog;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.vegameengine.base.BaseListActivity;
import com.volcengine.vegameengine.feature.MessageChannelActivity;
import com.volcengine.vegameengine.feature.SensorActivity;
import com.volcengine.vegameengine.util.Feature;

public class MainActivity extends BaseListActivity {

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void setupAdapter(ItemsHolder holder) {
        holder.addItem(R.string.audio, R.string.audio_desc, FeatureActivity.class, Feature.FEATURE_AUDIO);
        holder.addItem(R.string.camera, R.string.camera_desc, FeatureActivity.class, Feature.FEATURE_CAMERA);
        holder.addItem(R.string.clipboard, R.string.clipboard_desc, FeatureActivity.class, Feature.FEATURE_CLIPBOARD);
        holder.addItem(R.string.file_channel, R.string.file_channel_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL);
        holder.addItem(R.string.file_channel_ext, R.string.file_channel_ext_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL_EXT);
        holder.addItem(R.string.local_input, R.string.local_input_desc, FeatureActivity.class, Feature.FEATURE_LOCAL_INPUT);
        holder.addItem(R.string.location, R.string.location_desc, FeatureActivity.class, Feature.FEATURE_LOCATION);
        holder.addItem(R.string.message_channel, R.string.message_channel_desc, MessageChannelActivity.class, Feature.FEATURE_MESSAGE_CHANNEL);
        holder.addItem(R.string.multi_user, R.string.multi_user_desc, FeatureActivity.class, Feature.FEATURE_MULTI_USER);
        holder.addItem(R.string.pad_console, R.string.pad_console_desc, FeatureActivity.class, Feature.FEATURE_PAD_CONSOLE);
        holder.addItem(R.string.pod_control, R.string.pod_control_desc, FeatureActivity.class, Feature.FEATURE_POD_CONTROL);
        holder.addItem(R.string.probe_network, R.string.probe_network_desc, FeatureActivity.class, Feature.FEATURE_PROBE_NETWORK);
        holder.addItem(R.string.sensor, R.string.sensor_desc, SensorActivity.class, Feature.FEATURE_SENSOR);
        holder.addItem(R.string.unclassified, R.string.unclassified_desc, FeatureActivity.class, Feature.FEATURE_UNCLASSIFIED);
    }

    @Override
    public int titleRes() {
        return R.string.app_name;
    }

    @Override
    protected void onResume() {
        super.onResume();
        if (getPrefBoolean(String.valueOf(R.string.first_open_app), true)) {
            showAlertDialog();
        }
    }

    private void showAlertDialog() {
        AlertDialog alertDialog = new AlertDialog.Builder(this).create();
        alertDialog.show();
        alertDialog.setCancelable(false);

        String str = "欢迎使用火山引擎云游戏! 我们非常重视您的个人信息和隐私保护。" +
                "为了更好地保障您的个人权益，在您使用我们的产品前，请务必审慎阅读《隐私政策》和《服务条款》内的所有条款。" +
                "您点击\"同意并继续\"的行为即表示您已阅读完毕并同意以上协议的全部内容。\n\n" +
                "如您同意以上协议内容，请点击\"同意并继续\"，开始使用我们的产品和服务!";
        SpannableStringBuilder ssb = new SpannableStringBuilder();
        ssb.append(str);
        int privacyIndex = str.indexOf("《");
        ssb.setSpan(new ClickableSpan() {
            @Override
            public void onClick(@NonNull View view) {
                Intent intent = new Intent(MainActivity.this, WebViewActivity.class);
                intent.putExtra("uri", "https://www.volcengine.com/docs/6256/64902");
                startActivity(intent);
                alertDialog.cancel();
            }

            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setColor(Color.BLUE);
                ds.setUnderlineText(false);
            }
        }, privacyIndex, privacyIndex + 6, 0);

        int serviceIndex = str.lastIndexOf("《");
        ssb.setSpan(new ClickableSpan() {
            @Override
            public void onClick(@NonNull View view) {
                Intent intent = new Intent(getApplication(), WebViewActivity.class);
                intent.putExtra("uri", "https://www.volcengine.com/docs/6256/64903");
                startActivity(intent);
                alertDialog.cancel();
            }

            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setColor(Color.BLUE);
                ds.setUnderlineText(false);
            }
        }, serviceIndex, serviceIndex + 6, 0);

        final Window window = alertDialog.getWindow();
        if (window != null) {
            window.setContentView(R.layout.dialog_privacy);

            TextView tvContent = window.findViewById(R.id.tv_content);
            tvContent.setMovementMethod(LinkMovementMethod.getInstance());
            tvContent.setText(ssb, TextView.BufferType.SPANNABLE);

            TextView tvCancel = window.findViewById(R.id.tv_cancel);
            tvCancel.setOnClickListener(v -> {
                putPrefBoolean(String.valueOf(R.string.first_open_app), true);
                alertDialog.cancel();
                finish();
            });

            TextView tvConfirm = window.findViewById(R.id.tv_confirm);
            tvConfirm.setOnClickListener(v -> {
                putPrefBoolean(String.valueOf(R.string.first_open_app), false);
                alertDialog.cancel();
            });
        }
    }
}