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
                /**
                 * setRemoteGameForeground() -- 切换远端实例的APP到前台
                 */
                mGameGroundSwitchManager.setRemoteGameForeground();
            }
        }));

        TextView text = new TextView(getContext());
        addView(text, ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT);
        textConsumer = charSequence -> text.setText(charSequence);
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
            /**
             * 云游戏App切换到后台的回调
             *
             * @param switchType 切换类型
             *                   0 -- 客户端手动切换
             *                   1 -- 远端实例自动切换
             */
            @Override
            public void onRemoteGameSwitchedBackground(int switchType) {
                showTargetToast(true, switchType, true, "");
                AcLog.d(TAG, "onRemoteGameSwitchedBackground");
            }

            /**
             * 云游戏App切换到前台的回调
             *
             * @param switchType 切换类型
             *                   0 -- 客户端手动切换
             *                   1 -- 远端实例自动切换
             */
            @Override
            public void onRemoteGameSwitchedForeground(int switchType) {
                showTargetToast(true, switchType, false, "");
                AcLog.d(TAG, "onRemoteGameSwitchedForeground.");
            }

            /**
             * 云游戏App切换前后台失败回调
             *
             * @param errorCode 错误码
             * @param errorMsg 具体的错误信息
             */
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
