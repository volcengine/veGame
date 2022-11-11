package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
import android.view.KeyEvent;
import android.view.View;
import android.widget.Button;
import android.widget.LinearLayout;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

public class UnclassifiedView {

    private final DialogUtils.DialogWrapper mDialogWrapper;


    public UnclassifiedView(Context context, Button button) {
        mDialogWrapper = DialogUtils.wrapper(new TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private static class TestView extends LinearLayout {

        private final Activity mActivity;

        public TestView(Context context) {
            super(context);
            mActivity = (Activity) context;
            inflate(context, R.layout.dialog_unclassified, this);

            findViewById(R.id.btn_throw_exception).setOnClickListener(v -> {
                throw new IllegalArgumentException("test");
            });

            findViewById(R.id.btn_back).setOnClickListener(v -> {
                VeGameEngine.getInstance().sendKeyEvent(KeyEvent.KEYCODE_BACK);
            });

            findViewById(R.id.btn_restart).setOnClickListener(v -> {
                VeGameEngine.getInstance().restart();
            });

            findViewById(R.id.btn_stop).setOnClickListener(v -> {
                if (mActivity != null) {
                    mActivity.finish();
                }
            });

            findViewById(R.id.btn_pause).setOnClickListener(v -> {
                VeGameEngine.getInstance().pause();
            });

            findViewById(R.id.btn_resume).setOnClickListener(v -> {
                VeGameEngine.getInstance().resume();
            });
        }

    }
}
