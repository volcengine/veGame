package com.volcengine.vegameengine;

import android.os.Bundle;
import com.volcengine.vegameengine.base.BaseListActivity;
import com.volcengine.vegameengine.util.Feature;

public class MainActivity extends BaseListActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
    }

    @Override
    protected void setupAdapter(ItemsHolder holder) {
<<<<<<< HEAD
        holder.addItem(R.string.audio_title, R.string.audio_desc, FeatureActivity.class, Feature.FEATURE_AUDIO);
        holder.addItem(R.string.camera_title, R.string.camera_desc, FeatureActivity.class, Feature.FEATURE_CAMERA);
        holder.addItem(R.string.clipboard_title, R.string.clipboard_desc, FeatureActivity.class, Feature.FEATURE_CLIPBOARD);
        holder.addItem(R.string.file_channel_title, R.string.file_channel_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL);
        holder.addItem(R.string.local_input_title, R.string.local_input_desc, FeatureActivity.class, Feature.FEATURE_LOCAL_INPUT);
        holder.addItem(R.string.location_title, R.string.location_desc, FeatureActivity.class, Feature.FEATURE_LOCATION);
        holder.addItem(R.string.message_channel_title, R.string.message_channel_desc, FeatureActivity.class, Feature.FEATURE_MESSAGE_CHANNEL);
        holder.addItem(R.string.pod_control_title, R.string.pod_control_desc, FeatureActivity.class, Feature.FEATURE_POD_CONTROL);
        holder.addItem(R.string.probe_network_title, R.string.probe_network_desc, FeatureActivity.class, Feature.FEATURE_PROBE_NETWORK);
        holder.addItem(R.string.sensor_title, R.string.sensor_desc, FeatureActivity.class, Feature.FEATURE_SENSOR);
        holder.addItem(R.string.unclassified_title, R.string.unclassified_desc, FeatureActivity.class, Feature.FEATURE_UNCLASSIFIED);
=======
        holder.addItem(R.string.audio, R.string.audio_desc, FeatureActivity.class, Feature.FEATURE_AUDIO);
        holder.addItem(R.string.camera, R.string.camera_desc, FeatureActivity.class, Feature.FEATURE_CAMERA);
        holder.addItem(R.string.clipboard, R.string.clipboard_desc, FeatureActivity.class, Feature.FEATURE_CLIPBOARD);
        holder.addItem(R.string.file_channel, R.string.file_channel_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL);
        holder.addItem(R.string.local_input, R.string.local_input_desc, FeatureActivity.class, Feature.FEATURE_LOCAL_INPUT);
        holder.addItem(R.string.location, R.string.location_desc, FeatureActivity.class, Feature.FEATURE_LOCATION);
        holder.addItem(R.string.message_channel, R.string.message_channel_desc, FeatureActivity.class, Feature.FEATURE_MESSAGE_CHANNEL);
        holder.addItem(R.string.pad_console, R.string.pad_console_desc, FeatureActivity.class, Feature.FEATURE_PAD_CONSOLE);
        holder.addItem(R.string.pod_control, R.string.pod_control_desc, FeatureActivity.class, Feature.FEATURE_POD_CONTROL);
        holder.addItem(R.string.probe_network, R.string.probe_network_desc, FeatureActivity.class, Feature.FEATURE_PROBE_NETWORK);
        holder.addItem(R.string.sensor, R.string.sensor_desc, FeatureActivity.class, Feature.FEATURE_SENSOR);
        holder.addItem(R.string.unclassified, R.string.unclassified_desc, FeatureActivity.class, Feature.FEATURE_UNCLASSIFIED);
>>>>>>> 5e12d0d119070e715d34f95477594d850204a3fb
    }

    @Override
    public int titleRes() {
        return R.string.app_name;
    }
}