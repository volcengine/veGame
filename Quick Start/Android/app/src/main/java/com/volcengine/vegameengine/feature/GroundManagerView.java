package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.graphics.Color;
import android.view.Gravity;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.TextView;

import androidx.core.util.Consumer;

import com.blankj.utilcode.util.SizeUtils;
import com.blankj.utilcode.util.ToastUtils;
import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.cloudphone.apiservice.GameGroundSwitchManager;

public class GroundManagerView extends LinearLayout {

    private final Consumer<CharSequence> textConsumer;
    private static final String TAG = "GroundManagerView";
    GameGroundSwitchManager mGameGroundSwitchManager;
    private EditText packageNameInputView;

    public void setText(String text) {
        textConsumer.accept(text);
    }

    public GroundManagerView(Context context, GameGroundSwitchManager gameGroundSwitchManager) {
        super(context);
        this.mGameGroundSwitchManager = gameGroundSwitchManager;
        initGameGroundCallback();
        setOrientation(VERTICAL);
        addView(createEditRow(new View.OnClickListener() {

            @Override
            public void onClick(View v) {
                String packageName = packageNameInputView.getText().toString();
                mGameGroundSwitchManager.setRemoteGameForeground();
            }
        }));

        TextView text = new TextView(getContext());
        addView(text, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        textConsumer = charSequence -> text.setText(charSequence);
    }


    private View createButtonRow(View.OnClickListener listener) {
        Button button = new Button(getContext());
        button.setText("获取");
        button.setOnClickListener(listener);
        button.setAllCaps(false);

        TextView label = new TextView(getContext());
        label.setText("获取后台应用信息");
        label.setTextColor(Color.BLACK);
        int padding = SizeUtils.dp2px(12);
        label.setPadding(padding, 0, padding, 0);
        LinearLayout row = new LinearLayout(getContext());
        LayoutParams lp = new LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT);
        row.setGravity(Gravity.CENTER_VERTICAL);
        lp.weight = 1;
        row.addView(label, lp);
        row.addView(button, lp);
        return row;
    }

    private View createEditRow(View.OnClickListener listener) {
        Button button = new Button(getContext());
        button.setText("切前台");
        button.setOnClickListener(listener);

        packageNameInputView = new EditText(getContext());
        packageNameInputView.setHint("packageName");

        LinearLayout row = new LinearLayout(getContext());
        row.setGravity(Gravity.CENTER_VERTICAL);
        LayoutParams lp = new LayoutParams(0, ViewGroup.LayoutParams.WRAP_CONTENT);
        lp.weight = 1;
        row.addView(packageNameInputView, lp);
        row.addView(button, lp);

        return row;
    }

    private void initGameGroundCallback() {
        mGameGroundSwitchManager.setGroundChangeListener(new GameGroundSwitchManager.GameGroundSwitchedListener() {
            @Override
            public void onRemoteGameSwitchedBackground(int switchType) {
                showTargetToast(true, switchType, true, "");
                AcLog.d(TAG, "onRemoteGameSwitchedBackground");
            }

            @Override
            public void onRemoteGameSwitchedForeground(int switchType) {
                showTargetToast(true, switchType, false, "");
                AcLog.d(TAG, "onRemoteGameSwitchedForeground.");
            }

            @Override
            public void onRemoteGameSwitchedFailed(int errorCode, String errorMsg) {
                ToastUtils.showShort("errorCode: " + errorCode + "errorMsg: " + errorMsg, true);
                AcLog.d(TAG, "onRemoteGameSwitchedFailed");
            }
        });
    }

    private void showTargetToast(boolean gameOrPhone, int autoOrInactive, boolean backgroundOrForeground, String appInfo) {
        String text = gameOrPhone ? "游戏" : "应用";
        text += autoOrInactive == 1 ? "自动" : "主动";
        text += backgroundOrForeground ? "切换至后台" : "切换至前台";
        text += gameOrPhone ? "" : (" " + appInfo.toString());
        ToastUtils.showShort(text, true);
    }
}
