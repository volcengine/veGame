package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
import android.view.KeyEvent;
import android.widget.LinearLayout;
import android.widget.TextView;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.vegameengine.R;

public class UnclassifiedView extends LinearLayout {

    private final Activity mActivity;

    public UnclassifiedView(Context context) {
        super(context);
        this.mActivity = (Activity) context;
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
