#pragma once

#include <cstdint>
#include <vector>

namespace vegame {

#pragma warning(disable : 4200)
#pragma pack(push)
#pragma pack(1)

enum class VideoRotation {
    R0      = 0,            // 顺时针旋转 0 度
    R90     = 90,           // 顺时针旋转 90 度
    R180    = 180,          // 顺时针旋转 180 度
    R270    = 270           // 顺时针旋转 270 度
};

enum class VideoPixelFormat {
    Rgba = 1,
    I420,
    NV12,
    NV21,
    D3D11
};

// 参考 https://developer.android.com/reference/android/view/MotionEvent#ACTION_DOWN
enum class TouchType {
    None = -1,
    Down = 0,
    Up,
    Move,
    Cancel,
    Outside,
    PointerDown,
    PointerUp
};

enum class ImeCompositionType {
    Begin,
    End,
    Input,
};

enum class NetworkQualityState {
    Unknown = 0,
    Excellent,
    Good,
    Poor,
    Bad,
    VeryBad,
    Down
};

enum class StreamConnectionState {
    Disconnected = 1,       // 连接断开，断网 12s 触发，内部自动重连
    Connecting,             // 首次请求建立连接，正在连接中
    Connected,              // 首次连接成功
    Reconnecting,           // 首次连接时，10秒连接不成功；或连接成功后，断连 10 秒，自动重连中
    Reconnected,            // 连接断开后，重连成功
    Lost,                   // 处于 `DISCONNECTED` 状态超过 10 秒，且期间重连未成功
    Failed                  // 服务端异常状态导致失败，不会自动重试，可联系技术支持
};

enum class MessageChannelState {
    Connected = 1,
    Disconnected = 2,
};

enum class RemoteGameSwitchType {
    None = -1,
    ClientSwitch = 0,
    RemoteAutoSwitch,
    RepeatedlyGoFront
};


// EngineConfig
typedef struct tagEngineConfig {
    const char* log_file_dir{ nullptr };            // 本地日志文件夹名称
    const char* account_id{ nullptr };              // 火山引擎用户账号
    const char* user_id{ nullptr };                 // 自定义客户端用户 id
    bool        enable_debug{ false };              // 是否开启调试日志
} EngineConfig;


// GameConfig
typedef struct tagGameConfig {
    const char* ak{ nullptr };                      // 用户鉴权临时 access key
    const char* sk{ nullptr };                      // 用户鉴权临时 secret key
    const char* token{ nullptr };                   // 用户鉴权临时 token
    const char* game_id{ nullptr };                 // 游戏 id
    const char* custom_game_id{ nullptr };          // 自定义游戏 id，与 game_id 二选一
    const char* round_id{ nullptr };                // 本次生命周期标识符
    const char* user_tag{ nullptr };                // 用户标签
    const char* plan_id{ nullptr };                 // 套餐 id
    const char* reserved_id{ nullptr };             // 资源预锁定 id
    const char* profile_path_list{ nullptr };       // 保存用户游戏配置文件的路径列表
    void*       canvas{ nullptr };                  // 拉流后显示画面的画布，Windows 为 HWND
    int32_t     auto_recycle_time{ 0 };             // 设置用户无操作自动回收时长
    int32_t     video_stream_profile_id{ 0 };       // 游戏视频流清晰度 ID
    bool        enable_screen_lock{ false };        // 是否锁定屏幕横竖屏显示
    bool        enable_local_keyboard{ true };      // 是否开户本地键盘输入
    const char* extra{ nullptr };                   // 自定义扩展参数 todo(wuwenpeng):
} GameConfig;


// todo(wuwenpeng): 值对齐
// 操作相关
typedef struct tagKeyboardData {
    uint8_t key{ 0 };               // 键盘按键 key code，参考 https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    uint8_t down { 0 };             // 键盘按键状态   0 up, 1 down
} KeyboardData;

typedef struct tagMouseKeyData {
    uint8_t key{ 0 };               // 鼠标按键 key code，参考 https://docs.microsoft.com/en-us/windows/win32/inputdev/virtual-key-codes
    uint8_t down{ 0 };              // 鼠标按键状态   0 up, 1 down
    int32_t abs_x{ 0 };             // 鼠标在画面的 x 轴坐标，左-->右 映射到 [0,65535]
    int32_t abs_y{ 0 };             // 鼠标在画面的 y 轴坐标，上-->下 映射到 [0,65535]
} MouseKeyData;

typedef struct tagMouseMoveData {
    int32_t abs_x{ 0 };             // 鼠标在画面的 x 轴坐标，左-->右 映射到 [0,65535]
    int32_t abs_y{ 0 };             // 鼠标在画面的 y 轴坐标，上-->下 映射到 [0,65535]
    int32_t delta_x{ 0 };           // 相对上次鼠标位置 x 方向移动值，左负，右正
    int32_t delta_y{ 0 };           // 相对上次鼠标位置 y 方向移动值，上负，下正
} MouseMoveData;

typedef struct tagMouseWheelData {
    int16_t wheel{ 0 };             // 鼠标滚轮竖向滚动值
    int16_t hwheel{ 0 };            // 鼠标滚轮横向滚动值
} MouseWheelData;

typedef struct tagGamepadInputData {
    uint8_t index{ 0 };             // 手柄索引值
    int32_t buttons{ 0 };           // 手柄按键值，参数 msdn XINPUT_GAMEPAD 结构 https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_gamepad
    uint8_t lt{ 0 };                // left trigger，范围 [0, 256)
    uint8_t rt{ 0 };                // right trigger，范围 [0, 256)
    int16_t lx{ 0 };                // left thumbstick x，范围 [-32768, 32768)
    int16_t ly{ 0 };                // left thumbstick y，范围 [-32768, 32768)
    int16_t rx{ 0 };                // right thumbstick x，范围 [-32768, 32768)
    int16_t ry{ 0 };                // right thumbstick y，范围 [-32768, 32768)
} GamepadInputData;

typedef struct tagTouchData {
    int16_t     index{ 0 };                     // 触屏手指索引值
    TouchType   type{ TouchType::None };        // 触屏类型
    int32_t     abs_x{ 0 };                     // 触屏点在画面的 x 轴坐标，左-->右 映射到 [0,65535]
    int32_t     abs_y{ 0 };                     // 触屏点在画面的 y 轴坐标，上-->下 映射到 [0,65535]
} TouchData;

typedef struct tagTouchArrayData {
    uint8_t     count{ 0 };         // 触屏手指数
    TouchData   touch[0];
} TouchArrayData;

typedef struct tagImeCompositionData {
    ImeCompositionType  type;       // 输入类型
    const char*         str;        // 字符串，utf8 格式
} ImeCompositionData;

typedef struct tagImeStateData {
    bool    enable{ false };        // 输入法状态，true 为开启，false 为关闭
    int32_t w{ 0 };                 // 窗口客户区宽
    int32_t h{ 0 };                 // 窗口客户区高
    int32_t x{ 0 };                 // IME 窗口坐标 x，输入框左上角
    int32_t y{ 0 };                 // IME 窗口坐标 y，输入框左上角
    int32_t fw{ 0 };                // 字体宽（平均，差不多就行）
    int32_t fh{ 0 };                // 字体高
} ImeStateData;

typedef struct tagMouseCursorPos {
    int32_t abs_x{ 0 };             // 鼠标在画面的 x 轴坐标，左-->右 映射到 [0,65535]
    int32_t abs_y{ 0 };             // 鼠标在画面的 y 轴坐标，上-->下 映射到 [0,65535]
} MouseCursorPos;

typedef struct tagGamepadVibrationData {
    uint8_t index{ 0 };             // 手柄索引值
    int32_t lm{ 0 };                // 左马达，参考 https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_vibration
    int32_t rm{ 0 };                // 右马达
} GamepadVibrationData;


// 回调
typedef struct tagAudioStats {
    float   audio_loss_rate{ 0 };           // 音频丢包率。统计周期内的音频下行丢包率，取值范围为 [0, 1]
    int     received_kbitrate{ 0 };         // 接收码率。统计周期内的音频接收码率，单位为 kbps
    int     stall_count{ 0 };               // 音频卡顿次数。统计周期内的卡顿次数
    int     stall_duration{ 0 };            // 音频卡顿时长。统计周期内的卡顿时长，单位为 ms
    long    e2e_delay{ 0 };                 // 用户体验级别的端到端延时。从发送端采集完成编码开始到接收端解码完成渲染开始的延时，单位为 ms
    int     playout_sample_rate{ 0 };       // 播放采样率。统计周期内的音频播放采样率信息，单位为 Hz
    int     stats_interval{ 0 };            // 统计间隔。此次统计周期的间隔，单位为 ms
    int     rtt{ 0 };                       // 客户端到服务端数据传输的往返时延，单位为 ms
    int     total_rtt{ 0 };                 // 发送端——服务端——接收端全链路数据传输往返时延。单位为 ms
    int     quality{ 0 };                   // 云端用户发送的音频流质量
    int     jitter_buffer_delay{ 0 };       // 因引入 jitter buffer 机制导致的延时。单位为 ms
    int     num_channels{ 0 };              // 音频声道数
    int     received_sample_rate{ 0 };      // 音频接收采样率。统计周期内接收到的云端音频采样率信息，单位为 Hz
    int     frozen_rate{ 0 };               // 云端用户在加入房间后发生音频卡顿的累计时长占音频总有效时长的百分比。音频有效时长是指云端用户进房发布音频流后，除停止发送音频流和禁用音频模块之外的音频时长
    int     concealed_samples{ 0 };         // 音频丢包补偿(PLC) 样点总个数
    int     concealment_event{ 0 };         // 音频丢包补偿(PLC) 累计次数
    int     dec_sample_rate{ 0 };           // 音频解码采样率。统计周期内的音频解码采样率信息，单位为 Hz
    int     dec_duration{ 0 };              // 解码时长。对此次统计周期内接收的云端音频流进行解码的总耗时，单位为 s
    int     jitter{ 0 };                    // 音频下行网络抖动，单位为 ms
} AudioStats;

typedef struct tagVideoStats {
    uint8_t clarity{ 0 };                       // 清晰度 id
    int32_t width{ 0 };                         // 视频宽度
    int32_t height{ 0 };                        // 视频高度
    float   video_loss_rate{ 0 };               // 视频丢包率。统计周期内的视频下行丢包率，单位为 % ，取值范围：[0，1]
    int32_t received_kbitrate{ 0 };             // 接收码率。统计周期内的视频接收码率，单位为 kbps
    int32_t decoder_output_frame_rate{ 0 };     // 解码器输出帧率。统计周期内的视频解码器输出帧率，单位 fps
    int32_t renderer_output_frame_rate{ 0 };    // 渲染帧率。统计周期内的视频渲染帧率，单位 fps
    int32_t stall_count{ 0 };                   // 卡顿次数。统计周期内的卡顿次数
    int32_t stall_duration{ 0 };                // 卡顿时长。统计周期内的视频卡顿总时长。单位 ms
    int64_t e2e_delay{ 0 };                     // 用户体验级别的端到端延时。从发送端采集完成编码开始到接收端解码完成渲染开始的延时，单位为 ms
    bool    is_screen{ false };                 // 所属用户的媒体流是否为屏幕流。你可以知道当前数据来自主流还是屏幕流
    int32_t stats_interval{ 0 };                // 统计间隔，此次统计周期的间隔，单位为 ms，此字段用于设置回调的统计周期，目前设置为 2s
    int32_t rtt{ 0 };                           // 往返时延，单位为 ms
    int32_t frozen_rate{ 0 };                   // 云端用户在进房后发生视频卡顿的累计时长占视频总有效时长的百分比（%）。视频有效时长是指云端用户进房发布视频流后，除停止发送视频流和禁用视频模块之外的视频时长
    int32_t video_index{ 0 };                   // 对应多种分辨率的流的下标
    int32_t jitter{ 0 };                        // 视频下行网络抖动，单位为 ms
} VideoStats;

typedef struct tagVideoFrameInfo {
    int32_t         width{ 0 };                     // 视频帧宽（像素）
    int32_t         height{ 0 };                    // 视频帧高（像素）
    VideoRotation   rotation{ VideoRotation::R0 };  // 视频帧顺时针旋转角度
} VideoFrameInfo;

#pragma pack(pop)

} // namespace vegame
