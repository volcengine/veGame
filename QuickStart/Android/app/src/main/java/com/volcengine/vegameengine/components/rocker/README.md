#使用方式
1. 按照普通View，添加RockerView；
2. 在com.volcengine.cloudphone.apiservice.outinterface.IGamePlayerListener.onPlaySuccess回调用，设置KeyboardEventSender；
   参考代码：
   IODeviceManager ioDeviceManager = VeGameEngine.getInstance().getIODeviceManager();
   if(ioDeviceManager != null) {
       mRockView.setOnRockerChangeListener(new KeyboardEventSender(ioDeviceManager));
   }