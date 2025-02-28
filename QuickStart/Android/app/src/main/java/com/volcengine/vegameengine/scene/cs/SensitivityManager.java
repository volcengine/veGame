package com.volcengine.vegameengine.scene.cs;

public class SensitivityManager {

    // 映射成鼠标move事件的敏感度因子
    private static int sensitivityFactor = 50;

    public static void setSensitivity(int sensitivity){
        if(sensitivity < 10){
            sensitivity = 10;
        }
        if(sensitivity > 100){
            sensitivity  = 100;
        }
        if(sensitivity == sensitivityFactor){
            return;
        }

        sensitivityFactor = sensitivity;

    }

    public static int getCurrentSensitivity(){
        return sensitivityFactor;
    }
}
