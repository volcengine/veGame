# 云游戏客户端 Web SDK demo

这个开源项目展示了火山引擎云游戏客户端 Web SDK 的部分功能，具体可参考每个子目录下的 `README` 文件查看具体的内容。

## 目录结构

```bash
├── H5_Pc # h5玩端游的基本配置
├── H5_Phone # h5玩手游的基本配置
├── PcWeb_Pc # PC玩端游的基本配置
├── PcWeb_Phone # PC玩手游的基本配置
├── CommonFeatures # 各个端公共的SDK能力展示
```

`H5_Pc`,`H5_Phone`,`PcWeb_Pc`,`PcWeb_Phone`这四个目录的区别在于初始化 SDK 时 `isPC`和`mode`配置不同，`isPC`表示是否为 PC Web 环境下使用，`mode`表示游戏类型和模式，具体可见 [SDK 初始化配置](https://www.volcengine.com/docs/6512/75597#%E5%AE%9E%E4%BE%8B%E5%8C%96-vegamesdk)。

`CommonFeatures`目录中包含了几个端公用的一些能力，比如旋转、切换清晰度等。可通过修改`/CommonFeatures/config.js`中的`isPC`和`mode`切换到不同的场景。
