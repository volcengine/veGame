package com.volcengine.vegameengine.feature;

import android.content.Context;
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

import java.util.ArrayList;
import java.util.List;

public class PodControlServiceView {

    private final PodControlService mPodControlService;
    private static final String TAG = "PodControlServiceView";
    private DialogUtils.DialogWrapper mDialogWrapper;

    public PodControlServiceView(Context context, PodControlService podControlService, Button button) {
        this.mPodControlService = podControlService;
        mDialogWrapper = DialogUtils.wrapper(new TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends ScrollView {

        private final Consumer<CharSequence> textConsumer;
        private final SwitchCompat mSwitchCompat;
        private final TextView mTvSnapshotPath;

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

            findViewById(R.id.btn_background).setOnClickListener(view -> mPodControlService.switchBackground(true));
            findViewById(R.id.btn_foreground).setOnClickListener(view -> mPodControlService.switchBackground(false));
            findViewById(R.id.btn_send_idle_time).setOnClickListener(v -> mPodControlService.setIdleTime(3 * 60));
            findViewById(R.id.btn_auto_reycle_time).setOnClickListener(v -> {
                mPodControlService.setAutoRecycleTime(180, new PodControlService.SetAutoRecycleTimeCallback() {
                    @Override
                    public void onResult(int i, long l) {
                        Log.d(TAG, "autoRecycleTimeResult" + i + "time" + l);
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
