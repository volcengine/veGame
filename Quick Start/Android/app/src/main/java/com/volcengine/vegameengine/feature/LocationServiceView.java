package com.volcengine.vegameengine.feature;

import android.Manifest;
import android.annotation.SuppressLint;
import android.content.Context;
import android.location.LocationManager;
import android.os.Looper;
import android.util.Log;
import android.view.View;
import android.widget.Button;
import android.widget.EditText;
import android.widget.RadioGroup;
import android.widget.ScrollView;
import android.widget.TextView;
import android.widget.Toast;

import androidx.appcompat.widget.SwitchCompat;

import com.blankj.utilcode.util.PermissionUtils;
import com.blankj.utilcode.util.ToastUtils;
import com.volcengine.cloudphone.apiservice.LocationService;
import com.volcengine.vegameengine.R;
import com.volcengine.vegameengine.util.DialogUtils;

import java.text.DateFormat;
import java.util.Date;
import java.util.Locale;

public class LocationServiceView {

    LocationService mLocationService;
    private DialogUtils.DialogWrapper mDialogWrapper;
    private static final String TAG = "LocationServiceView";

    public LocationServiceView(Context context, LocationService locationService, Button button) {
        this.mLocationService = locationService;
        mDialogWrapper = DialogUtils.wrapper(new TestView(context));
        button.setVisibility(View.VISIBLE);
        button.setOnClickListener(v -> mDialogWrapper.show());
    }

    private class TestView extends ScrollView implements LocationService.LocationEventListener {
        private final TextView mTvLocation;
        private final TextView tv_log;

        public TestView(Context context) {
            super(context);
            inflate(context, R.layout.dialog_location, this);
            tv_log = findViewById(R.id.tv_log);
            SwitchCompat switchCompat = findViewById(R.id.switch_enable);
            switchCompat.setChecked(mLocationService.isLocationServiceEnabled());
            switchCompat.setOnCheckedChangeListener((buttonView, isChecked) -> mLocationService.enableLocationService(isChecked));
            SwitchCompat switchListener = findViewById(R.id.switch_listener);
            mLocationService.setLocationEventListener(this);
            switchListener.setChecked(true);
            switchListener.setOnCheckedChangeListener((buttonView, isChecked) -> mLocationService.setLocationEventListener(isChecked ? this : null));
            mTvLocation = findViewById(R.id.tv_location);
            findViewById(R.id.btn_setRemoteLocation).setOnClickListener(v -> requestLocationAndSet(true));
            EditText et_lat = findViewById(R.id.et_lat);
            EditText et_long = findViewById(R.id.et_long);
            findViewById(R.id.btn_setRemoteLocation2).setOnClickListener(v -> {
                try {
                    double lng = Double.parseDouble(et_long.getText().toString());
                    double lat = Double.parseDouble(et_lat.getText().toString());
                    mLocationService.setRemoteLocationMock(new LocationService.LocationInfo(lat, lng));
                } catch (NumberFormatException e) {
                    ToastUtils.showShort("输入合法小数");
                }
            });
            RadioGroup rg_mode = findViewById(R.id.rg_mode);
            rg_mode.check(mLocationService.getLocationServiceMode() == LocationService.MODE_AUTO ? R.id.rb_auto : R.id.rb_manual);
            rg_mode.setOnCheckedChangeListener((group, checkedId) ->
                    mLocationService.setLocationServiceMode(checkedId == R.id.rb_auto ? LocationService.MODE_AUTO : LocationService.MODE_MANUAL));

        }

        @Override
        public void onReceivedRemoteLocationRequest(LocationService.RequestOptions options) {
            Log.d(TAG, "onReceivedRemoteLocationRequest: ");
            tv_log.append("==> onRemoteLocationRequest: " + options + "\n\n");
            requestLocationAndSet(false);
        }

        @Override
        public void onRemoteLocationRequestEnded() {
            Log.d(TAG, "onRemoteLocationRequestEnded: ");
            tv_log.append("==> onStopRemoteLocationRequest: " + DateFormat.getInstance().format(new Date()) + "\n\n");
        }

        @Override
        public void onSentLocalLocation(LocationService.LocationInfo location) {
            Log.d(TAG, "onSentLocalLocation: ");
            tv_log.append("==> onSentLocalLocation: " + location + "\n\n");
        }

        @Override
        public void onRemoteLocationUpdated(LocationService.LocationInfo location) {
            Log.d(TAG, "onRemoteLocationUpdated: ");
            tv_log.append("==> onRemoteLocationUpdated() called with: location = [" + location + "]" + "\n\n");
        }

        private void requestLocationAndSet(boolean manual) {
            PermissionUtils.permission(Manifest.permission.ACCESS_FINE_LOCATION)
                    .callback(new PermissionUtils.SimpleCallback() {
                        @SuppressLint("MissingPermission")
                        @Override
                        public void onGranted() {
                            LocationManager lm = (LocationManager) getContext().getSystemService(Context.LOCATION_SERVICE);
                            lm.requestSingleUpdate(LocationManager.NETWORK_PROVIDER, location -> {
                                if (manual) {
                                    mTvLocation.setText(String.format(
                                            Locale.getDefault(),
                                            "客户端的坐标是：\nlat[%f], long[%f]",
                                            location.getLatitude(),
                                            location.getLongitude()));
                                }
                                mLocationService.setRemoteLocationMock(new LocationService.LocationInfo(
                                        location.getLatitude(),
                                        location.getLongitude()
                                ));
                            }, Looper.getMainLooper());
                        }

                        @Override
                        public void onDenied() {
                            Toast.makeText(getContext(), "无定位权限", Toast.LENGTH_SHORT).show();
                        }
                    })
                    .request();
        }

    }

//    public LocationServiceView(Context context, LocationService locationService) {
//        super(context);
//        this.mLocationService = locationService;
//        inflate(context, R.layout.dialog_location, this);
//        tv_log = findViewById(R.id.tv_log);
//        SwitchCompat switchCompat = findViewById(R.id.switch_enable);
//        switchCompat.setChecked(mLocationService.isLocationServiceEnabled());
//        switchCompat.setOnCheckedChangeListener((buttonView, isChecked) -> mLocationService.enableLocationService(isChecked));
//        SwitchCompat switchListener = findViewById(R.id.switch_listener);
//        mLocationService.setLocationEventListener(this);
//        switchListener.setChecked(true);
//        switchListener.setOnCheckedChangeListener((buttonView, isChecked) -> mLocationService.setLocationEventListener(isChecked ? this : null));
//        mTvLocation = findViewById(R.id.tv_location);
//        findViewById(R.id.btn_setRemoteLocation).setOnClickListener(v -> requestLocationAndSet(true));
//        EditText et_lat = findViewById(R.id.et_lat);
//        EditText et_long = findViewById(R.id.et_long);
//        findViewById(R.id.btn_setRemoteLocation2).setOnClickListener(v -> {
//            try {
//                double lng = Double.parseDouble(et_long.getText().toString());
//                double lat = Double.parseDouble(et_lat.getText().toString());
//                mLocationService.setRemoteLocationMock(new LocationService.LocationInfo(lat, lng));
//            } catch (NumberFormatException e) {
//                ToastUtils.showShort("输入合法小数");
//            }
//        });
//        RadioGroup rg_mode = findViewById(R.id.rg_mode);
//        rg_mode.check(mLocationService.getLocationServiceMode() == LocationService.MODE_AUTO ? R.id.rb_auto : R.id.rb_manual);
//        rg_mode.setOnCheckedChangeListener((group, checkedId) ->
//                mLocationService.setLocationServiceMode(checkedId == R.id.rb_auto ? LocationService.MODE_AUTO : LocationService.MODE_MANUAL));
//
//    }

//    @Override
//    public void onReceivedRemoteLocationRequest(LocationService.RequestOptions options) {
//        Log.d(TAG, "onReceivedRemoteLocationRequest: ");
//        tv_log.append("==> onRemoteLocationRequest: " + options + "\n\n");
//        requestLocationAndSet(false);
//    }
//
//    @Override
//    public void onRemoteLocationRequestEnded() {
//        Log.d(TAG, "onRemoteLocationRequestEnded: ");
//        tv_log.append("==> onStopRemoteLocationRequest: " + DateFormat.getInstance().format(new Date()) + "\n\n");
//    }
//
//    @Override
//    public void onSentLocalLocation(LocationService.LocationInfo location) {
//        Log.d(TAG, "onSentLocalLocation: ");
//        tv_log.append("==> onSentLocalLocation: " + location + "\n\n");
//    }
//
//    @Override
//    public void onRemoteLocationUpdated(LocationService.LocationInfo location) {
//        Log.d(TAG, "onRemoteLocationUpdated: ");
//        tv_log.append("==> onRemoteLocationUpdated() called with: location = [" + location + "]" + "\n\n");
//    }
//
//    private void requestLocationAndSet(boolean manual) {
//        PermissionUtils.permission(Manifest.permission.ACCESS_FINE_LOCATION)
//                .callback(new PermissionUtils.SimpleCallback() {
//                    @SuppressLint("MissingPermission")
//                    @Override
//                    public void onGranted() {
//                        LocationManager lm = (LocationManager) getContext().getSystemService(Context.LOCATION_SERVICE);
//                        lm.requestSingleUpdate(LocationManager.NETWORK_PROVIDER, location -> {
//                            if (manual) {
//                                mTvLocation.setText(String.format(
//                                        Locale.getDefault(),
//                                        "客户端的坐标是：\n纬度[%f], 经度[%f]",
//                                        location.getLatitude(),
//                                        location.getLongitude()));
//                            }
//                            mLocationService.setRemoteLocationMock(new LocationService.LocationInfo(
//                                    location.getLatitude(),
//                                    location.getLongitude()
//                            ));
//                        }, Looper.getMainLooper());
//                    }
//
//                    @Override
//                    public void onDenied() {
//                        Toast.makeText(getContext(), "无定位权限", Toast.LENGTH_SHORT).show();
//                    }
//                })
//                .request();
//    }

}
