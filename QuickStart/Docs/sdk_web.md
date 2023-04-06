# 火山引擎云游戏 Web/H5 客户端 SDK 接入说明  

本文档描述了火山引擎云游戏 Web/H5 客户端 SDK 的使用说明，方便开发者快速接入。

## 集成准备

### SDK 下载

|  **SDK 包文件**  |  **快速接入 Demo（GitHub 仓库）**  |
| --- | --- |
| [前往云游戏控制台](https://console.volcengine.com/veGame/guidepage?activeStep=sdk) | [Quick Start Demo](https://github.com/volcengine/veGame/tree/master/QuickStart/Web) |

### 环境要求

* iOS 11.0 及以上系统版本
* Android 浏览器：需要支持 WebRTC 的浏览器内核
* PC Web 浏览器：内核为 Chrome 58+ 的浏览器，以及 Safari 11 及以上版本

说明：本文提供的示例代码均为 JavaScript 语言。

### 快速接入

下载 SDK 包文件后，解压后引入即可：

```
import veGameSDK from 'veGameSDK.js'
```

## 接口说明

### 实例化 veGameSDK

描述：实例化方法，通过 `new veGameSDK(config)` 调用

入参：`config`，格式为 JSON

返回值：veGame 实例

`config` 字段描述如下：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| userId | String | null | 是 | 自定义客户端用户 ID，用于区分用户 |
| accountId | String | null | 是 | 火山引擎用户账号，可通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取 |
| domId | String | null | 是 | 运行云游戏的 DOM ID ，SDK 将在该 DOM 新建画布以及渲染画面。注意：需要将该 DOM ID 设置为游戏的有效画面区域；如需调整画面放缩比，可调用 [updateVideoScale]() 接口。 |
| isPC | Boolean | false | 否 | 是否为 PC Web 环境下使用。默认非 PC Web 环境下将会监听 Touch 事件，而 PC Web 环境下将监听鼠标以及键盘事件 |
| mode | Number | null | 否 | 游戏类型和模式，可选：  <br>MODE.CLOUD_PHONE_GAME（云游戏手游，默认）  <br>MODE.CLOUD_PC（云游戏端游）详细信息，参考 [常量说明](#常量)。 |
| isDebug | Boolean | false | 否 | 是否开启调试日志 |
| enableSyncClipBoard | Boolean | false | 否 | 是否开启剪切板同步 |
| enableLocalKeyboard | Boolean | false | 否 | 是否开启本地键盘输入 |
| needConnectFileChannel | Boolean | false | 否 | 是否开启文件传输通道 |
| disableDefaultTouchEvent | Boolean | false | 否 | 是否禁止 SDK 将触控事件发送到云端游戏 |

参考以下示例：

```JavaScript
let config = {"userId":"your_user_id",...};
let veGame = new veGameSDK(config);
```

### 获取设备唯一标识

描述：在完成 SDK 初始化后，获取用户终端设备的唯一标识。

> 最佳实践：保存获取到的设备唯一标识到业务的服务端。在通过调用服务端 PreAllocateResource 接口锁定资源、请求游戏服务时，透传保存的设备唯一标识，用于确定是否支持游戏多开。

返回值：获取到的设备唯一标识

参考以下示例：

```JavaScript
veGame.getDeviceId();
```

### 启动

描述：启动云游戏，需要在初始化成功之后调用。请确认游戏 ID 等信息正确，并确保已经在云游戏控制台完成实例订购和相应的游戏上架。

入参：`options`

返回值：Promise，分别返回开始「成功/失败」的状态

`options` 的有效字段描述如下：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| gameId | String | null | 是 | 游戏 ID，可通过火山引擎云游戏控制台『游戏管理』页面获取，例如：1428112352161312345，当传入 customGameId 时，可不传入（以 gameId 优先） |
| customGameId | String | null | 是 | 注册游戏时指定的用户自定义游戏 ID，当传入 gameId 时，可不传入 |
| token | Object | null | 是 | 启动云游戏的令牌，为一个 JSON 对象，包含以下属性（通过调用服务端 STSToken 接口获取）：  <br>AccessKeyID：用于鉴权的临时 Access Key  <br>SecretAccessKey：用于鉴权的临时 Secret Key  <br>SessionToken：用于鉴权的临时 Token  <br>CurrentTime：Token 创建时间  <br>ExpiredTime：Token 过期时间  <br>有关服务端 STSToken 接口的详细信息，参考 [签发临时 Token]() |
| roundId | String | null | 是 | 当次游戏生命周期的标识符，命名规则如下：  <br>最大长度为128位的非空字符串，支持的字符集范围为:  <br>大写字母 A ~ Z  <br>小写字母 a ~ z  <br>数字 0 ~ 9  <br>下划线、减号 |
| podSettings | Object | null | 否 | 用于控制是否开启多人游戏及游戏控制权转移（不传该参数则表示游戏为普通云游戏），为一个 JSON 对象，包含以下属性：  <br>type：游戏类型，可选项：0：单人控制单人观看，默认、1：单人控制多人观看，不可转移游戏控制权、2：单人控制多人观看，可转移游戏控制权  <br>role：当前用户进入游戏的默认角色，可选项：0：观看者，默认、1：操作者  <br>说明：指定该参数后，可在游戏中调用 [changeRole]() 方法将观看者设置为操作者，转移游戏控制权。进入游戏的观看者也会占用实例资源，建议控制观看者数量。如有大量观看者，可使用 [StartLive]() OpenAPI 接口对游戏进行直播。 |
| mute | Boolean | false | 否 | 是否静音模式，在非交互式的场景下启动云游戏，需要设置为 true（部分浏览器为了防止网页在用户非自愿的情况下自动播放声音，对网页上的自动播放 Autoplay 功能做了限制：在没有用户交互操作之前不允许有声音的媒体播放，具体请参考各浏览器的自动播放策略） |
| audioAutoPlay | Boolean | false | 否 | 是否在用户首次点击操作时，自动播放音频（适用于自动播放音频失败的场景，有关自动播放策略处理的最佳实践，参考 [云游戏自动播放策略处理最佳实践]()） |
| videoStreamProfileId | Number | 1 | 否 | 游戏视频流清晰度 ID；如不传入，则使用默认清晰度（清晰度档位 ID 列表和详细说明，参考 [清晰度切换](#清晰度切换)） |
| rotation | String | 游戏默认 | 否 | 游戏的横竖屏（如不指定，则使用游戏默认显示方式，可在上架游戏时设置）：  <br>landscape：横屏显示   <br>portrait：竖屏显示 |
| isScreenLock | Boolean | false | 否 | 是否锁定屏幕横竖屏显示（仅支持移动端）：  <br>true：锁定屏幕开启  <br>false：锁定屏幕关闭 |
| userTag | String | null | 否 | 用户标签，由业务方指定，可通过调用服务端 CreateUserTag 接口创建，参考 [创建用户属性]() |
| planId | String | null | 否 | 火山侧套餐 ID，可通过调用服务端 [ListResourceSet]() 接口获取（configuration_code 字段） |
| reservedId | String | null | 否 | 资源预锁定 ID，通过调用服务端 [PreAllocateResource]() 接口获取（如不传入，将在启动成功时返回）  <br>资源预锁定 ID 用于申请火山引擎云游戏的每次服务，建议业务记录此 ID，用于每次服务会话的唯一标识，可用于调试和问题排查。 |
| autoRecycleTime | Integer | null | 否 | 设置无操作自动回收服务时长，单位秒（如不设置或设置为0，则使用默认时长300秒，支持设置的上限值为7200，即2小时） |
| userProfilePathList | String[] | null | 否 | 保存用户游戏配置文件的路径列表 |
| enableFullScreen | Boolean | null | 否 | 是否开启设备全屏显示 |
| videoRenderMode（仅支持云游戏端游） | String | VIDEO_RENDER_MODE.CONTAIN | 否 | 指定视频流渲染模式：  <br>VIDEO_RENDER_MODE.CONTAIN：默认  <br>VIDEO_RENDER_MODE.COVER：根据视频画面分辨率长边做裁剪 |
| systemCursorShowStatManual | Boolean | null | 否 | 是否手动控制光标显示/隐藏：  <br>true：手动控制光标显示/隐藏  <br>false：由 SDK 自动控制光标显示/隐藏（默认） |
| customLocalKeyboardInputId | String | null | 否 | 自定义 Input 元素的 DOM ID（在使用本地输入法场景下，用于对输入框的 UI 和位置进行自定义）；详细使用方法，参考 [Web SDK iOS 本地输入法方案]() |
| extra | Map<String,String> | null | 否 | 根据业务需要，自定义的扩展参数；详细信息，参考 [自定义扩展参数列表]() |

Promise 「成功」的返回值如下：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| width | Number | 云端实例的宽度 |
| height | Number | 云端实例的高度 |
| roundId | String | 当次游戏生命周期的唯一标识 |
| gameId | String | 游戏 ID |
| reservedId | String | 资源预锁定 ID |
| extra | Map | 根据业务需要，自定义的扩展参数 |

参考以下示例：

```JavaScript
veGame.start(options).then((gameStream) => {
  console.log(gameStream.width);
  console.log(gameStream.height);
  console.log(gameStream.roundId);
  console.log(gameStream.reservedId);
})
```
#### 错误码

启动游戏可能返回的错误码和相关说明如下：

|  **错误码**  |  **错误信息**  |  **说明**  |
| --- | --- | --- |
| 10000 | ERROR_START_GENERAL | 开始游戏失败。原因：通用错误。建议：请检查开始游戏 start() 接口参数。 |
| 10001 | ERROR_START_AUTHENTICATION_FAILED | 开始游戏失败。原因：火山引擎服务鉴权失败。建议：检查您的 AK、SK、Token 生成，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588)。 |
| 10002 | ERROR_START_GAME_ID_NOT_EXIST | 开始游戏失败。原因：当前游戏 ID 或 自定义游戏 ID 不存在。建议：请通过火山引擎云游戏控制台『游戏管理』页面获取正确的游戏 ID。 |
| 10003 | ERROR_START_GAME_ID_NOT_READY | 开始游戏失败。原因：当前游戏尚在适配中。建议：游戏适配需要一定时间，如需加急，可联系火山引擎云游戏服务对接人员。 |
| 10004 | ERROR_START_CONFIGURATION_CODE_NOT_EXIST | 开始游戏失败。原因：资源套餐 ID 不存在。建议：可通过调用服务端 [ListResourceSet](https://www.volcengine.com/docs/6512/102116) 接口获取（configuration_code 字段）获取正确的套餐信息。 |
| 10005 | ERROR_START_CONFIGURATION_CODE_NOT_REDAY | 开始游戏失败。原因：游戏尚未配置资源套餐或套餐 ID 无效。建议：请通过火山引擎云游戏控制台『游戏管理』页面为游戏配置部署策略。 |
| 10006 | ERROR_START_RESOURCE_NOT_READY | 开始游戏失败。原因：当前业务没有订购资源。建议：请通过火山引擎云游戏控制台『资源管理』页面购买资源。 |
| 10007 | ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH | 开始游戏失败。原因：当前游戏资源用量超限或无可用资源。建议：请通过火山引擎云游戏控制台『资源管理』页面购买资源。 |
| 10008 | ERROR_START_STREAM_AUTO_PLAY_FAILED | 开始游戏失败。原因：游戏串流自动播放失败。建议：使用 [playByInteraction()](#交互播放) 方法进行处理。  |
| 10009 | ERROR_START_AUTHENTICATION_KEY_FAILED | 开始游戏失败。原因：火山引擎服务鉴权失败。建议：请检查临时鉴权 Token 的有效期，参考 [生成临时 Token 接口](https://www.volcengine.com/docs/6512/75588)。 |
| 10011 | ERROR_START_CONNECTION_ENDED | 开始游戏失败。原因：在调用 start() 接口启动游戏、启动成功回调触发之前，游戏被停止（可能原因包括：服务端游戏崩溃导致开始游戏失败、调用了服务端 [GameStop](https://www.volcengine.com/docs/6512/102175) 接口停止游戏服务）。 |
| 10012 | ERROR_START_RESERVED_ID_NOT_FOUND | 开始游戏失败。原因：资源预锁定 ID（reservedId）不存在。建议：请确保指定的 reservedId 正确；可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口获取。 |
| 10013 | ERROR_START_RESERVED_ID_EXPIRED | 开始游戏失败。原因：资源预锁定 ID（reservedId）已失效。建议：可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 10014 | ERROR_START_RESERVED_ID_ALREADY_USED_RELEASE | 开始游戏失败。原因：资源预锁定 ID（reservedId）已被使用且已释放。建议：可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 10015 | ERROR_START_RESERVED_ID_USING | 开始游戏失败。原因：资源预锁定 ID（reservedId）正在被使用中。建议：可通过调用服务端 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口重新获取。 |
| 10016 | ERROR_START_RESERVED_ID_MISMATCH_PREPARE | 开始游戏失败。原因：资源预锁定 ID（reservedId）与调用 [PreAllocateResource](https://www.volcengine.com/docs/6512/107714) 接口时指定的参数不一致。建议：请检查资源预锁定 ID（reservedId）与调用 PreAllocateResource 接口时指定的参数是否一致。 |
| 10017 | ERROR_START_NO_SUFFICIENT_FUND | 开始游戏失败。原因：启动游戏时，后付费账户欠费，导致服务不可用。建议：请通过火山引擎『费用中心』充值付费，充值到账后服务自动开启。 |
| 10018 | ERROR_START_USER_CONFLICT | 开始游戏失败。原因：用户使用多个设备申请游戏服务时，触发了游戏多开限制。建议：需要客户端提示用户不要进行多开操作。 |
| 10026 | ERROR_START_MISMATCH_ACCOUNTID | 开始游戏失败。原因：指定的火山引擎账号校验失败。建议：通过火山引擎官网页面右上角 **用户 > 账号管理 > 主账号信息** 获取正确的账号。 |
| 10027 | ERROR_START_INVALID_LOCAL_TIME | 开始游戏失败。原因：用户手机时间和服务端时间相差超过7天，导致鉴权 Token 过期。建议：需要客户端提示用户把手机时间修正为标准时间。 |

### 停止
描述：停止从云端拉流，并且退出，但不会回收客户端本地资源。

返回值：Promise，分别返回结束游戏「成功/失败」的状态

```JavaScript
veGame.stop();
```

### 重启

描述：重启当前游戏。

返回值：Promise，分别返回重启游戏「成功/失败」的状态

```JavaScript
veGame.restart();
```

### 暂停

描述：暂停拉取音视频流

返回值：void

```JavaScript
veGame.pause();
```

### 恢复

描述：恢复已经暂停的音视频流。

返回值：void

```JavaScript
veGame.resume();
```

### 交互播放

描述：用于自动播放失败的情况下，提示和引导用户与页面进行交互来恢复播放。

返回值：promise

参考以下示例（当检测到自动播放失败的报错后，在页面上动态显示一个按钮让用户点击）：

```JavaScript
veGame.start(options).then(() => {
  console.log('start successfully')
}).catch((e) => {
  if (e.errorCode === 10008) {
     // 弹出引导用户点击继续播放的 UI 界面提示
     showBtnUI();
  }
})；

btn.onclick = () => {
  veGame.playByInteraction();
};
```

### 清晰度切换

描述：在游戏运行过程中，通过指定清晰度档位 ID，切换视频流的清晰度。

返回值：promise，分别返回切换清晰度「成功/失败」的状态

```JavaScript
veGame.switchVideoStreamProfile(streamProfileId);
```

有关 `streamProfileId` 的可选值和详细的清晰度配置，参考以下 **清晰度档位说明**。

#### 清晰度档位说明

|  **档位 ID**  |  **宽**  |  **高**  |  **码率（kbps）**  |  **帧率**  |
| --- | --- | --- | --- | --- |
| 1 | 720 | 1280 | 4000 | 30 |
| 2（流畅） | 720 | 1280 | 4000 | 60 |
| 3 | 720 | 1280 | 5000 | 30 |
| 4 | 720 | 1280 | 5000 | 60 |
| 5 | 720 | 1280 | 6000 | 30 |
| 6（标清，默认档位） | 720 | 1280 | 6000 | 60 |
| 7 | 720 | 1280 | 8000 | 30 |
| 8 | 720 | 1280 | 8000 | 60 |
| 9 | 1080 | 1920 | 4000 | 30 |
| 10 | 1080 | 1920 | 4000 | 60 |
| 11 | 1080 | 1920 | 5000 | 30 |
| 12 | 1080 | 1920 | 5000 | 60 |
| 13 | 1080 | 1920 | 6000 | 30 |
| 14（高清） | 1080 | 1920 | 6000 | 60 |
| 15 | 1080 | 1920 | 8000 | 30 |
| 16（超清） | 1080 | 1920 | 8000 | 60 |
| 17 | 540 | 960 | 4000 | 30 |
| 18 | 450 | 800 | 4000 | 30 |
| 19 | 360 | 640 | 3000 | 30 |
| 20 | 1080 | 1920 | 10000 | 30 |
| 21 | 1080 | 1920 | 10000 | 60 |
| 22 | 1440 | 2560 | 12000 | 30 |
| 23 | 1440 | 2560 | 12000 | 60 |

### 销毁实例

描述：销毁客户端实例，释放资源。

返回值：void

```JavaScript
veGame.destroy();
```

### 插件机制

veGame SDK 从 V1.19.0 开始引入插件机制，开发者可按需引入减少主包体积。可以通过 `use` 函数引入插件，参考以下示例：

```JavaScript
import Plugin from './plugin.min.js';
veGameSDK.use(Plugin);
```

#### Socket 插件使用

云游戏默认使用 RTC 接入音视频流，如果需要考虑兼容性场景，可以手动引入 Socket 插件包来进行软解兜底。引入 `veSocket` 插件之后就可以使用 `mediaProvider` 参数指定使用 Socket 软解方案。

```JavaScript
import veSocket from './veSocket.min.js';
veGameSDK.use(veSocket);
```

### 发送自定义消息

描述：客户端发送消息到云端实例。

返回值：Promise，分别返回发送「成功/失败」的状态

入参：`options`

`options` 的有效字段描述如下：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| message | String/ArrayBuffer | null | 是 | 消息内容（仅端游支持 ArrayBuffer 数据类型发送） |
| needAck | Boolean | null | 否 | （仅手游支持该参数）发送的消息否需要应答 |
| timeout | Number | null | 否 | （仅手游支持该参数）应答消息的超时时间，单位 ms |
| destUserId | String | null | 否 | （仅手游支持该参数）云端游戏初始化 veMessageChannelClient 时填入的目标用户 ID（如果 Message Channel 连接通道超过1条，必须指定 destUserId）  <br>有关 Message Channel SDK 的使用方法，参考 [Message Channel SDK 接入说明]()。 |

参考示例：

```JavaScript
veGame.sendMessage({
    message: 'message'
})；

veGame.sendMessage({
    message: 'message',
    needAck: true,
    timeout: 5 * 1000,
    destUserId: 'appname'
})；
```

### 发送剪贴板消息

描述：发送客户端本地剪贴板消息到云端。

返回值：Promise，分别返回发送「成功/失败」的状态

入参：任意 String

参考示例：

```JavaScript
veGame.sendClipBoardMessage('message')
```

### 发送触控事件

描述：发送触控事件。

返回值：Promise，分别返回发送「成功/失败」的状态

入参：格式是JSON

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| pointerId | Number | null | 是 | 触控 ID，用于标识单个手指 |
| action | Number | null | 是 | 触控动作，可选：  <br>ACTION.TOUCH_START  <br>ACTION.TOUCH_END  <>ACTION.TOUCH_MOVE  <br>详细信息，见 [常量部分]()） |
| x | Number | null | 是 | 取值范围 [0,1] 。游戏画面的 x 轴相对坐标，可根据 DOM 的宽高比以及 start 接口返回的云端实例宽高比进行计算 |
| y | Number | null | 是 | 取值范围 [0,1] 。游戏画面的 y 轴相对坐标，可根据 DOM 的宽高比以及 start 接口返回的云端实例宽高比进行计算 |

参考以下示例：

```JavaScript
import { ACTION } from 'veGameSDK.js';

veGame.sendTouchMessage({
  pointerId: 0,
  action: ACTION.TOUCH_START,
  x: 0.25,
  y: 0.25
})
```

### 发送按键事件
描述：发送按键事件。

返回值：Promise，分别返回发送「成功/失败」的状态

入参：格式是JSON

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| keycode | Number | null | 是 | 键码，可选 KEYCODE 枚举类型（见常量部分） |
| action | Number | null | 是 | 按键动作，可选 ACTION.DOWN、ACTION.UP（见常量部分） |

参考以下示例：

```JavaScript
import { KEYCODE, ACTION } from 'veGameSDK.js';

veGame.sendKeycodeMessage({
  keycode: KEYCODE.KEYCODE_BACK,
  action: ACTION.DOWN
})
```

### 发送鼠标事件

返回值：Promise，分别返回发送「成功/失败」的状态

入参：格式是 JSON

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| action | Number | null | 是 | 鼠标动作，可选 ACTION.DOWN、ACTION.UP、ACTION.MOVE、ACTION.WHEEL（见常量部分） |
| button | Number | null | 否，action 为 DOWN、UP、MOVE 时有效 | 鼠标按键标示，MOUSE_BUTTON.LEFT、MOUSE_BUTTON.RIGHT、MOUSE_BUTTON.CENTER |
| wheel | Number | null | 否，action 为 WHEEL 时有效 | 滚轮值 |
| x | Number | null | 否，action 为 MOVE 时有效 | 相对有效画面的位置 x 轴  0.00000 - 1.00000 |
| y | Number | null | 否，action 为 MOVE 时有效 | 相对有效画面的位置 y 轴  0.00000 - 1.00000 |

参考以下示例：

```JavaScript
veGame.sendMouseMessage({
  action: ACTION.DOWN,
  button: MOUSE_BUTTON.LEFT,
  x: 0.2,
  y: 0.3
})
```

### 透传加速度传感器数据

描述：发送加速度传感器数据。

返回值：Promise，分别返回发送「成功/失败」的状态

入参：格式是 JSON

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| x | Number | null | 是 | 表示 x 轴上的（西到东）加速度 |
| y | Number | null | 是 | 表示 y 轴上的（南到北）加速度 |
| z | Number | null | 是 | 表示 z 轴上的（上到下）加速度 |

参考以下示例：

```JavaScript
veGame.sendAccelerationDataToGame({
  x: 0.2,
  y: 0.3,
  z: 0.4
})
```

模拟“摇一摇”示例代码：

```JavaScript
function mockShake(){
    let i = 0;
    let counter = 10;
    let flag = -1;
    let intervalId = setInterval(() => {
        if(i%counter === 0 ){
            flag = flag* -1;
        }
        let x = 0 + i*flag;
        let y = 0 + i*flag;
        let z = 0;
        bytePlay.sendAccelerationDataToGame({ x: x, y: y, z: z })
            .then(() => {
                console.log('传感器数据成功')
            })
        i += 0.5;
        if (i >= 300) {
            clearInterval(intervalId);
        }
    }, 10)
}
```

### 是否静音

描述：实例方法，关闭/开启播放声音；默认为 false，不静音；调用 start 接口之后才可调用。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| isMute | Boolean | false | 否 | 是否静音 |

参考以下示例：

```JavaScript
veGame.setAudioMute(isMute);
```

### 调节本地播放音量

描述：调节客户端本地播放音量。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| volume | Float | null | 是 | 音量大小，范围 [0,1] |

参考示例：

```JavaScript
veGame.getLocalAudioPlaybackVolume(); //获取客户端本地播放音量
veGame.setLocalAudioPlaybackVolume(volume); //设置客户端本地播放音量，范围 [0,1]
```

### 获取音频设备信息

描述：获取音频设备列表、或当前使用的音频设备。

返回值：promise

参考示例：

```JavaScript
veGame.getAudioPlaybackDeviceList(); //获取音频播放设备列表
veGame.getAudioCaptureDeviceList(); //获取音频采集设备列表
veGame.getAudioPlaybackDevice(); //获取当前使用的音频播放设备
veGame.getAudioCaptureDevice(); //获取当前使用的音频采集设备
```

### 设置音频播放设备

描述：设置音频播放的设备。
- 设置音频播放设备时，需要保证已开启音频数据发送，否则设置无效；
- 当使用蓝牙耳机进行音频播放时，需要获取蓝牙权限。

返回值：void

入参：
|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| deviceId | String | null | 是 | 音频播放设备 ID |

参考示例：

```JavaScript
veGame.setAudioPlaybackDevice(deviceId); //指定音频播放设备（系统喇叭、外接喇叭和耳机、有线外接播放器，蓝牙播放器等）
```

### 开启/关闭音频数据发送

描述：开启或关闭采集麦克风音频数据后发送给云端实例。注意: 需要开启麦克风使用权限。

返回值：void

参考示例：

```JavaScript
veGame.startSendAudioStream() //获取麦克风权限后，采集并发送音频数据
veGame.stopSendAudioStream() //关闭音频数据发送，并且不进行音频采集
```

### 获取视频设备信息

描述：获取视频采集设备信息。

返回值：promise

参考示例：

```JavaScript
veGame.getVideoCaptureDevice();
```

### 开启视频采集

描述：获取本地摄像头权限后，采集并发送摄像头视频数据。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| deviceId | String | null | 是 | 指定摄像头 ID |

参考示例：

```JavaScript
veGame.startVideoStream(deviceId);
```

### 切换摄像头

描述：切换前后摄像头。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| deviceId | String | null | 是 | 切换摄像头 ID |

参考示例：

```JavaScript
veGame.switchCamera(deviceId);
```

### 停止视频采集

描述：关闭视频数据发送，并且不进行视频采集。

返回值：void

参考示例：

```JavaScript
veGame.stopVideoStream();
```

### 设置本地视频画面布局

描述：设置本地视频画面布局。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| mode | Integer | null | 是 | 可选枚举值及说明如下：  <br>1：RENDER_MODE_HIDDEN（默认，视窗填满优先；缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长大于等于视窗对应边长）  <br>2：RENDER_MODE_FIT（视频帧内容全部显示优先；缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长小于等于视窗对应边长）  <br>3：RENDER_MODE_Fill（视频帧自适应画布；视频帧非等比缩放，直至画布被填满。在此过程中，视频帧的长宽比例可能会发生变化）|

参考示例：

```JavaScript
veGame.setLocalVideoView(mode);
```

### 镜像翻转本地摄像头画面

描述：使用“前置摄像头”采集时，是否开启镜像翻转本地摄像头画面。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| mode | Integer | null | 是 | 可选枚举值及说明如下：  <br>0（MIRROR_MODE_OFF，不开启镜像翻转，默认）  <br>1（MIRROR_MODE_ON，开启镜像翻转） |

参考示例：

```JavaScript
veGame.setLocalVideoMirrorMode(mode);
```

### 设置视频编码质量参数

描述：根据客户端的网络情况，配置本地最大视频编码质量参数。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| width | Number | null | 是 | 宽度 |
| height | Number | null | 是 | 高度 |
| frameRate | Number | null | 是 | 帧率 |
| maxKbps | Number | null | 是 | 最大码率 |

参考示例：

```JavaScript
veGame.setVideoEncoderConfig({
  width: 1080,
  height: 1920,
  frameRate: 30,
  maxKbps: 3000
})
```

### 设置显示光标的键盘按键

描述：在游戏中，当鼠标为隐藏锁定状态时，如需要显示光标，可自定义显示光标的键盘按键。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| keyCode | String | null | 是 | 键盘按键 |


参考示例：

```JavaScript
veGame.setSystemCursorShowStatControlKey(keyCode);
```

### 显示和隐藏本地光标

描述：当选择手动控制光标的显示/隐藏时（在 `start` 接口中设置 `systemCursorShowStatManual:true`），控制本地光标的显示或隐藏状态。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| show | Boolean | null | 是 | 显示或隐藏本地光标：  <br>true（显示）  <br>false（隐藏） |

参考示例：

```JavaScript
veGame.setSystemCursorShowStat(show);
```

### 设置光标隐藏状态下的灵敏度

描述：设置光标在隐藏状态下的灵敏度。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| sensitivity | Number | null | 是 | 设置光标灵敏度，可选值范围 (0, 2] |

参考示例：

```JavaScript
veGame.setSystemCursorSensitivity(sensitivity);
```

### 设置键盘启用状态

描述：设置是否允许用户使用键盘进行信息输入的能力。

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| enable | Boolean | true | 否 | 是否允许用户启用键盘进行信息输入：  <br>true（允许）  <br>false（禁止）|

参考示例：

```JavaScript
veGame.setKeyboardEnable(true);
veGame.setKeyboardEnable(false);
```

### 获取键盘启用状态

描述：获取键盘启用状态（即允许/禁止用户使用键盘输入信息）。

参考示例：

```JavaScript
const bool = veGame.getKeyboardEnable();
```

### 设置保活时间

描述：在游戏中动态设置客户端切后台或者客户端断连之后，云端游戏服务的保活时间（如不设置，则使用默认保活时长300秒；支持设置的上限值为7200，即2小时；如需设置更长时间，可联系火山引擎云游戏技术支持）。

> 当客户端切后台或客户端断连时开始计时。如用户在设置的保活时间到达时未切回前台或者未恢复连接，服务端将断开游戏连接。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| time | int | null | 是 | 保活时长，单位秒 |

参考示例：
```JavaScript
veGame.setIdleTime(10);
```

### 设置/获取无操作回收服务时间

描述：在游戏中动态设置无操作回收服务时长（需要在 `start` 成功之后设置；如不设置或设置为0，则使用默认时长300秒；支持设置的上限值为7200，即2小时；如需设置更长时间，可联系火山引擎云游戏技术支持）。

> 如果已在启动云游戏时通过 `autoRecycleTime` 参数设置，动态设置的时长优先级高。

返回值：promise
入参：
|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| time | Integer | null | 是 | 无操作回收服务时长，单位：秒 |

参考示例：

```JavaScript
veGame.setAutoRecycleTime(60).then(()=> {
  console.log('set successful');
});

veGame.getAutoRecycleTime().then(({duration})=> {
  console.log('get duration successful: ', duration);
});
```

### 刷新签名密钥

描述：在用户鉴权密钥过期之前，刷新签名密钥。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| ststoken | Object | null | 是 | 启动云游戏的令牌，为一个 JSON 对象，包含以下属性（通过调用服务端 STSToken 接口获取）：  <br>AccessKeyID：用于鉴权的临时 Access Key  <br>SecretAccessKey：用于鉴权的临时 Secret Key  <br>SessionToken：用于鉴权的临时 Token  <br>CurrentTime：Token 创建时间  <br>ExpiredTime：Token 过期时间  <br>有关服务端 STSToken 接口的详细信息，参考 [签发临时 Token]() |

参考示例：
```JavaScript
veGame.renewToken(ststoken)
```

### 坐标旋转

描述：当使用 CSS transform 的 rotate/rotate3d 进行 UI 旋转操作时，需要配合调用该方法进行坐标旋转，保证点击/触控坐标计算准确。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| degree | Number | null | 是 | 顺时针旋转角度，可选：0、90、180、270 |

参考以下示例：

```JavaScript
//顺时针旋转 90 度
veGame.rotateCoordinate(90)
```

### 旋转

描述：当云端实例屏幕方向发生变化时，需要配合调用该方法进行坐标旋转，保证点击/触控坐标计算准确。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| degree | Number | null | 是 | 顺时针旋转角度，可选：0、90、180、270 |

参考以下示例：

```JavaScript
//顺时针旋转 90 度
veGame.rotate(90)
```

### 客户端前后台切换

描述：设置客户端应用或游戏切换前后台的状态。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| on | Boolean | null | 是 | 客户端切换前后台：  <br>true（切后台）  <br>false（切前台） |


参考示例：

```JavaScript
veGame.switchBackground(true);
```

### 云端游戏切换前台

描述：将云端游戏切换到前台。

返回值：promise

参考示例：

```JavaScript
veGame.setRemoteGameForeground().then(() => {
  console.log('set successful');
});
```

### 更新画面放缩比

描述：游戏过程中容器（画布宽高比）发生变化时，更新调整画面的放缩比例，使游戏画面能够重新充满画布。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| videoElement | String | null | 是 | 当前渲染的游戏画面 |

参考示例：

```JavaScript
veGame.updateVideoScale();
```

### 设置全屏状态

描述：PC 端设置全屏、退出全屏（需要通过 `start()` 接口的配置参数 `enableFullScreen`，开启设备全屏显示，并在游戏启动成功之后使用）。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| state | Boolean | false | 是 | 设置是否全屏:  <br>true（进入全屏）  <br>false（退出全屏） |

参考示例：

```JavaScript
veGame.setFullScreenState(true)
```

### 设置重连时间

描述：设置游戏重连时间（单位：ms；在 `start` 成功后的回调中设置）。在游戏过程中或者游戏切换前后台时，因为网络问题需要重连时，支持指定游戏重连时长，防止重复尝试连接。重连超过指定时长时，会给出报错（错误码：40010）。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| time | int | null | 是 | 重连时长，单位：毫秒 |

参考示例：

```JavaScript
veGame.setReconnectTime(1000)
```

### 设置保存用户配置文件的路径

描述：设置保存游戏云端配置文件的路径，当用户正常退出或异常退出（切前后台保活到期、踢下线、游戏崩溃等），按照配置的路径保存用户的配置文件。当用户再次进入游戏时，拉取和还原已保存的用户配置信息。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| userProfilePath | String[] | null | 是 | 保存配置文件的路径 |

参考示例：

```JavaScript
veGame. setUserProfilePath(['/a/b/', '/c/d/'])
```

### 获取保存用户配置文件的路径

描述：当用户再次进入游戏时，获取保存用户配置文件的路径，拉取和还原已保存的用户配置信息。

返回值：promise

参考示例：

```JavaScript
veGame.getUserProfilePath().then((userProfilePath) => {
  console.log(userProfilePath);
})
```

### 设置游戏玩家角色

描述：在游戏中，设置游戏玩家的角色（操作者或观看者）。

注：需要在启动游戏时，通过 `start` 接口传入 `podSettings` 参数，开启游戏控制权转移功能。

返回值：promise

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| data | Object | null | 是 | 游戏玩家和角色信息：  <br>userId：游戏玩家用户 ID  <br>role：游戏玩家角色（0：观看者；1：操作者） |

参考示例：

```JavaScript
veGame.changeRole({
    userId: 'xxx',
    role: 1,
});
```

### 文件传输

描述：用于客户端与云端游戏之间传输文件的相关功能。

注：需要在实例化 SDK 时，通过设置参数 `needConnectFileChannel:true` 开启文件传输通道。

返回值：Promise

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| file | String | null | 是 | 发送的文件（支持发送的文件不能大于5MB） |
| needAck | Boolean | null | 是 | 是否需要接收文件发送到达回执 |

参考示例：

```JavaScript
let sn = await veGame.sendData(file, false);
```

### 大文件传输

描述：用于客户端与云端游戏之间传输文件数据的相关功能（需要与 Message Channel SDK V1.0.9 及以上版本配合使用。有关 Message Channel SDK 的使用方法，参考 [Message Channel SDK 接入说明]()）。

#### 发送大文件

描述：启动客户端向云端游戏发送文件。

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| file | File | null | 是 | 发送的文件 |
| config | Object | null | 否 | 控制发送文件的详细参数 |

参考示例：

```JavaScript
// 第二个参数可以不传
veGame.startSendFile(file);

veGame.startSendFile(file, {
    options: {
        value: 'xxx', // 自定义参数
    }
});
```

#### 暂停发送文件

描述：暂停客户端向云端游戏发送文件。

参考示例：

```JavaScript
veGame.pauseSendFile();
```

#### 停止发送文件

描述：停止客户端向云端游戏发送文件。

参考示例：

```JavaScript
veGame.stopSendFile();
```

#### 暂停下载文件

描述：暂停接收云端游戏发送到客户端的文件。

参考示例：

```JavaScript
veGame.pauseReceiveFile();
```

#### 停止下载文件

描述：停止接收云端游戏发送到客户端的文件。

参考示例：

```JavaScript
veGame.stopReceiveFile();
```

### 摇一摇协议

描述：用于触发云端游戏（应用）摇一摇，适配摇一摇场景。

返回值：Promise
入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| duration | Number | null | 否 | 摇一摇的持续时长，单位：ms，默认：1800 |

参考示例：

```JavaScript
veGame.sendShakeEventToGame(3000);
```

### 本地输入法显示中文合成过程

描述：部分输入法不支持显示合成过程，此接口用于显示或隐藏本地中文输入法合成过程。

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| visible | Boolean | null | 是 | 是否显示合成过程 |
| style | String | null | 否 | css 样式 |

参考示例：

```JavaScript
veGame.setIMECompositionVisible(true, 'background-color: transparent;');
```

### 同步本地键盘隐藏/关闭状态

描述：将本地键盘的隐藏或关闭状态同步到云端游戏（当用户点击软键盘完成按钮或进行其他关闭本地软键盘行为时，将软键盘关闭状态同步至云端游戏）。

返回值：void

参考示例：

```JavaScript
veGame.syncLocalKeyboardCloseStatus();
```

### 设置调试模式

描述：设置 debug 模式，用于开发过程中打印 SDK 内部调试信息

返回值：void

入参：

|  **参数名**  |  **类型**  |  **默认值**  |  **是否必选**  |  **描述**  |
| --- | --- | --- | --- | --- |
| enable | Boolean | false | 否 | 是否开启调试模式 |

参考示例：

```JavaScript
veGame.setDebug(true);
```

### 获取连接状态

描述：获取流连接状态。

返回值：String

参考示例：

```JavaScript
let state = veGame.getConnectionState();
```

### 获取实时运行状态  

描述：获取云游戏实时运行指标，包含延时、丢包率、帧率等。

返回值：Promise

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| accessDelay | Number | RTT 网络延时，单位为 ms |
| videoReceivedPacketsLostRate | Number | 接收视频丢包率 |
| videoReceivedBitrate | Number | 接收视频码率，单位为 kbps |
| videoReceivedFrameRate | Number | 接收视频帧率，单位为 fps |

参考示例：

```JavaScript
let state = await veGame.getStats();
```

## 事件回调

### message 事件

描述：运行过程中，云端实例发送到客户端的自定义消息事件回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| userId | String | 消息发送方的用 ID |
| text | String | 消息内容 |
| mid | String | 消息唯一标识 ID |
| time | Number | 消息发送时的时间戳 |

参考示例：

```JavaScript
veGame.on('message', (message) => {
  console.log(message.text);
});
```

### general-message-received 事件

描述：运行过程中，云端实例发送到客户端的消息已接收回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| from | String | 消息发送端 ID |
| to | String | 消息接收端 ID |
| msg | Object | 消息内容 |

参考示例：

```JavaScript
veGame.on('general-message-received', (message) => {
  console.log(message.msg);
});
```

### clipboard-message-received 事件

描述：运行过程中，云端实例剪贴板发生变化时的回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| textArray | Array[String] | 剪贴板内容，为一个字符串数组 |

参考示例：

```JavaScript
veGame.on('clipboard-message-received', (textArray) => {
  console.log(textArray[0]);
});
```

### error 事件

描述：SDK 运行过程中的错误回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| errorCode | Number | 错误码 |
| errorMessage | String | 错误信息，详细信息，可参考 [错误码](#错误码) |

参考示例：

```JavaScript
veGame.on('error', (err) => {
  console.log(err);
});
```

### warning 事件

描述：SDK 运行过程中的警告回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| warningCode | Number | 警告码 |
| message | String | 警告信息，详细信息，可参考 [警告码](#警告码) |

参考示例：

```JavaScript
veGame.on('warning', (info) => {
  console.log(info);
});
```

### message-channel-ready 事件

描述：消息收发通道就绪的事件。

回调入参：无

参考示例：

```JavaScript
veGame.on('message-channel-ready', () => {
});
```

### message-channel-online 事件

描述：通过消息通道收发消息时，云端实例在线的回调。

回调入参：destUserId

参考示例：

```JavaScript
veGame.on('message-channel-online', (destUserId) => {
});
```

### message-channel-offline 事件

描述：通过消息通道收发消息时，云端实例离线的回调（发送消息可能失败）。

回调入参：destUserId

参考示例：

```JavaScript
veGame.on('message-channel-offline', (destUserId) => {
});
```

### stream-reconnecting 事件

描述：游戏视频流开始重连的回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('stream-reconnecting', () => {
});
```

### stream-reconnected 事件

描述：游戏视频流重连成功的回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('stream-reconnected', () => {
});
```

### background-switched 事件

描述：客户端应用或游戏切换前后台的回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('background-switched', () => {
});
```

### remote-game-switched-background 事件

描述：云端游戏切换到后台的回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('remote-game-switched-background', () => {
});
```

### remote-game-switched-foreground 事件

描述：云端游戏切换到前台的回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('remote-game-switched-foreground', () => {
});
```

### remote-stream-start-request 事件

描述：收到云端请求开始音频数据事件回调。

回调入参：{isAudio,isVideo}

参考示例：

```JavaScript
veGame.on('remote-stream-start-request', (res) => {
    const {isAudio,isVideo} = res
});
```

### remote-stream-stop-request 事件

描述：收到云端请求停止音频数据事件回调。

回调入参：{isAudio,isVideo}

参考示例：

```JavaScript
veGame.on('remote-stream-stop-request', (res) => {
    const {isAudio,isVideo} = res
});
```

### start-audio-capture 事件

描述：音频采集开始回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('start-audio-capture', () => {
});
```

### stop-audio-capture 事件

描述：音频采集结束回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('stop-audio-capture', () => {
});
```

### start-video-capture 事件

描述：相机采集开始回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('start-video-capture', () => {
});
```

### stop-video-capture 事件

描述：相机采集结束回调。

回调入参：无

参考示例：

```JavaScript
veGame.on('stop-video-capture', () => {  
});
```

### audio-playback-route-updated 事件

描述：更新音频播放设备回调。

回调入参：deviceId

参考示例：

```JavaScript
veGame.on('audio-playback-route-updated', (res) => {
    const {deviceId} = res
});
```

### audio-capture-route-updated 事件

描述：更新音频采集设备回调。

回调入参：deviceId

参考示例：

```JavaScript
veGame.on('audio-capture-route-updated', (res) => {
    const {deviceId} = res
});
```

### send-stream-stats 事件

描述：发送流状态回调。

回调入参：Object

参考示例：

```JavaScript
veGame.on('send-stream-stats', (res) => {
    const {stats} = res
});
```

### received-stream-stats 事件

描述：接收流状态回调。

回调入参：Object

参考示例：

```JavaScript
veGame.on('received-stream-stats', (res) => {
    const {stats} = res
});
```

### video-capture-route-updated 事件

描述：更新相机采集设备回调。

回调入参：Object

参考示例：

```JavaScript
veGame.on('video-capture-route-updated', (res) => {
    const {deviceId} = res
});
```

### local-audio-stream-state 事件

描述：本地音频流状态回调。

回调入参：Object

参考示例：

```JavaScript
veGame.on('local-audio-stream-state', (res) => {
    const {state,deviceId} = res
});
```

### full-screen-change 事件

描述：全屏变化回调。

回调入参：Object

参考示例：

```JavaScript
veGame.on('full-screen-change', (res) => {
    const {originalEvent,state} = res
});
```

### full-screen-error 事件

描述：全屏错误回调。

回调入参：Object

参考示例：

```JavaScript
veGame.on('full-screen-error', (res) => {
    const {originalEvent} = res
});
```

### delay-detected 事件

描述：检测操作延迟事件回调，每 5s 触发一次；当事件被触发时，上报到数据平台（该方法仅在 Chrome 86+ 版本支持）。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| elapse | Number | 操作延时，单位 ms |

参考示例：

```JavaScript
veGame.on('delay-detected', (res) => {
    const { elapse } = res
});
```

### system-cursor-stat-changed 事件

描述：云端游戏内光标显示/隐藏状态的变化事件。

回调入参：String

参考示例：

```JavaScript
veGame.on('system-cursor-stat-changed', (status) => {
    console.log(status)
});
```

### local-system-cursor-stat-changed 事件

描述：本地光标显示/隐藏状态的变化事件。

回调入参：String

参考示例：

```JavaScript
veGame.on('local-system-cursor-stat-changed', (status) => {
    console.log(status)
});
```

### shake-response 事件

描述：摇一摇结果回调。

参考示例：

```JavaScript
veGame.on('shake-response', data=>{
    console.log(data);
})
```

### on-touch-event 事件

描述：触控事件回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| touchList | Array<touchItem> | 触控 list，长度由同时触控的手指数决定 |
| touchItem.action | ACTION | 鼠标动作，可选：（见常量部分）  <br>ACTION.TOUCH_START  <br>ACTION.TOUCH_END  <br>ACTION.TOUCH_MOVE |
| touchItem.pointerId | Number | 触控唯一 ID |
| touchItem.x | Number | 相对有效画面的位置 x 轴，0.00000 - 1.00000 |
| touchItem.y | Number | 相对有效画面的位置 y 轴，0.00000 - 1.00000 |

参考示例：

```JavaScript
veGame.on('on-touch-event', touchList => {
    console.log(touchList);
})
```

### 游戏控制权转移相关事件

描述：游戏控制权转移相关事件回调。

参考示例：

```JavaScript
veGame.on('control-start-response', (message) => {
    console.log(message)
});
veGame.on('control-change-response', (message) => {
    console.log(message)
});
veGame.on('control-broadcast', (message) => {
    console.log(message)
});
```

### 文件传输相关事件

描述：文件传输相关事件回调。

参考示例：

```JavaScript
veGame.on('on-filechannel-ack', (sn) => {
    console.log(sn)
});
veGame.on('on-filechannel-data', (data) => {
    console.log(data)
});
veGame.on('on-filechannel-error', (error) => {
    console.log(error)
});
veGame.on('on-filechannel-status-change', (status) => {
    console.log(status)
});
veGame.on('on-filechannel-reconnected', (status) => {
    console.log(status)
});
veGame.on('on-filechannel-connected', (status) => {
    console.log(status)
});
```

### 文件传输相关事件（基于对象存储）

描述：大文件传输相关事件回调（基于对象存储）。

参考示例：

```JavaScript
veGame.on('on-send-file-progress', (progress) => {
    console.log(progress)
});
veGame.on('on-send-file-done', (data) => {
    console.log(data)
});
veGame.on('on-download-file-progress', (progress) => {
    console.log(progress)
});
veGame.on('on-download-file-done', (data) => {
    console.log(data)
});

veGame.on('on-send-file-request-error', (error) => {
    console.log(error)
});
veGame.on('on-send-file-error', (error) => {
    console.log(error)
});
veGame.on('on-send-file-message-error', (error) => {
    console.log(error)
});
veGame.on('on-download-file-message-error', (error) => {
    console.log(error)
});
veGame.on('on-download-file-request-error', (error) => {
    console.log(error)
});
```

### 设置键盘启用状态相关事件

描述：设置键盘启用状态相关事件。

参考示例：

```JavaScript
veGame.on('on-keyboard-enable-result', (result) => {
    console.log(result)
});
veGame.on('on-keyboard-enable-change', (result) => {
    console.log(result)
});
```

### 本地键盘显示/打开事件

描述：本地键盘已显示或打开相关事件。

回调入参：void

参考示例：

```JavaScript
veGame.on('local-keyboard-open', () => {
    console.log('local-keyboard-open')
});
```

### 本地键盘隐藏/关闭事件

描述：本地键盘已隐藏或关闭相关事件。

回调入参：void

参考示例：

```JavaScript
veGame.on('local-keyboard-close', () => {
    console.log('local-keyboard-close')
});
```

### network-quality 事件

描述：网络质量检测回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| quality | Integer | 网络质量评级（可根据当前返回的网络质量评级进行推流参数降级或者终止拉流）：  <br>0（UNKNOWN，表示当前网络状况未知，无法判断网络质量）  <br>1（EXCELLENT，表示当前网络状况极佳，能够高质量承载当前业务）  <br>2（GOOD，表示当前网络状况良好，能够较好地承载当前业务）  <br>3（POOR，表示当前网络状况有轻微劣化，但不影响正常使用）  <br>4（BAD，表示当前网络质量欠佳，会影响当前业务的主观体验）  <br>5（VERY_BAD，表示当前网络已经无法承载当前业务的媒体流，需要采取相应策略，比如降低媒体流的码率或者更换网络）  <br>6（DOWN，表示当前网络完全无法正常通信） |

参考示例：

```JavaScript
veGame.on('network-quality', (quality) => {
    console.log(quality)
});
```

### 大文件传输相关事件

描述：客户端与云端游戏之间收发大文件相关事件。

参考示例：

```JavaScript
veGame.on('on-send-file-done', (data) => {
  console.log(data);
});
veGame.on('on-send-file-progress', (progress) => {
  console.log(progress);
});
veGame.on('on-send-file-info', (data) => {
  console.log(data);
});
veGame.on('on-send-file-error', (data) => {
  console.log(data);
});
veGame.on('on-send-file-message-error', (error) => {
  console.log(error);
});
veGame.on('on-send-file-request-error', (error) => {
  console.log(error);
});
veGame.on('on-download-file-done', (data) => {
  console.log(data);
});
veGame.on('on-download-file-progress', (progress) => {
  console.log(progress);
});
veGame.on('on-download-file-request-error', (error) => {
  console.log(data);
});
veGame.on('on-download-file-message-error', (error) => {
  console.log(data);
});
```

### 实体游戏手柄连接事件

描述：实体游戏手柄连接事件回调。

回调入参：

|  **参数名**  |  **类型**  |  **描述**  |
| --- | --- | --- |
| gamePadInfo | object | 游戏手柄属性：  <br>mapping（String，手柄的映射协议）  <br>index（Number，手柄的序号）  <br>id（String，手柄的 ID）  <br>buttonCount（Number，手柄的按钮数量）  <br>axesCount（Number，手柄的轴按钮数量） |

参考示例：

```JavaScript
veGame.on('gamepad-connected', (gamePadInfo) => {
    console.log(gamePadInfo)
});
veGame.on('gamepad-disconnected', (gamePadInfo) => {
    console.log(gamePadInfo)
});
```

## 常量

引入方法：

```
import { MODE, KEYCODE, ACTION } from 'cloudcore.min.js';
```

### MODE

描述：CloudCore 几种不同模式可选枚举值：

|  **键值**  |  **描述**  |
| --- | --- |
| MODE.CLOUD_PHONE_GAME | 云游戏模式，该模式下 SDK 将会默认监听触控事件 |
| MODE.CLOUD_PHONE | 云手机模式，该模式下 SDK 将会默认监听触控事件。并且在初始化参数 isPC 生效时，会默认监听键盘事件 |
| MODE.CLOUD_PC | 云 PC 模式，该模式下 SDK 将会默认监听鼠标、键盘事件 |

### KEYCODE

描述：按键键码可选枚举值，不适用于云游戏场景：

|  **键值**  |  **描述**  |
| --- | --- |
| KEYCODE.KEYCODE_HOME | Home 键 |
| KEYCODE.KEYCODE_BACK | 返回键 |
| KEYCODE.KEYCODE_VOLUME_UP | 音量增加 |
| KEYCODE.KEYCODE_VOLUME_DOWN | 音量降低 |
| KEYCODE.KEYCODE_POWER | 电源 |
| KEYCODE.KEYCODE_MENU | 菜单 menu |
| KEYCODE.KEYCODE_APP_SWITCH | 应用切换 |

### ACTION

描述：按键动作可选枚举值:

|  **键值**  |  **描述**  |
| --- | --- |
| ACTION.DOWN | 按键按下 |
| ACTION.UP | 按键放开 |
| ACTION.MOVE | 移动 |
| ACTION.WHEEL | 滚轮 |

## 错误码

### start方法

|  **错误码**  |  **描述**  |
| --- | --- |
| 100001 | 在 SDK API 层判断参数不合法 |
| 100003 | 已在 RTC 房间中，而调用了 start |
| 100004 | RTC 房间连接中，而调用了 start |
| 2000001 | start 超时，超过 20s 没有订阅上视频流 |

### stop方法

|  **错误码**  |  **描述**  |
| --- | --- |
| 100002 | 未在房间中，而调用了publish、leave等方法 |


### error 事件

|  **错误码**  |  **描述**  |
| --- | --- |
| 2001000 | 实例 crash |
| 2001001 | 游戏下架 |
| 2001002 | 网络连接超时退出 |
| 2001003 | 长时间无操作后，用户下线 |
| 300001 | 信令监听下流异常 |
| 300002 | peerConnection 错误，ICE failed / closed |
| 300003 | peerConnection 错误，connectionState failed / closed |
