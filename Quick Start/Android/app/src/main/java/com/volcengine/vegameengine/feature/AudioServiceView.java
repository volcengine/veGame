package com.volcengine.vegameengine.feature;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.util.Log;
import android.util.SparseIntArray;
import android.view.ViewGroup;
import android.widget.Button;
import android.widget.RadioGroup;
import android.widget.ScrollView;
import android.widget.SeekBar;
import android.widget.Toast;

import androidx.appcompat.widget.SwitchCompat;

import com.blankj.utilcode.util.PermissionUtils;
import com.volcengine.cloudcore.common.mode.AudioPlaybackDevice;
import com.volcengine.cloudgame.VeGameEngine;
import com.volcengine.cloudphone.apiservice.AudioService;
import com.volcengine.vegameengine.R;

import java.util.Locale;

public class AudioServiceView extends ScrollView implements AudioService.AudioControlListener {
    private final SeekBar mSbLocalAudioPlaybackVolume;
    private final SeekBar mSbRemoteAudioPlaybackVolume;
    private final SparseIntArray mDeviceArray = new SparseIntArray();
    private final RadioGroup mRgAudioPlaybackDevice;
    private final SeekBar mSbLocalAudioCaptureVolume;
    private final Button mBtnLocalAudioPlaybackVolume;
    private final Button mBtnRemoteAudioPlaybackVolume;
    private final Button mBtnLocalAudioCaptureVolume;
    private final AudioService mAudioManager;
    private static final String TAG = "AudioServiceView";

    {
        mDeviceArray.put(R.id.rb_headset, AudioPlaybackDevice.HEADSET);
        mDeviceArray.put(R.id.rb_earpiece, AudioPlaybackDevice.EARPIECE);
        mDeviceArray.put(R.id.rb_speakerphone, AudioPlaybackDevice.SPEAKERPHONE);
        mDeviceArray.put(R.id.rb_bluetooth_headset, AudioPlaybackDevice.HEADSET_BLUETOOTH);
    }

    public AudioServiceView(Context context, AudioService audioManager) {
        super(context);
        this.mAudioManager = audioManager;
        inflate(context, R.layout.dialog_audio, this);

        setLayoutParams(new LayoutParams(ViewGroup.LayoutParams.MATCH_PARENT, ViewGroup.LayoutParams.WRAP_CONTENT));

        mAudioManager.setAudioControlListener(this);


        findViewById(R.id.btn_mute).setOnClickListener(v -> {
            VeGameEngine engine = VeGameEngine.getInstance();
            engine.muteAudio(!engine.isAudioMuted());
        });

        findViewById(R.id.btn_volumeDown).setOnClickListener(v -> {
            VeGameEngine.getInstance().volumeDown();
        });

        findViewById(R.id.btn_volumeUp).setOnClickListener(v -> {
            VeGameEngine.getInstance().volumeUp();
        });

        SwitchCompat switchCompat = findViewById(R.id.switch_enable);
        switchCompat.setChecked(mAudioManager.isEnableSendAudioStream());
        switchCompat.setOnCheckedChangeListener((compoundButton, b) -> mAudioManager.setEnableSendAudioStream(b));

        findViewById(R.id.btn_start_send_audio).setOnClickListener(view -> onRemoteAudioStartRequest());
        findViewById(R.id.btn_stop_send_audio).setOnClickListener(view -> onRemoteAudioStopRequest());

        // 本地播放音量
        mSbLocalAudioPlaybackVolume = findViewById(R.id.sb_local_audio_playback_volume);
        mBtnLocalAudioPlaybackVolume = findViewById(R.id.btn_local_audio_playback_volume);
        mBtnLocalAudioPlaybackVolume.setOnClickListener(view -> {
            int volume = mAudioManager.getLocalAudioPlaybackVolume();
            mSbLocalAudioPlaybackVolume.setProgress(volume);
            mBtnLocalAudioPlaybackVolume.setText(String.format(Locale.getDefault(), "get[%d]", volume));
        });
        mSbLocalAudioPlaybackVolume.setProgress(mAudioManager.getLocalAudioPlaybackVolume());
        mSbLocalAudioPlaybackVolume.setOnSeekBarChangeListener(mOnSeekBarChangeListener);
        // 远端音量
        mSbRemoteAudioPlaybackVolume = findViewById(R.id.sb_remote_audio_playback_volume);
        mSbRemoteAudioPlaybackVolume.setProgress(mAudioManager.getRemoteAudioPlaybackVolume());
        mBtnRemoteAudioPlaybackVolume = findViewById(R.id.btn_remote_audio_playback_volume);
        mBtnRemoteAudioPlaybackVolume.setOnClickListener(view -> {
            int volume = mAudioManager.getRemoteAudioPlaybackVolume();
            mSbRemoteAudioPlaybackVolume.setProgress(volume);
            mBtnRemoteAudioPlaybackVolume.setText(String.format(Locale.getDefault(), "get[%d]", volume));
        });
        mSbRemoteAudioPlaybackVolume.setOnSeekBarChangeListener(mOnSeekBarChangeListener);
        // 本地采集音量
        mSbLocalAudioCaptureVolume = findViewById(R.id.sb_local_audio_capture_volume);
        mSbLocalAudioCaptureVolume.setProgress(mAudioManager.getLocalAudioCaptureVolume());
        mBtnLocalAudioCaptureVolume = findViewById(R.id.btn_local_audio_capture_volume);
        mBtnLocalAudioCaptureVolume.setOnClickListener(view -> {
            int volume = mAudioManager.getLocalAudioCaptureVolume();
            mSbLocalAudioCaptureVolume.setProgress(volume);
            mBtnLocalAudioCaptureVolume.setText(String.format(Locale.getDefault(), "get[%d]", volume));
        });
        mSbLocalAudioCaptureVolume.setOnSeekBarChangeListener(mOnSeekBarChangeListener);

        mRgAudioPlaybackDevice = findViewById(R.id.rg_audio_playback_device);
        int audioPlaybackDevice = mAudioManager.getAudioPlaybackDevice();
        for (int i = 0; i < mDeviceArray.size(); i++) {
            int id = mDeviceArray.keyAt(i);
            if (mDeviceArray.get(id) == audioPlaybackDevice) {
                mRgAudioPlaybackDevice.check(id);
            }
        }

        mRgAudioPlaybackDevice.setOnCheckedChangeListener(mOnCheckedChangeListener);

    }

    private final SeekBar.OnSeekBarChangeListener mOnSeekBarChangeListener = new SeekBar.OnSeekBarChangeListener() {
        @Override
        public void onProgressChanged(SeekBar seekBar, int i, boolean b) {
            if (seekBar == mSbLocalAudioCaptureVolume) {
                mBtnLocalAudioCaptureVolume.setText(String.format(Locale.getDefault(), "get[%d]", i));
            } else if (seekBar == mSbRemoteAudioPlaybackVolume) {
                mBtnRemoteAudioPlaybackVolume.setText(String.format(Locale.getDefault(), "get[%d]", i));
            } else if (seekBar == mSbLocalAudioPlaybackVolume) {
                mBtnLocalAudioPlaybackVolume.setText(String.format(Locale.getDefault(), "get[%d]", i));
            }
        }

        @Override
        public void onStartTrackingTouch(SeekBar seekBar) {
        }

        @Override
        public void onStopTrackingTouch(SeekBar seekBar) {
            int i = seekBar.getProgress();
            if (seekBar == mSbLocalAudioCaptureVolume) {
                mAudioManager.setLocalAudioCaptureVolume(i);
            } else if (seekBar == mSbRemoteAudioPlaybackVolume) {
                mAudioManager.setRemoteAudioPlaybackVolume(i);
            } else if (seekBar == mSbLocalAudioPlaybackVolume) {
                mAudioManager.setLocalAudioPlaybackVolume(i);
            }
        }
    };

    private final RadioGroup.OnCheckedChangeListener mOnCheckedChangeListener = new RadioGroup.OnCheckedChangeListener() {
        @Override
        public void onCheckedChanged(RadioGroup radioGroup, int i) {
            int device = mDeviceArray.get(i);
            Log.d(TAG, "onCheckedChanged: device = " + device);
            mAudioManager.setAudioPlaybackDevice(device);
        }
    };


    @Override
    public void onRemoteAudioPlaybackVolumeChanged(int volume) {
        Log.d(TAG, "onRemoteAudioPlaybackVolumeChanged() called with: volume = [" + volume + "]");
        mSbRemoteAudioPlaybackVolume.setProgress(volume);
    }

    @Override
    public void onRemoteAudioStartRequest() {
        Log.d(TAG, "onRemoteAudioStartRequest() called");
        PermissionUtils.permission(Manifest.permission.RECORD_AUDIO)
                .callback(new PermissionUtils.SimpleCallback() {
                    @SuppressLint("MissingPermission")
                    @Override
                    public void onGranted() {
                        mAudioManager.startSendAudioStream();
                        Log.d(TAG, "startSendAudioStream: " + mAudioManager.isSendingAudioStream());
                    }

                    @Override
                    public void onDenied() {
                        Toast.makeText(getContext(), "无录音权限", Toast.LENGTH_SHORT).show();
                    }
                }).request();
    }

    @Override
    public void onRemoteAudioStopRequest() {
        Log.d(TAG, "onRemoteAudioStopRequest() called");
        mAudioManager.stopSendAudioStream();
        Log.d(TAG, "stopSendAudioStream: " + mAudioManager.isSendingAudioStream());
    }

    @Override
    public void onAudioPlaybackDeviceChanged(int device) {
        Log.d(TAG, "onAudioPlaybackDeviceChanged() called with: device = [" + device + "]");
        for (int i = 0; i < mDeviceArray.size(); i++) {
            int id = mDeviceArray.keyAt(i);
            if (mDeviceArray.get(id) == device) {
                mRgAudioPlaybackDevice.setOnCheckedChangeListener(null);
                mRgAudioPlaybackDevice.check(id);
                mRgAudioPlaybackDevice.setOnCheckedChangeListener(mOnCheckedChangeListener);
            }
        }
    }
}
