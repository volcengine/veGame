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

import com.volcengine.vegameengine.base.BaseListActivity;
import com.volcengine.vegameengine.feature.AudioServiceActivity;
import com.volcengine.vegameengine.feature.CameraManagerActivity;
import com.volcengine.vegameengine.feature.ClarityServiceActivity;
import com.volcengine.vegameengine.feature.ClipBoardServiceManagerActivity;
import com.volcengine.vegameengine.feature.FileChannelExtActivity;
import com.volcengine.vegameengine.feature.GamePadServiceActivity;
import com.volcengine.vegameengine.feature.GroundManagerActivity;
import com.volcengine.vegameengine.feature.IODeviceManagerActivity;
import com.volcengine.vegameengine.feature.LocalInputManagerActivity;
import com.volcengine.vegameengine.feature.LocationServiceActivity;
import com.volcengine.vegameengine.feature.MessageChannelActivity;
import com.volcengine.vegameengine.feature.MultiUserManagerActivity;
import com.volcengine.vegameengine.feature.OthersActivity;
import com.volcengine.vegameengine.feature.PodControlServiceActivity;
import com.volcengine.vegameengine.feature.ProbeNetworkActivity;
import com.volcengine.vegameengine.feature.SensorActivity;
import com.volcengine.vegameengine.scene.cs.CSDemoActivity;
import com.volcengine.vegameengine.scene.cs.CSDemoWithOutterTouchListenerActivity;

public class MainActivity extends BaseListActivity {

    private static final String TAG = "MainActivity";

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void setupAdapter(ItemsHolder holder) {
        holder.addItem(R.string.audio, R.string.audio_desc, AudioServiceActivity.class);
        holder.addItem(R.string.camera, R.string.camera_desc, CameraManagerActivity.class);
        holder.addItem(R.string.clarity, R.string.clarity_desc, ClarityServiceActivity.class);
        holder.addItem(R.string.clipboard, R.string.clipboard_desc, ClipBoardServiceManagerActivity.class);
        holder.addItem(R.string.file_channel_ext, R.string.file_channel_ext_desc, FileChannelExtActivity.class);
        holder.addItem(R.string.switch_ground, R.string.switch_ground_desc, GroundManagerActivity.class);
        holder.addItem(R.string.io_device, R.string.io_device_desc, IODeviceManagerActivity.class);
        holder.addItem(R.string.local_input, R.string.local_input_desc, LocalInputManagerActivity.class);
        holder.addItem(R.string.location, R.string.location_desc, LocationServiceActivity.class);
        holder.addItem(R.string.message_channel, R.string.message_channel_desc, MessageChannelActivity.class);
        holder.addItem(R.string.multi_user, R.string.multi_user_desc, MultiUserManagerActivity.class);
        holder.addItem(R.string.game_pad, R.string.game_pad_desc, GamePadServiceActivity.class);
        holder.addItem(R.string.pod_control, R.string.pod_control_desc, PodControlServiceActivity.class);
        holder.addItem(R.string.probe_network, R.string.probe_network_desc, ProbeNetworkActivity.class);
        holder.addItem(R.string.sensor, R.string.sensor_desc, SensorActivity.class);
        holder.addItem(R.string.cs_demo, R.string.cs_demo_desc, CSDemoActivity.class);
        holder.addItem(R.string.cs_demo_2, R.string.cs_demo_desc_2, CSDemoWithOutterTouchListenerActivity.class);
        holder.addItem(R.string.unclassified, R.string.unclassified_desc, OthersActivity.class);
    }

    @Override
    public int titleRes() {
        return R.string.app_name;
    }

    @Override
    protected void onResume() {
        super.onResume();
//        // 首次打开应用时，弹出授权弹窗
//        if (getPrefBoolean(String.valueOf(R.string.first_open_app), true)) {
//            showAlertDialog();
//        }
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