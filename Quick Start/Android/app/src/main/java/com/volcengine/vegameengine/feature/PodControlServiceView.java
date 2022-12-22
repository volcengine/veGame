package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.text.TextUtils;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.CompoundButton;
import android.widget.EditText;
import android.widget.ScrollView;
import android.widget.TextView;

import androidx.appcompat.widget.SwitchCompat;
import androidx.core.util.Consumer;

import com.blankj.utilcode.util.ToastUtils;
import com.volcengine.cloudphone.apiservice.PodControlService;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

import java.text.DateFormat;
import java.text.MessageFormat;
import java.util.ArrayList;
import java.util.Date;
import java.util.List;

public class PodControlServiceView {

    private final PodControlService mPodControlService;
    private static final String TAG = "PodControlServiceView";
    private DialogUtils.DialogWrapper mDialogWrapper;
    private TestView mTestView;

    public PodControlServiceView(Context context, PodControlService podControlService, Button button) {
        this.mPodControlService = podControlService;
        mTestView = new TestView(context);
        mDialogWrapper = DialogUtils.wrapper(mTestView);
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());

        mPodControlService.setBackgroundSwitchListener(on -> {
            mTestView.checkWithoutCallback(on);
            mTestView.textConsumer.accept("onBackgroundSwitched: on = [" + on + "] at " + DateFormat.getInstance().format(new Date()));
        });
        mPodControlService.setUserProfilePathListener((isSuccess, type)
                -> ToastUtils.showShort("isSuccess:" + isSuccess + " type :" + type));
        mPodControlService.setScreenShotListener((code, savePath, msg) -> {
            if (!TextUtils.isEmpty(savePath)) {
                mTestView.mTvSnapshotPath.setText(savePath);
            } else {
                mTestView.mTvSnapshotPath.setText(MessageFormat.format("code = [{0}], msg = [{1}]", code, msg));
            }
        });
        mPodControlService.setFocusedWindowAppListener(new PodControlService.FocusedWindowAppListener() {
            @Override
            public void onResult(int code, String packageName, String msg) {
                Log.d(TAG, "onResult: code = [" + code + "], packageName = [" + packageName + "], msg = [" + msg + "]");
                if (mTestView != null) {
                    mTestView.mTvAppPackage.setText(MessageFormat.format("code = [{0}], packageName = [{1}], msg = [{2}]", code, packageName, msg));
                }
            }

            @Override
            public void onFocusedWindowAppChanged(String packageName) {
                Log.d(TAG, "onFocusedWindowAppChanged: packageName = [" + packageName + "]");
            }
        });
    }

    private class TestView extends ScrollView {

        private final Consumer<CharSequence> textConsumer;
        private final SwitchCompat mSwitchCompat;
        private final TextView mTvSnapshotPath, mTvAppPackage;
        private final EditText mEtAutoRecycleTime, mEtIdleTime;

        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_pod_control, this);

            TextView text = findViewById(R.id.tv_info);

            textConsumer = charSequence -> {
                text.append(charSequence);
                text.append("\n");
            };

            mSwitchCompat = findViewById(R.id.switch_background);
            mSwitchCompat.setOnCheckedChangeListener(mOnCheckedChangeListener);

            mEtAutoRecycleTime = findViewById(R.id.et_auto_recycle_time);
            mEtIdleTime = findViewById(R.id.et_idle_time);

            findViewById(R.id.btn_background).setOnClickListener(view -> mPodControlService.switchBackground(true));
            findViewById(R.id.btn_foreground).setOnClickListener(view -> mPodControlService.switchBackground(false));
            findViewById(R.id.btn_send_idle_time).setOnClickListener(v -> mPodControlService.setIdleTime(3 * 60));
            findViewById(R.id.btn_send_idle_time).setOnClickListener(v -> {
                int idleTime = 3 * 60;
                if (!TextUtils.isEmpty(mEtIdleTime.getText())) {
                    idleTime = Integer.parseInt(mEtIdleTime.getText().toString());
                }
                mPodControlService.setIdleTime(idleTime);
            });
            findViewById(R.id.btn_set_auto_recycle_time).setOnClickListener(v -> {
                int autoRecycleTime = 3 * 60;
                if (!TextUtils.isEmpty(mEtAutoRecycleTime.getText())) {
                    autoRecycleTime = Integer.parseInt(mEtAutoRecycleTime.getText().toString());
                }
                mPodControlService.setAutoRecycleTime(autoRecycleTime, new PodControlService.SetAutoRecycleTimeCallback() {
                    @Override
                    public void onResult(int i, long l) {
                        Log.d(TAG, "setAutoRecycleTimeResult" + i + "time" + l);
                    }
                });
            });
            findViewById(R.id.btn_get_auto_recycle_time).setOnClickListener(v -> {
                mPodControlService.getAutoRecycleTime(new PodControlService.GetAutoRecycleTimeCallback() {
                    @Override
                    public void onResult(int code, long time) {
                        Log.d(TAG, "getAutoRecycleTimeResult" + code + "time" + time);
                    }
                });
            });

            findViewById(R.id.btn_getUserProfile).setOnClickListener(v -> {
                mPodControlService.getUserProfilePath(list -> {
                    ToastUtils.showShort(list.toString());
                });
            });


            findViewById(R.id.btn_setUserProfile).setOnClickListener(v -> {
                EditText editText = findViewById(R.id.et_user_profile_input);
                List<String> list = new ArrayList<>();
                list.add(editText.getText().toString());
                mPodControlService.setUserProfilePath(list);
            });

            findViewById(R.id.btn_snapshot).setOnClickListener(v -> mPodControlService.screenShot());
            mTvSnapshotPath = findViewById(R.id.tv_snapshot_path);

            mTvAppPackage = findViewById(R.id.tv_app_package);
            findViewById(R.id.btn_get_focus_window).setOnClickListener(v -> mPodControlService.getFocusedWindowApp());
        }

        boolean mWithoutCallback;

        void checkWithoutCallback(boolean check) {
            mWithoutCallback = true;
            mSwitchCompat.setChecked(check);
            mWithoutCallback = false;
        }

        final CompoundButton.OnCheckedChangeListener mOnCheckedChangeListener = new CompoundButton.OnCheckedChangeListener() {
            @Override
            public void onCheckedChanged(CompoundButton compoundButton, boolean isChecked) {
                if (mWithoutCallback) {
                    return;
                }
                mPodControlService.switchBackground(isChecked);
            }
        };
    }
}
