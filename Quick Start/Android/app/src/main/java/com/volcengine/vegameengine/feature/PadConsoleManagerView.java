package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
import android.os.Vibrator;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ScrollView;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.gamepad.GamePadService;
import com.volcengine.cloudplay.gamepad.api.VeGameConsole;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

public class PadConsoleManagerView {

    Vibrator vibrator;
    FrameLayout mContainer;

    GamePadService mGamePadService;
    private final DialogUtils.DialogWrapper mDialogWrapper;

    public PadConsoleManagerView(Context context, GamePadService gamePadService, Button button) {
        this.mGamePadService = gamePadService;
        vibrator = (Vibrator) context.getSystemService(Context.VIBRATOR_SERVICE);
        mContainer = ((Activity)context).findViewById(R.id.container);
        mDialogWrapper = DialogUtils.wrapper(new PadConsoleManagerView.TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends ScrollView {

        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_pc, this);
            Button gamePadInit = findViewById(R.id.game_console_init);
            Button gamePadShow = findViewById(R.id.game_console_show);
            Button gamePadHide = findViewById(R.id.game_console_hide);
            Button gamePadRelease = findViewById(R.id.game_console_release);
            gamePadInit.setOnClickListener(v -> {
                VeGameConsole.getInstance().loadConsole(0, context, mContainer);
                VeGameConsole.getInstance().setGamePadService(
                        VeGameEngine.getInstance().getGamePadService()
                );
            });
            gamePadShow.setOnClickListener(v -> {
                VeGameConsole.getInstance().show();
                VeGameEngine.getInstance().setInterceptTouchEvent(true);
            });
            gamePadHide.setOnClickListener(v -> {
                VeGameConsole.getInstance().hide();
                VeGameEngine.getInstance().setInterceptTouchEvent(false);
            });
            gamePadRelease.setOnClickListener(v -> VeGameConsole.getInstance().release());
        }

    }
}
