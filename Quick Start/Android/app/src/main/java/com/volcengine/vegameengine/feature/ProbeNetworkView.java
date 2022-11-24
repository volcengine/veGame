package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.app.AlertDialog;
import android.content.Context;
import android.view.Gravity;
import android.view.LayoutInflater;
import android.view.View;
import android.view.WindowManager;
import android.widget.Button;
import android.widget.LinearLayout;
import android.widget.TextView;
import android.widget.Toast;

import com.volcengine.cloudgame.GamePlayConfig;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.IProbeNetworkListener;
import com.volcengine.cloudphone.apiservice.ProbeStats;
import com.volcengine.vegameengine.R;

public class ProbeNetworkView implements View.OnClickListener {

    public interface Callback {
        void onCancel(View view);
    }

    private final Activity mActivity;

    private AlertDialog mDialog;
    private Button mCancelButton;
    private TextView mRttMsTextView;
    private TextView mDownBwKbitTextView;
    private TextView mDownJitterMsTextView;
    private TextView mDownLossPctTextView;
    private TextView mUpBwKbitTextView;
    private TextView mUpJitterMsTextView;
    private TextView mUpLossPctTextView;

    private final Callback mCallback;

    public ProbeNetworkView(Context context, Callback callback) {
        mActivity = (Activity) context;
        mCallback = callback;
    }

    public void show() {
        if (mDialog != null) return;

        AlertDialog.Builder builder = new AlertDialog.Builder(mActivity);


        View view = LayoutInflater.from(mActivity).inflate(R.layout.dialog_probe_network, null);

        mCancelButton = view.findViewById(R.id.btn_cancel);
        mCancelButton.setOnClickListener(this);
        mCancelButton.setEnabled(true);

        mRttMsTextView = view.findViewById(R.id.tv_rtt_ms);
        mDownBwKbitTextView = view.findViewById(R.id.tv_down_bt_kbit);
        mDownJitterMsTextView = view.findViewById(R.id.tv_down_jitter_ms);
        mDownLossPctTextView = view.findViewById(R.id.tv_down_loss_pct);
        mUpBwKbitTextView = view.findViewById(R.id.tv_up_bt_kbit);
        mUpJitterMsTextView = view.findViewById(R.id.tv_up_jitter_ms);
        mUpLossPctTextView = view.findViewById(R.id.tv_up_loss_pct);

        builder.setView(view);
        builder.setCancelable(true);
        mDialog = builder.create();
        mDialog.show();

        mDialog.getWindow().getDecorView().setPadding(0, 0, 0, 0);
        mDialog.getWindow().getDecorView().setBackground(mActivity.getResources().getDrawable(R.drawable.bg_dialog_background));

        mDialog.getWindow().setLayout(WindowManager.LayoutParams.MATCH_PARENT, WindowManager.LayoutParams.MATCH_PARENT);
        mDialog.getWindow().setGravity(Gravity.CENTER);
        mDialog.setOnCancelListener(dialogInterface -> {
            VeGameEngine.getInstance().probeInterrupt();
            dismiss();
        });
    }

    @Override
    public void onClick(View view) {
        if (view.getId() == R.id.btn_cancel) {
            mCancelButton.setEnabled(false);
            if (mCallback != null) {
                mCallback.onCancel(view);
            }
        }
    }

    public void update(int rttMs, int downBwKbit, int downJitterMs, double downLossPct, int upBwKbit, int upJitterMs, double upLossPct) {
        mActivity.runOnUiThread(() -> {
            try {
                mRttMsTextView.setText("rttMs:" + rttMs);
                mDownBwKbitTextView.setText("downBwKbit:" + downBwKbit);
                mDownJitterMsTextView.setText("downJitterMs:" + downJitterMs);
                mDownLossPctTextView.setText("downLossPct:" + downLossPct);
                mUpBwKbitTextView.setText("upBwKbit:" + upBwKbit);
                mUpJitterMsTextView.setText("upJitterMs:" + upJitterMs);
                mUpLossPctTextView.setText("upLossPct:" + upLossPct);
            } catch (Exception e) {
                e.printStackTrace();
            }
        });
    }

    public void dismiss() {
        if (mDialog != null && mDialog.isShowing()) {
            mDialog.dismiss();
            mDialog = null;
        }
    }

    public void showProbeNetworkDialogForGame(GamePlayConfig config) {
        VeGameEngine.getInstance().probeStart(config ,new IProbeNetworkListener() {
            @Override
            public void onProbeStarted() {
                mActivity.runOnUiThread(() -> show() );
            }

            @Override
            public void onProbeProgress(ProbeStats stats) {
                mActivity.runOnUiThread(() -> update(stats.getRtt(),
                        stats.getDownBandwidth(),
                        stats.getDownloadJitter(),
                        stats.getDownloadLossPercent(),
                        stats.getUploadBandwidth(),
                        stats.getUploadJitter(),
                        stats.getUploadLossPercent()));
            }

            @Override
            public void onProbeCompleted(ProbeStats stats, int quality) {
                if (stats != null) {
                    StringBuilder sb = new StringBuilder();
                    sb.append("rttMs:" + stats.getRtt());
                    sb.append(",downBw:" + stats.getDownBandwidth());
                    sb.append(",downJitter:" + stats.getDownloadJitter());
                    sb.append(",downLossPct:" + stats.getDownloadLossPercent());
                    sb.append(",upBw:" + stats.getUploadBandwidth());
                    sb.append(",upJitter:" + stats.getUploadJitter());
                    sb.append(",upLossPct:" + stats.getUploadLossPercent());
                    sb.append(",quality:" + quality);
                    mActivity.runOnUiThread(() -> showToast(mActivity, sb.toString()));
                }

                mActivity.runOnUiThread(() -> dismiss());
            }

            @Override
            public void onProbeError(int err, String message) {
                mActivity.runOnUiThread(() -> showToast(mActivity, message));
                mActivity.runOnUiThread(() -> dismiss());
            }
        });
    }

    private void showToast(Context context, String message) {
        Toast.makeText(context, message, Toast.LENGTH_SHORT).show();
    }
}
