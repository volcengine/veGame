package com.volcengine.vegameengine.feature;

import android.content.ClipData;
import android.content.Context;
import android.widget.LinearLayout;

import com.volcengine.cloudphone.apiservice.IClipBoardServiceManager;
import com.volcengine.vegameengine.R;

public class ClipBoardServiceManagerView extends LinearLayout {

    private IClipBoardServiceManager mClipBoardServiceManager;

    public ClipBoardServiceManagerView(Context context, IClipBoardServiceManager clipBoardServiceManager) {
        super(context);
        this.mClipBoardServiceManager = clipBoardServiceManager;
        setOrientation(VERTICAL);
        inflate(context, R.layout.dialog_clipboard, this);

        findViewById(R.id.btn_send_clipData).setOnClickListener(v -> {
            mClipBoardServiceManager.sendClipBoardMessage(ClipData.newPlainText("test", "test data"));
        });
    }
}
