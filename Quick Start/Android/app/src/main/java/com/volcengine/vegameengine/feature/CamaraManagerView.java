package com.volcengine.vegameengine.feature;

import android.content.Context;
import android.view.Gravity;
import android.view.SurfaceView;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.FrameLayout;
import android.widget.LinearLayout;
import android.widget.ScrollView;

import androidx.appcompat.widget.SwitchCompat;

import com.volcengine.cloudcore.common.mode.CameraId;
import com.volcengine.cloudcore.common.mode.LocalVideoStreamDescription;
import com.volcengine.cloudcore.common.mode.MirrorMode;
import com.volcengine.cloudphone.apiservice.CameraManager;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

import java.util.ArrayList;
import java.util.List;

public class CamaraManagerView {

    private final CameraManager mCameraManager;
    private static final String TAG = "CamaraManager";
    private DialogUtils.DialogWrapper mDialogWrapper;

    public CamaraManagerView(Context context, CameraManager cameraManager, Button button) {
        this.mCameraManager = cameraManager;
        mDialogWrapper  = DialogUtils.wrapper(new CamaraManagerView.TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends ScrollView {

        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_camera, this);
            setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
            findViewById(R.id.btn_add_camera_canvas).setOnClickListener(v -> {
                FrameLayout view = findViewById(R.id.fm_local_canvas);
                SurfaceView surfaceView = new SurfaceView(context);
                FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
                params.gravity = Gravity.CENTER;
                surfaceView.setZOrderOnTop(true);
                view.addView(surfaceView, params);
                mCameraManager.setLocalVideoCanvas(surfaceView);
            });

            SwitchCompat switchCompat = findViewById(R.id.switch_enable_mirror);
            switchCompat.setChecked(false);
            switchCompat.setOnCheckedChangeListener((compoundButton, b) ->
                    mCameraManager.setLocalVideoMirrorMode(b ? MirrorMode.MIRROR_MODE_ON : MirrorMode.MIRROR_MODE_OFF)
            );

            findViewById(R.id.btn_publish_stream).setOnClickListener(v -> {
                List<LocalVideoStreamDescription> list = new ArrayList<>();
                list.add(new LocalVideoStreamDescription(1920, 1080, 30, 5000));
                list.add(new LocalVideoStreamDescription(1420, 720, 20, 3000));
                list.add(new LocalVideoStreamDescription(1000, 500, 20, 2000));
                mCameraManager.setVideoEncoderConfig(list);
            });

            findViewById(R.id.btn_check_camera_back).setOnClickListener(v -> {
                mCameraManager.switchCamera(CameraId.BACK);
            });

            findViewById(R.id.btn_check_camera_front).setOnClickListener(v -> {
                mCameraManager.switchCamera(CameraId.FRONT);
            });
        }
    }

//    public CamaraManagerView(Context context, CameraManager cameraManager) {
//        super(context);
//        this.mCameraManager = cameraManager;
//        inflate(context, R.layout.dialog_camera, this);
//        setLayoutParams(new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));
//        findViewById(R.id.btn_add_camera_canvas).setOnClickListener(v -> {
//            FrameLayout view = findViewById(R.id.fm_local_canvas);
//            SurfaceView surfaceView = new SurfaceView(context);
//            FrameLayout.LayoutParams params = new FrameLayout.LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.MATCH_PARENT);
//            params.gravity = Gravity.CENTER;
//            surfaceView.setZOrderOnTop(true);
//            view.addView(surfaceView, params);
//            mCameraManager.setLocalVideoCanvas(surfaceView);
//        });
//
//        SwitchCompat switchCompat = findViewById(R.id.switch_enable_mirror);
//        switchCompat.setChecked(false);
//        switchCompat.setOnCheckedChangeListener((compoundButton, b) ->
//                mCameraManager.setLocalVideoMirrorMode(b ? MirrorMode.MIRROR_MODE_ON : MirrorMode.MIRROR_MODE_OFF)
//        );
//
//        findViewById(R.id.btn_publish_stream).setOnClickListener(v -> {
//            List<LocalVideoStreamDescription> list = new ArrayList<>();
//            list.add(new LocalVideoStreamDescription(1920, 1080, 30, 5000));
//            list.add(new LocalVideoStreamDescription(1420, 720, 20, 3000));
//            list.add(new LocalVideoStreamDescription(1000, 500, 20, 2000));
//            mCameraManager.setVideoEncoderConfig(list);
//        });
//
//        findViewById(R.id.btn_check_camera_back).setOnClickListener(v -> {
//            mCameraManager.switchCamera(CameraId.BACK);
//        });
//
//        findViewById(R.id.btn_check_camera_front).setOnClickListener(v -> {
//            mCameraManager.switchCamera(CameraId.FRONT);
//        });
//    }
}
