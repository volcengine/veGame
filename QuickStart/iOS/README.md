# veGame SDK Demo

## 说明

本项目是火山引擎云游戏 iOS 客户端 SDK 的快速演示 Demo。获取项目以后，开发者可以快速构建应用，体验云游戏服务的主要功能；也能参考其中的代码，在实际的客户端应用中实现相似的功能。

## 环境要求
1. iOS 11.0 及以上系统版本的设备
2. 使用 Objective-C 开发语言
3. VeGame.framework 为 Dynamic Library，且只支持真机运行，不支持模拟器

说明：本文档中涉及编译器的指引及示例图参考 Xcode 13.4 版本。



# 快速开始

## 使用 CocoaPods 安装

1. 在工程的 `Podfile` 中添加 CocoaPods 的 source 源，并依赖 `VeGameSDK`

   ```ruby
   source 'https://github.com/volcengine/volcengine-specs.git'
   
   platform :ios, '11.0'
   inhibit_all_warnings!
   
   target 'TargetName' do
     pod 'VeGameSDK', '1.35.0'
   end
   ```

## 接入的流程

1. 建议在 AppDelegate 的 didFinishLaunchingWithOptions 中初始化 VeGameSDK 配置信息。

   ```objective-c
   [[VeGameManager sharedInstance] initWithAccountId:@"------AccountID------"];。
   ```

2. 在 VeGameViewController 中填写游戏的 ak、sk、token 等鉴权信息和 userId、gameId、roundId 等配置信息。

   ```objective-c
   self.akTextField.text = @"------ak------";
   self.skTextField.text = @"------sk------";
   self.tokenTextField.text = @"------token------";
   self.userIdTextField.text = @"------userId------";
   self.gameIdTextField.text = @"------gameId------";
   self.roundIdTextField.text = @"------roundId------";
   self.rotationTextField.text = @"270"; //竖屏0，横屏270
   ```

3. 初始化成功后，调用 `- (void)startWithConfig:(VeGameConfigObject *)configObj` 接口启动游戏。

   ```objective-c
   VeGameConfigObject *configObj = [VeGameConfigObject new];
   configObj.ak = self.configObj.ak;
   configObj.sk = self.configObj.sk;
   configObj.token = self.configObj.token;
   configObj.userId = self.configObj.userId;
   configObj.gameId = self.configObj.gameId;
   configObj.roundId = self.configObj.roundId;
   VeGameControlObject *control = [VeGameControlObject new];
   control.role = self.configObj.role;
   control.roomType = self.configObj.roomType;
   configObj.control = control;
   configObj.sessionMode = self.configObj.sessionMode;
   configObj.queuePriority = self.configObj.queuePriority;
   configObj.keyboardEnable = self.configObj.keyboardEnable;
   configObj.autoRecycleTime = self.configObj.autoRecycleTime;
   configObj.videoStreamProfileId = self.configObj.videoStreamProfileId;
   configObj.reservedId = self.configObj.reservedId.length ? self.configObj.reservedId : nil;
   [[VeGameManager sharedInstance] startWithConfig: configObj];
   ```
4. 实现相关回调。

   ```objective-c
   #pragma mark - VeGameManagerDelegate
   - (void)gameManager:(VeGameManager *)manager startSucceedResult:(NSString *)gameId videoStreamProfileId:(NSInteger)streamProfileId reservedId:(NSString *)reservedId planId:(NSString *)planId extra:(NSDictionary *)extra
   {
       // 启动成功，收到首帧画面回调
   }
   
   - (void)gameManager:(VeGameManager *)manager changedDeviceRotation:(NSInteger)rotation
   {
       // 横竖屏方向回调，注意：VeGameSDK只负责游戏横竖屏方向回调，不负责横竖屏的旋转，接入方根据rotation自行处理
   }
   
   - (void)gameManager:(VeGameManager *)manager onWarning:(VeGameWarningCode)warnCode
   {
       // 警告回调
   }
   
   - (void)gameManager:(VeGameManager *)manager onError:(VeGameErrorCode)errCode
   {
       // 错误回调
   }
   ```
5. 结束时，调用 `- (void)stop` 接口结束游戏。

   ```objective-c
   [[VeGameManager sharedInstance] stop];
   ```

## 参考资料

客户端 SDK 下载：https://www.volcengine.com/docs/6512/75594

注：如果不能访问以上链接，请参考 [开通云游戏服务](https://www.volcengine.com/docs/6512/75577) 说明文档。
