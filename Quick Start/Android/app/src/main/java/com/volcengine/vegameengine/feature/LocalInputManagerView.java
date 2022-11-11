package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.view.ViewGroup;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;

import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.cloudphone.apiservice.LocalInputManager;
import com.volcengine.vegameengine.R;

public class LocalInputManagerView extends LinearLayout {

    public LocalInputManagerView(Context context, LocalInputManager localInputManager) {
        super(context);
        inflate(context, R.layout.dialog_local_input, this);
        setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
        EditText inputView = findViewById(R.id.et_text_input);
        SwitchCompat enableInput = findViewById(R.id.switch_show_local_data);
        findViewById(R.id.btn_send_input_data).setOnClickListener(v -> {
            localInputManager.coverCurrentEditTextMessage(inputView.getText().toString());
        });

        enableInput.setChecked(false);
        enableInput.setOnCheckedChangeListener((buttonView, isChecked) ->
                localInputManager.enableShowCurrentInputText(isChecked)
        );

        SwitchCompat enableAutoKeyBoard = findViewById(R.id.switch_close_input_manager);
        enableAutoKeyBoard.setChecked(false);
        enableAutoKeyBoard.setOnCheckedChangeListener((buttonView, isChecked) ->
                localInputManager.closeAutoKeyBoard(isChecked)
        );
    }

}
