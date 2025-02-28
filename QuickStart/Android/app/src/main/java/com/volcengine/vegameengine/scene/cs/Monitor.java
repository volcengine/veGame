package com.volcengine.vegameengine.scene.cs;

import android.util.Log;

public class Monitor {
    static int sendEventsNum = 0;
    static long startTime = 0;

    static MonitorListener listener;

    static void monitor(){
        sendEventsNum++;
        long currentTime = System.currentTimeMillis();
        if(startTime == 0) {
            startTime = currentTime;
        } else if(currentTime - startTime > 2 * 1000){
            float num = ((float) sendEventsNum * 1000.0f) / (float)(currentTime - startTime);
            if(listener != null){
                listener.onMonitor(num);
            }
            Log.e("zhanghang", "the num of events pre second is " + num);
            startTime = currentTime;
            sendEventsNum = 0;
        }
    }

    public interface MonitorListener {
        void onMonitor(float num);
    }
}
