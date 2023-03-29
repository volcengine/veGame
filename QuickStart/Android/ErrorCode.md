|errorCode|errorMsg|错误原因|操作建议|
|:---:|:---:|:---:|:---:|
|10000|ERROR_START_GENERAL|开始游戏失败。原因：通用错误。|请检查开始游戏 start() 接口参数。|
|10001|ERROR_START_AUTHENTICATION_FAILED|开始游戏失败。原因：火山引擎服务鉴权失败。|检查您的 AK、SK、Token 生成，具体参考服务端 STSToken 接口。|
|10002|ERROR_START_GAME_ID_OR_CUSTOM_GAME_ID_NOT_EXIST|开始游戏失败。原因：当前游戏 ID 或 自定义游戏 ID 不存在。|请通过火山引擎云游戏控制台『游戏管理』页面获取正确的游戏 ID。|
|10003|ERROR_START_GAME_ID_NOT_READY|开始游戏失败。原因：当前游戏尚在适配中。|游戏适配需要一定时间。如需加急，可联系火山引擎云游戏服务对接人员。|
|10004|ERROR_START_CONFIGURATION_CODE_NOT_EXIST|开始游戏失败。原因：资源套餐 ID 不存在。|可通过调用服务端 ListResourceSet 接口（configuration_code 字段）获取正确的套餐信息。|
|10005|ERROR_START_CONFIGURATION_CODE_NOT_REDAY|开始游戏失败。原因：游戏尚未配置资源套餐或套餐 ID 无效。|请通过火山引擎云游戏控制台『游戏管理』页面为游戏配置部署策略。|
|10006|ERROR_START_RESOURCE_NOT_READY|开始游戏失败。原因：当前业务没有订购资源。|请通过火山引擎云游戏控制台『资源管理』页面购买资源。|
|10007|ERROR_START_RESOURCE_CAPACITY_NOT_ENOUGH|开始游戏失败。原因：当前游戏资源用量超限或无可用资源。|请通过火山引擎云游戏控制台『资源管理』页面购买资源。|
|10009|ERROR_START_AUTHENTICATION_KEY_FAILED|开始游戏失败。原因：火山引擎服务鉴权失败。|请检查临时 Token 的有效期，可参考服务端 签发临时 Token 接口说明。|
|10010|ERROR_START_NO_STOP_BEFORE|开始游戏失败。原因：同一个客户端连续调用了两次 start() 接口启动游戏，之前未调用 stop() 接口停止游戏。|请调用服务端 GameStop 接口结束上一次 “游戏”，再调用 start() 接口开始游戏。|
|10011|ERROR_START_CONNECTION_ENDED|开始游戏失败。原因：在调用 start() 接口启动游戏、启动成功回调触发之前，游戏被停止（可能原因包括：服务端游戏崩溃导致开始游戏失败、调用了服务端 GameStop 接口停止游戏服务）。| |
|10012|ERROR_START_RESERVED_ID_NOT_FOUND|开始游戏失败。原因：资源预锁定 ID（reservedId）不存在。|请确保指定的 reservedId 正确；可通过调用服务端 PreAllocateResource 接口获取。|
|10013|ERROR_START_RESERVED_ID_EXPIRED|开始游戏失败。原因：资源预锁定 ID（reservedId）已失效。|可通过调用服务端 PreAllocateResource 接口重新获取。|
|10014|ERROR_START_RESERVED_ID_ALREADY_USED_RELEASE|开始游戏失败。原因：资源预锁定 ID（reservedId）已被使用且已释放。|可通过调用服务端 PreAllocateResource 接口重新获取。|
|10015|ERROR_START_RESERVED_ID_USING|开始游戏失败。原因：资源预锁定 ID（reservedId）正在被使用中。|可通过调用服务端 PreAllocateResource 接口重新获取。|
|10016|ERROR_START_RESERVED_ID_MISMATCH_PREPARE|开始游戏失败。原因：资源预锁定 ID（reservedId）与调用 PreAllocateResource 接口时指定的参数不一致。|请检查资源预锁定 ID（reservedId）与调用 PreAllocateResource 接口时指定的参数是否一致。|
|10017|ERROR_START_NO_SUFFICIENT_FUND|开始游戏失败。原因：启动游戏时，后付费账户欠费，导致服务不可用。|请通过火山引擎『费用中心』充值付费，充值到账后服务自动开启。|
|10018|ERROR_START_USER_CONFLICT|开始游戏失败。原因：用户使用多个设备申请游戏服务时，触发了游戏多开限制。|需要客户端提示用户不要进行多开操作。|
|10019|WARNING_START_INVALID_AUTO_RECYCLE_TIME|开始游戏失败。原因：设置无操作回收服务时长出错。|请参考 autoRecycleTime 参数描述进行设置。|
|10021|ERROR_START_RESERVEDID_PLAN_NOT_MATCHED| | |
|10023|WARNING_START_WITH_FRAMEWORK_NOT_FOUND|指定的伴随程序不存在。原因：传入的伴随程序包名错误。|检查是否已上传指定的伴随程序、且伴随程序包名称正确。|
|10024|WARNING_START_WITH_FRAMEWORK_PART_MATCH|指定的部分伴随程序不存在。原因：传入的伴随程序包名部分错误。|检查是否已上传指定的伴随程序、且伴随程序包名称是否正确。|
|10025|WARNING_START_WITH_FRAMEWORK_WRONG_INPUT_FORMAT|指定的伴随程序包名格式错误。|检查启动游戏接口的 extra 参数格式是否正确。|
|10026|ERROR_START_MISMATCH_ACCOUNTID|sdk传入的accountId 和 start 请求的gameId 所属的accountId 不一致|填入正确的accountId|
|10027|ERROR_START_INVALID_LOCAL_TIME|用户手机时间和服务端时间差超过7天(前后)|用户把手机时间修正为标准时间|
| | | | |
| | | | |
| | | | |
|20000|ERROR_STREAM_GENERAL|游戏串流连接错误，原因：网络原因。|请检查网络连接。|
|20002|ERROR_STREAM_CHANGE_CLARITY_ID_NOT_IN_START_STATE|切换清晰度失败。原因：当前处于非播放状态 。|请调用 start() 接口，在收到播放成功回调后，再调用切换清晰度档位接口。|
|20003|ERROR_STREAM_USER_MESSAGE_SEND_RESULT| | |
| | | | |
| | | | |
| | | | |
|30000|ERROR_SDK_GENERAL|客户端 SDK 通用错误。| |
|30001|ERROR_SDK_INIT_FAILED|客户端 SDK 初始化失败。| |
|30002|ERROR_SDK_<Parameter>_EMPTY|客户端调用 SDK 接口错误。原因：接口参数为空。|请参考接口说明文档，检查接口参数。|
|30007|WARNING_SDK_LACK_OF_LOCATION_PERMISSION|SDK 获取本地定位服务失败| |
|30009|ERROR_INIT_ACCOUNT_ID_ILLEGAL|火山 accountId 为空 |用户需要填入accountId|
|30201|ERROR_DOWNLOAD_PLUGIN_FAILED|内部上报事件，不需要对外暴露错误码| |
|30203|ERROR_INJECT_DEX_FAILED| | |
|30204|ERROR_CHECK_PLUGIN_FAILED| | |
|30205|ERROR_LOAD_PLUGIN_FAILED| | |
|31000|ERROR_CONFIG_PLATFORM_AUTH_FAILED| | |
|31001|ERROR_REQUEST_INIT_PARAMS_FAILED| | |
|31002|ERROR_MATCH_INIT_PARAMS_FAILED| | |
|31003|ERROR_REQUEST_PLUGIN_CONFIG_FAILED| | |
|31004|ERROR_MATCH_PLUGIN_CONFIG_FAILED| | |
|31005|ERROR_CONFIG_PLATFORM_REQUEST_AUTH_FAILED|网络原因|请检查网络或者重试|
| | | | |
| | | | |
| | | | |
|40000|ERROR_GAME_ABNORMAL_EXIT|游戏停止。原因：云端服务异常退出。| |
|40001|ERROR_GAME_CRASH|游戏停止。原因：服务端游戏崩溃。|请尝试复现游戏在本地设备是否运行异常，如本地未复现，可以联系火山引擎云游戏服务技术支持。|
|40002|ERROR_GAME_OFFLINE|游戏停止。原因：当前游戏被下架。|游戏停止。原因：当前游戏被下架。建议：请通过火山引擎云游戏控制台『游戏管理』页面或调用服务端 ListGame 接口查看当前游戏的上架状态。|
|40003|ERROR_GAME_EXIT_TIMEOUT|游戏停止。原因：超时退出。已废弃| |
|40004|ERROR_GAME_STOPPED_IDLE|游戏停止。原因：用户长时间未操作，服务端自动断开游戏连接。|客户端需要提示用户长时间无操作被踢下线。|
|40005|ERROR_GAME_STOPPED_RESET|游戏停止。原因：服务端主动重置了游戏。|请联系火山引擎云游戏服务技术支持。|
|40006|ERROR_GAME_STOPPED_API|游戏停止。原因：一般是通过调用 OPEN API Stop 接口主动停止了游戏。| |
|40007|ERROR_GAME_STOPPED_DUPLICATE_START|游戏停止。原因：某个 start() 请求使用了与当前游戏相同的 userId 和 gameId，导致当前游戏停止。|请检查调用 start() 接口的参数是否重复。如果需要重新启动被停止的游戏，请再次使用相同的 userId 和 gameId 调用 start() 接口。|
|40008|ERROR_POD_STOPPED_BACKGROUND_TIMEOUT|游戏停止。原因：游戏切换后台，超过指定的保活时长未切换前台，服务端自动断开游戏连接。建议：客户端需要提示用户因为游戏切后台超过保活时长，连接断开。| |
|40023|MESSAGE_RESOURCE_RELEASED_NORMAL|实例正常释放。| |
|40024|MESSAGE_RESOURCE_RELEASED_NO_USER|实例异常释放：客户端超时（2分钟）未加入| |
|40026|MESSAGE_RESOURCE_RELEASED_OS_MISSED|实例异常释放：游戏镜像缺失。| |
|40027|MESSAGE_RESOURCE_RELEASED_GAME_START_FAILURE|实例异常释放：游戏启动失败。| |
|40028|MESSAGE_RESOURCE_RELEASED_STREAMING_ERROR|实例异常释放：RTC 推流成功，但是推流过程中出现异常。|请联系火山引擎云游戏服务技术支持。|
|40029|MESSAGE_RESOURCE_RELEASED_3RD_APP_MISSED|实例异常释放：伴随包镜像缺失。|请联系火山引擎云游戏服务技术支持。|
|40032|MESSAGE_CLOUD_GAME_CRASH_OFTEN|游戏频繁崩溃。|请尝试复现游戏在本地设备是否运行异常，如本地未复现，可以联系火山引擎云游戏服务技术支持。|
|40033|MESSAGE_GAME_STEAMING_FAILURE|RTC 推流不成功。|请联系火山引擎云游戏服务技术支持。|
|40037|WARNING_LOCAL_ALREADY_SET_BACKGROUND|本地客户端应用已切换到后台，请勿重复调用切换后台接口。| |
|40038|WARNING_LOCAL_ALREADY_SET_FOREGROUND|本地客户端应用已切换到前台，请勿重复调用切换前台接口。| |
|40040|WARNING_REMOTE_<package name>_FOREGROUND_FAILURE_NO_SUCH_PACKAGE_NAME|云端游戏切换前台失败。原因：游戏包名不存在。|请检查指定的游戏包名是否正确。|
|40041|WARNING_REMOTE_<package name>_ALREADY_FOREGROUND|云端游戏切换前台失败。原因：云端游戏已处于前台。| |
|40043|WARNING_REMOTE_<package name>_FOREGROUND_FAILURE_NO_PACKAGE_NAME_BACKGROUND|云端游戏切换前台失败。原因：指定的游戏不在后台或已被关闭。| |
|40044|WARNING_GAME_STOPPED_INGAME_EXIT| | |
|40047|MESSAGE_RESOURCE_RELEASED_INVALID_PARAMETER| | |
|40048|MESSAGE_RESOURCE_RELEASED_HEART_BEAT_TIMEOUT| | |
|40049|MESSAGE_RESOURCE_RELEASED_INGAME_EXIT| | |
|40050|MESSAGE_RESOURCE_RELEASED_START_ERROR_ARCHIVE_DOWNLOAD_FAILURE| | |
| | | | |
| | | | |
| | | | |
|50000|ERROR_MESSAGE_GENERAL|未知错误| |
|50001|ERROR_MESSAGE_NOT_CONNECTED|在客户端SDK与云端应用建联之前发送消息触发|在收到 onRemoteOnline 回调之后再发送消息|
|50002|ERROR_MESSAGE_FAILED_TO_PARSE_MSG|无法解析云端发送过来的消息时触发|联系OnCall|
|50003|ERROR_MESSAGE_CHANNEL_UID_ILLEGAL|发送消息时，UID非法时触发|检查sendMessage的Uid参数|
|50007|ERROR_MESSAGE_OVER_SIZED|发送数据超过包体大小(60KB)时触发|检查sendMessage的payload的长度|
|50009|ERROR_MESSAGE_TIMEOUT_ILLEGAL|MessageChannel sendMessage timeout 值非法时触发, 合法值：> 0|检查sendMessage的timeout参数|
| | | | |
| | | | |
| | | | |
|60001|ERROR_NET_REQUEST_ERROR|网络请求失败。|请检查网络。|
|60002|ERROR_HTTP_REQUEST_ERROR|服务端接口返回错误。| |
| | | | |
| | | | |
| | | | |
|71001|ERROR_FILE_CHANNEL_FILE_NOT_FOUND|要发送的文件不存在|检查文件路径|
|71002|ERROR_FILE_CHANNEL_FILE_NO_ACCESS|要发送的文件没有访问权限|检查应用是否拥有该路径的访问权限|
|71003|ERROR_FILE_CHANNEL_NETWORK_ABORT|发送和接收文件过程发生网络异常|检查网络状态或重试|
|71004|ERROR_FILE_CHANNEL_TIMEOUT|发送和接收文件过程超时|检查网络状态或重试|
|71005|ERROR_FILE_CHANNEL_STORAGE_NOT_ENOUGH|接收文件的磁盘空间不足|检查磁盘空间|
|71006|ERROR_FILE_CHANNEL_INTERNAL_ERROR|发送和接收文件时内部发生逻辑错误，通常是依赖库集成原因触发的|检查应用引入依赖库是否同云游戏SDK冲突|
| | | | |
