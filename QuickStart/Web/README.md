# veGame Web SDK 快速开始指南

## 概述

这里是一些供参考的云游戏 Web SDK 的使用示例，用户可直接查看不同场景下 SDK 的用法，并且在每个 sample 中 通过 README 提示操作快速运行示例

## 目录结构

```plaintext
Web_v2/
├── h5-pc/                         # H5 端玩端游场景
├── h5-phone/                      # H5 端玩手游场景
├── pc-web-pc/                     # PC 端玩端游场景
├── pc-web-phone/                  # PC 端玩手游场景
├── h5-phone-gamePad/              # H5 端玩手游场景：虚拟手柄配置
├── h5-phone-localKeyboard/        # H5 端玩手游场景：本地键盘
├── h5-phone-touchToMouse/         # H5 端玩手游场景：触控转鼠标
├── pc-web-pc-fileChannel/         # PC 端玩端游场景：文件通道
├── pc-web-pc-messageChannel/      # PC 端玩端游场景：消息通道
├── pc-web-pc-queue/               # PC 端玩端游场景：排队
├── pc-web-phone-autoRecycleTime/  # PC 端玩手游场景：设置自动回收时长
├── pc-web-phone-background/       # PC 端玩手游场景：客户端前后台切换
├── pc-web-phone-extraStartConfig/ # PC 端玩手游场景：其他参数配置: Pod 内跳转到第三方浏览器、Pod 内游戏产生日志上传、云游戏改机参数、指定运行游戏需要的伴随程序列表
├── pc-web-phone-idleTime/         # PC 端玩手游场景：设置空闲时间
├── pc-web-phone-reconnectTime/    # PC 端玩手游场景：设置重连时间
├── pc-web-phone-role/             # PC 端玩手游场景：设置游戏玩家角色
├── pc-web-phone-rotation/         # PC 端玩手游场景：设置游戏旋转角度
├── pc-web-phone-sessionMode/      # PC 端玩手游场景：设置或切换游戏的挂机模式
├── pc-web-phone-setIMECompositionVisible/ # PC 端玩手游场景：本地输入法显示中文合成过程
├── pc-web-phone-updateVideoScale/ # PC 端玩手游场景：更新画面放缩比
├── pc-web-phone-userFilePath/     # PC 端玩手游场景：设置/获取保存用户配置文件的路径
├── pc-web-phone-videoStreamProfile/ # PC 端玩手游场景：设置视频流分辨率
├── framework-integration/         # 框架集成说明
│   ├── react.md
│   └── vue.md
└── README.md
```

云游戏 Web SDK 根据 PC 端/H5 端 以及 端游/手游 场景进行了拆分，即有 4 个场景（`isPC`和`mode`是 sdk 的初始化参数）：

```
| 实际场景       | isPC（是否是PC端） | mode（手游/端游）   |
| pc玩端游       | true             | MODE.CLOUD_PC        |
| pc玩手游       | true             | MODE.CLOUD_PHONE_GAME|
| h5玩端游       | false            | MODE.CLOUD_PC        |
| h5玩手游       | false            | MODE.CLOUD_PHONE_GAME|
```

以上目录中只给了部分用法示例，比如`设置游戏旋转角度`，`设置自动回收时长`等功能，不仅适用于`PC 端玩手游场景`, 同样适用于`PC 端玩端游场景`、`H5 端玩手游场景`、`H5 端玩端游场景`等场景，而`虚拟手柄配置`只支持 H5 端使用。具体可支持场景以及切换设置可参考每个 sample 的 README 进行调整。
