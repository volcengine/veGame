package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.view.View;
import android.view.ViewGroup;
import android.view.inputmethod.InputMethodManager;
import android.widget.Button;
import android.widget.EditText;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.Toast;

import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.cloudphone.apiservice.LocalInputManager;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

public class LocalInputManagerView {

    private LocalInputManager localInputManager;
    private DialogUtils.DialogWrapper mDialogWrapper;

    public LocalInputManagerView(Context context, LocalInputManager localInputManager, Button btnLocalInput) {
        this.localInputManager = localInputManager;
        mDialogWrapper  = DialogUtils.wrapper(new TestView(context));
        btnLocalInput.setVisibility(View.VISIBLE);
        btnLocalInput.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends LinearLayout {

        public TestView(Context context) {
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

            SwitchCompat enableKeyBoard = findViewById(R.id.switch_close_input_keyboard);
            enableKeyBoard.setChecked(localInputManager.getKeyboardEnable());
            enableKeyBoard.setOnCheckedChangeListener((buttonView, isChecked) -> {
                if (!isChecked) {
                    InputMethodManager inputMethodManager = (InputMethodManager) context.getSystemService(Context.INPUT_METHOD_SERVICE);
                    inputMethodManager.hideSoftInputFromWindow(this.getWindowToken(), 0);
                }
                localInputManager.setKeyBoardEnable(isChecked);
            });
            findViewById(R.id.btn_get_keyboardEnable).setOnClickListener(v -> {
                Toast.makeText(context, "" + localInputManager.getKeyboardEnable(), Toast.LENGTH_SHORT).show();
            });
        }
    }
}
