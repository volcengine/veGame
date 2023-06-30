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
import com.volcengine.vegameengine.feature.CameraManagerActivity;
import com.volcengine.vegameengine.feature.ClarityServiceActivity;
import com.volcengine.vegameengine.feature.ClipBoardServiceManagerActivity;
import com.volcengine.vegameengine.feature.GamePadServiceActivity;
import com.volcengine.vegameengine.feature.LocalInputManagerActivity;
import com.volcengine.vegameengine.feature.LocationServiceActivity;
import com.volcengine.vegameengine.feature.MessageChannelActivity;
import com.volcengine.vegameengine.feature.MultiUserManagerActivity;
import com.volcengine.vegameengine.feature.OthersActivity;
import com.volcengine.vegameengine.feature.PodControlServiceActivity;
import com.volcengine.vegameengine.feature.ProbeNetworkActivity;
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
        holder.addItem(R.string.camera, R.string.camera_desc, CameraManagerActivity.class, Feature.FEATURE_CAMERA);
        holder.addItem(R.string.clarity, R.string.clarity_desc, ClarityServiceActivity.class, Feature.FEATURE_CLARITY);
        holder.addItem(R.string.clipboard, R.string.clipboard_desc, ClipBoardServiceManagerActivity.class, Feature.FEATURE_CLIPBOARD);
        holder.addItem(R.string.file_channel, R.string.file_channel_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL);
        holder.addItem(R.string.file_channel_ext, R.string.file_channel_ext_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL_EXT);
        holder.addItem(R.string.local_input, R.string.local_input_desc, LocalInputManagerActivity.class, Feature.FEATURE_LOCAL_INPUT);
        holder.addItem(R.string.location, R.string.location_desc, LocationServiceActivity.class, Feature.FEATURE_LOCATION);
        holder.addItem(R.string.message_channel, R.string.message_channel_desc, MessageChannelActivity.class, Feature.FEATURE_MESSAGE_CHANNEL);
        holder.addItem(R.string.multi_user, R.string.multi_user_desc, MultiUserManagerActivity.class, Feature.FEATURE_MULTI_USER);
        holder.addItem(R.string.game_pad, R.string.game_pad_desc, GamePadServiceActivity.class, Feature.FEATURE_GAME_PAD);
        holder.addItem(R.string.pod_control, R.string.pod_control_desc, PodControlServiceActivity.class, Feature.FEATURE_POD_CONTROL);
        holder.addItem(R.string.probe_network, R.string.probe_network_desc, ProbeNetworkActivity.class, Feature.FEATURE_PROBE_NETWORK);
        holder.addItem(R.string.sensor, R.string.sensor_desc, SensorActivity.class, Feature.FEATURE_SENSOR);
        holder.addItem(R.string.unclassified, R.string.unclassified_desc, OthersActivity.class, Feature.FEATURE_UNCLASSIFIED);
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
        alertDialog.setCancelable(false); // 设置点击弹窗外部或者按返回键不消失

        String content = getString(R.string.content);
        SpannableStringBuilder ssb = new SpannableStringBuilder();
        ssb.append(content);
        int privacyIndex = content.indexOf("《");
        ssb.setSpan(new ClickableSpan() {
            @Override
            public void onClick(@NonNull View view) {
                Intent intent = new Intent(MainActivity.this, WebViewActivity.class);
                intent.putExtra("uri", "https://www.volcengine.com/docs/6256/64902");
                startActivity(intent);
            }

            @Override
            public void updateDrawState(@NonNull TextPaint ds) {
                super.updateDrawState(ds);
                ds.setColor(Color.BLUE);
                ds.setUnderlineText(false);
            }
        }, privacyIndex, privacyIndex + 6, 0);

        int serviceIndex = content.lastIndexOf("《");
        ssb.setSpan(new ClickableSpan() {
            @Override
            public void onClick(@NonNull View view) {
                Intent intent = new Intent(getApplication(), WebViewActivity.class);
                intent.putExtra("uri", "https://www.volcengine.com/docs/6256/64903");
                startActivity(intent);
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