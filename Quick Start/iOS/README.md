# veGame SDK Demo

## 说明

本项目是火山引擎云游戏 iOS 客户端 SDK 的快速演示 Demo。获取项目以后，开发者可以快速构建应用，体验云游戏服务的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。

## 环境要求
1. iOS 9.0 及以上系统版本的设备
2. 使用 Objective-C 开发语言
3. VeGame.framework 为 Dynamic Library，且只支持真机运行，不支持模拟器

说明：本文档中涉及编译器的指引及示例图参考 Xcode 13.4 版本。

## 快速开始

1. 克隆或下载 Demo 工程源文件到本地。
2. 执行 pod install 指令，成功之后，打开 VeGamePublicDemo.xcworkspace。
3. 相关的运行信息，在打印的 log 中查看。

## 接入的流程

1. 建议在 AppDelegate 的 didFinishLaunchingWithOptions 中初始化 VeGameSDK 配置信息，[VeGameManager setConfigInfo]。
1. 在 VeGameViewController 中填写游戏的 ak、sk、token 等鉴权信息和 userId、gameId、roundId 等配置信息，并初始化 SDK 实例（`veGameEngine`）。
2. 初始化成功后，调用 `startWithConfig` 接口启动游戏。
3. 结束时，调用 `stop()` 接口结束游戏。

## 参考资料

客户端 SDK 下载：https://www.volcengine.com/docs/6512/75594

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
