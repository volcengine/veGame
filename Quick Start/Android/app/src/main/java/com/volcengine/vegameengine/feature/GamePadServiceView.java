package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.widget.Button;
import android.widget.ScrollView;

import com.volcengine.vegameengine.R;

public class GamePadServiceView extends ScrollView {

//    private GamePadPlus gamePadPlus;

    public GamePadServiceView(Context context) {
        super(context);
        inflate(context, R.layout.dialog_pc, this);
        Button btnGamePad = findViewById(R.id.btn_show_or_hide_game_pad);
        btnGamePad.setOnClickListener(v -> {
//            showOrHideGamePadView(mActivity);
        });
    }
}
