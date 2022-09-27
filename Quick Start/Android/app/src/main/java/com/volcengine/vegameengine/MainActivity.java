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
        holder.addItem(R.string.audio_title, R.string.audio_desc, FeatureActivity.class, Feature.FEATURE_AUDIO);
        holder.addItem(R.string.camera_title, R.string.camera_desc, FeatureActivity.class, Feature.FEATURE_CAMERA);
        holder.addItem(R.string.clipboard_title, R.string.clipboard_desc, FeatureActivity.class, Feature.FEATURE_CLIPBOARD);
        holder.addItem(R.string.file_channel_title, R.string.file_channel_desc, FeatureActivity.class, Feature.FEATURE_FILE_CHANNEL);
        holder.addItem(R.string.location_title, R.string.location_desc, FeatureActivity.class, Feature.FEATURE_LOCATION);
        holder.addItem(R.string.message_channel_title, R.string.message_channel_desc, FeatureActivity.class, Feature.FEATURE_MESSAGE_CHANNEL);
        holder.addItem(R.string.pod_control_title, R.string.pod_control_desc, FeatureActivity.class, Feature.FEATURE_POD_CONTROL);
        holder.addItem(R.string.sensor_title, R.string.sensor_desc, FeatureActivity.class, Feature.FEATURE_SENSOR);
        holder.addItem(R.string.unclassified_title, R.string.unclassified_desc, FeatureActivity.class, Feature.FEATURE_UNCLASSIFIED);
    }

    @Override
    public int titleRes() {
        return R.string.app_name;
    }
}