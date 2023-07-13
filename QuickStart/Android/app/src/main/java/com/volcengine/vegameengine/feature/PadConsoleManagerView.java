package com.volcengine.vegameengine.feature;

import android.app.Activity;
import android.content.Context;
import android.os.Vibrator;
import android.view.InputDevice;
import android.view.View;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.ScrollView;

import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.gamepad.GamePadService;
import com.volcengine.cloudplay.gamepad.api.OnGamePadStatusListener;
import com.volcengine.cloudplay.gamepad.api.OnPhysicalDeviceListener;
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

    /**
     * 初始化VeGameConsole-SDK
     * 说明：
     * VeGameConsole.getInstance().init()必需；
     * VeGameConsole.getInstance().loadVirtualConsole() 如使用虚拟手柄则必需；
     * VeGameConsole.getInstance().setGamePadService() 可选
     * VeGameConsole.getInstance().registerGameConsoleDevice()如使用物理、非默认虚拟手柄则必需
     * VeGameConsole.getInstance().registerGameConsoleDevice()如使用物理、非默认虚拟手柄则必需
     * VeGameConsole.getInstance().setGamePadStatusListener()可选
     *
     */
    private void initVeGameConsole() {
        // 初始化VeGameConsole SDK
        VeGameConsole.getInstance().init(mContainer.getContext());
        // 加载VeGameConsole SDK虚拟手柄支持
        VeGameConsole.getInstance().loadVirtualConsole(mContainer.getContext(), (FrameLayout) mContainer);
        // 设置云游戏手柄核心服务
        VeGameConsole.getInstance().setGamePadService(VeGameEngine.getInstance().getGamePadService());
        // 添加物理手柄监听，通过onDeviceAdded和onDeviceRemoved回调决定是否注册/解注册手柄
        VeGameConsole.getInstance().setPhysicalDeviceListener(new OnPhysicalDeviceListener() {
            @Override
            public void onDeviceAdded(InputDevice device) {
                // 注册一个物理手柄到云端
                VeGameConsole.getInstance().registerGameConsoleDevice(device.getName(), device.getId());
            }

            @Override
            public void onDeviceRemoved(InputDevice device) {
                // 解除一个云端已注册的物理手柄
                VeGameConsole.getInstance().unregisterGameConsoleDevice(device.getName(), device.getId());
            }
        });

        // 设置云端设备状态变化监听，一般注册\解注册后会收到此消息
        VeGameConsole.getInstance().setGamePadStatusListener(new OnGamePadStatusListener() {
            @Override
            public void onGamePadStatusChanged(int deviceId, boolean enable) {
                // 设备在云端状态改变
            }
        });
    }

    private class TestView extends ScrollView {

        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_pc, this);
            Button gamePadInit = findViewById(R.id.game_console_init);
            Button gamePadShow = findViewById(R.id.game_console_show);
            Button gamePadHide = findViewById(R.id.game_console_hide);
            Button gamePadRelease = findViewById(R.id.game_console_release);

            /**
             * loadConsole(int consoleId, Context context, FrameLayout attachFrame) -- 初始化虚拟手柄
             *
             * @param consoleId 默认为0
             * @param context 虚拟手柄初始化所需上下文
             * @param attachFrame 添加虚拟手柄的View层级，需为FrameLayout以及子类
             *
             * @return 虚拟手柄初始化的结果
             *          0 -- 初始化成功
             *          1 -- 初始化失败，context为空
             *          2 -- 初始化失败，attachFrame为空
             *
             *
             * setGamePadService(GamePadService service) -- 设置GamePadService
             * 该service作为客户端与云端手柄事件传递桥梁，
             * 如果没有设置GamePadService手柄事件将无法传递到云端
             */
            gamePadInit.setOnClickListener(v -> {
                // 初始化VeGameConsole-SDK
                initVeGameConsole();
            });

            /**
             * show() -- 显示手柄
             *
             * @apiNote 显示手柄后，云游戏主容器如需屏蔽Touch事件，请务必调用VeGameEngine.getInstance().setInterceptTouchEvent(true)
             *
             * @return  10 -- 成功
             *          11 -- 失败，手柄未初始化
             *          12 -- 失败，已经是显示状态
             */
            gamePadShow.setOnClickListener(v -> {
                VeGameConsole.getInstance().showVirtual();
                VeGameEngine.getInstance().setInterceptTouchEvent(true);
            });

            /**
             * hide() -- 隐藏手柄
             *
             * @apiNote 隐藏手柄后，云游戏主容器如需重新获取Touch事件，请务必调用VeGameEngine.getInstance().setInterceptTouchEvent(false)
             *
             * @return  10 -- 成功
             *          11 -- 失败，手柄未初始化
             *          13 -- 失败，已经是隐藏状态
             */
            gamePadHide.setOnClickListener(v -> {
                VeGameConsole.getInstance().hideVirtual();
                VeGameEngine.getInstance().setInterceptTouchEvent(false);
            });

            /**
             * release() -- 释放手柄资源
             */
            gamePadRelease.setOnClickListener(v -> VeGameConsole.getInstance().release());
        }

    }
}
