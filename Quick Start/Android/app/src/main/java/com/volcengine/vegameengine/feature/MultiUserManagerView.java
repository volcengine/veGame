package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.LinearLayout;
import android.widget.RadioButton;
import android.widget.RadioGroup;
import android.widget.TextView;
import android.widget.Toast;

import com.volcengine.androidcloud.common.log.AcLog;
import com.volcengine.cloudcore.common.mode.Role;
import com.volcengine.cloudphone.apiservice.outinterface.MultiUserService;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

import java.util.Locale;

public class MultiUserManagerView {

    private static final String TAG = "TestMultiUserManager";

    private MultiUserService mMultiUserManager;
    private DialogUtils.DialogWrapper mDialogWrapper;

    private Role mRole = Role.PLAYER;

    private Context mContext;

    public MultiUserManagerView(Context context, MultiUserService multiUserService, Button button) {
        mContext = context;
        mMultiUserManager = multiUserService;
        mDialogWrapper = DialogUtils.wrapper(new TestView(context));
        mMultiUserManager.setRoomListener(new MultiUserService.RoomListener() {

            @Override
            public void onPlayerChanged(String userId) {
                AcLog.d(TAG, "onPlayerChanged " + userId);
                showToast("onPlayerChanged" + userId);
            }

            @Override
            public void onJoinRoomRoleResult(Role role, int reason, String playerUid) {
                AcLog.d(TAG, "onJoinRoomRoleResult " + role + "，reason " + reason + " ,PlayerUid " + playerUid);
                showToast(String.format(Locale.getDefault(), "onJoinRoomRoleResult role: %d, reason : %d , playerUid %S  ",
                        role.id, reason, playerUid));
            }
        });
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends LinearLayout {
        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_multi_user, this);
            RadioGroup radioGroup = findViewById(R.id.group_choose_player);
            RadioButton radioButtonPlayer = findViewById(R.id.btn_player);
            radioButtonPlayer.setChecked(true);

            EditText editText = findViewById(R.id.text_userId);

            radioGroup.setOnCheckedChangeListener((group, checkedId) -> {
                if (checkedId == R.id.btn_player) {
                    mRole = Role.PLAYER;
                } else if (checkedId == R.id.btn_viewer) {
                    mRole = Role.VIEWER;
                }
            });

            findViewById(R.id.btn_change_update).setOnClickListener(v -> {
                mMultiUserManager.changeRole(editText.getText().toString(), mRole, (userId, role, result) -> {
                    Toast.makeText(context, "result " + result + ", userId " + userId + ", role " + role,
                            Toast.LENGTH_SHORT).show();
                    AcLog.d(TAG, "change result " + userId + ", role " + role + "result " + result);
                });
            });

            findViewById(R.id.btn_get_role).setOnClickListener(v -> {
                TextView textView = findViewById(R.id.tv_current_role);
                textView.setText("" + mMultiUserManager.getCurrentRole());
            });
        }
    }

    private void showToast(String s) {
        Toast.makeText(mContext, s, Toast.LENGTH_SHORT).show();
    }

}
