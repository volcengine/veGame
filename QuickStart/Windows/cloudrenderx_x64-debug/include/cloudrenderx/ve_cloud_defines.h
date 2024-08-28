#pragma once

#include <cstdint>
#include <vector>

namespace vecommon {

#pragma warning(disable : 4200)
#pragma pack(push)
#pragma pack(1)

/**
* @type enum
*
* @brief 流类型
* 
* @locale zh
*/
enum class StreamType
{
    None = 0,
    /**
     * @brief 音频流类型
     */
    Audio,

    /**
     * @brief 视频流类型
     */
    Video,

    /**
     * @brief 音、视频流类型
     */
    Both
};

 /**
 * @type enum
 *
 * @brief 渲染模式
 * 
 * @locale zh
 */
enum class VideoScaleType {
    /**
     * @brief 视窗填满优先，默认值。
     *        视频帧等比缩放，直至视窗被视频填满。如果视频帧长宽比例与视窗不同，视频帧的多出部分将无法显示。
     *        缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长大于等于视窗对应边长。
     */
    Hidden = 1,
    /**
     * @brief 视频帧内容全部显示优先。
     *        视频帧等比缩放，直至视频帧能够在视窗上全部显示。如果视频帧长宽比例与视窗不同，视窗上未被视频帧填满区域将被涂黑。
     *        缩放完成后，视频帧的一边长和视窗的对应边长一致，另一边长小于等于视窗对应边长。
     */
    Fit = 2,
    /**
     *  @brief 视频帧自适应画布。
     *         视频帧非等比缩放，直至画布被填满。在此过程中，视频帧的长宽比例可能会发生变化。
     */
    Fill = 3,
};

/**
* @type enum
*
* @brief 视频帧类型
* 
* @locale zh
*/
enum class VideoPixelFormat {
    Rgba = 1,
    I420,
    NV12,
    NV21,
    D3D11
};

 /**
 * @type enum
 *
 * @brief 触摸事件类型，参考定义 https ://developer.android.com/reference/android/view/MotionEvent
 * 
 * @locale zh
 */
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

/**
 * @type enum
 *
 * @brief 网络状态
 * 
 * @locale zh
 */
enum class NetworkQualityState {
    Unknown = 0,
    Excellent,
    Good,
    Poor,
    Bad,
    VeryBad,
    Down
};

/**
 * @type enum
 *
 * @brief 云渲染连接状态
 * 
 * @locale zh
 */
enum class StreamConnectionState {
    /**
     * 连接断开，断网 12s 触发，内部自动重连
     */
    Disconnected = 1,
    /**
     * 首次请求建立连接，正在连接中
     */
    Connecting,
    /**
     * 首次连接成功
     */
    Connected,
    /**
     * 首次连接中或断开后自动重连中
     */
    Reconnecting,
    /**
     * 连接断开后，重连成功
     */
    Reconnected,
    /**
     * 处于 `DISCONNECTED` 状态超过 10 秒，且期间重连未成功
     */
    Lost,
    /**
     * 服务端异常状态导致失败，不会自动重试，可联系技术支持
     */
    Failed
};

/**
 * @type enum
 *
 * @brief 远端游戏切换类型
 * 
 * @locale zh
 */
enum class RemoteGameSwitchType {
    None = -1,
    ClientSwitch = 0,
    RemoteAutoSwitch,
    RepeatedlyGoFront
};

/**
 * @type enum
 *
 * @brief 相机ID
 * 
 * @locale zh
 */
enum class CameraId {
    Front = 0,
    Back = 1,
    External = 2,
    Invalid = 3
};

/**
 * @type enum
 * 
 * @brief 外部渲染视频帧的旋转角度
 * 
 * @locale zh
 */
enum RotateDegree {
    /**
     * 方向 ： ↑
     */
    DEGREE_0 = 0,
    /**
     * 方向 ： →
     */
    DEGREE_90 = 90,
    /**
     * 方向 ： ↓
     */
    DEGREE_180 = 180,
    /**
     * 方向 ： ←
     */
    DEGREE_270 = 270
};

/**
 * @type api
 *
 * @brief 触摸事件
 * 
 * @locale zh
 */
struct TouchData {
    /**
     * 触屏手指索引值
     */
    int16_t index{0};
    /**
     * 触屏类型
     */
    TouchType type{TouchType::None};
    /**
     * 触屏点在画面的 x 轴坐标，左-->右 映射到 [0,65535]
     */
    int32_t absX{0};
    /**
     * 触屏点在画面的 y 轴坐标，上-->下 映射到 [0,65535]
     */
    int32_t absY{0};

};

/**
 * @type api
 *
 * @brief 触摸事件数组
 * 
 * @locale zh
 */
struct TouchArrayData {
    uint8_t     count{0};
    TouchData   touch[0];
};

/**
 * @type api
 *
 * @brief 远端输入法状态数据
 * 
 * @locale zh
 */
struct ImeStateData {
    /**
     * 输入法状态，true 为开启，false 为关闭
     */
    bool enable{false};
    /**
     * 窗口客户区宽
     */
    int32_t w{0};
    /**
     * 窗口客户区高
     */
    int32_t h{0};
    /**
     * IME 窗口坐标 x，输入框左上角
     */
    int32_t x{0};
    /**
     * IME 窗口坐标 y，输入框左上角
     */
    int32_t y{0};
    /**
     * 字体宽
     */
    int32_t fw{0};
    /**
     * 字体高
     */
    int32_t fh{0};
};

/**
 * @type api
 *
 * @brief 游戏手柄震动数据结构
 * 
 * @locale zh
 */
struct GamepadVibrationData {
    /**
     * 手柄索引值
     */
    uint8_t index{0};
    /**
     * 左马达，参考 https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_vibration
     */
    int32_t lm{0};
    /**
     * 右马达，参考 https://docs.microsoft.com/en-us/windows/win32/api/xinput/ns-xinput-xinput_vibration
     */
    int32_t rm{0};
};

/**
 * @type api
 *
 * @brief 云游戏音频帧状态
 * 
 * @locale zh
 */
struct AudioStats {
    /**
     * 音频丢包率。统计周期内的音频下行丢包率，取值范围为 [0, 1]
     */
    float audio_loss_rate{0};
    /**
     * 接收码率。统计周期内的音频接收码率，单位为 kbps
     */
    int receivedKbitrate{0};
    /**
     * 音频卡顿次数。统计周期内的卡顿次数
     */
    int stallCount{0};
    /**
     * 音频卡顿时长。统计周期内的卡顿时长，单位为 ms
     */
    int stallDuration{0};
    /**
     * 用户体验级别的端到端延时。从发送端采集完成编码开始到接收端解码完成渲染开始的延时，单位为 ms
     */
    long e2eDelay{0};
    /**
     * 播放采样率。统计周期内的音频播放采样率信息，单位为 Hz
     */
    int playoutSampleRate{0};
    /**
     * 统计间隔。此次统计周期的间隔，单位为 ms
     */
    int statsInterval{0};
    /**
     * 客户端到服务端数据传输的往返时延，单位为 ms
     */
    int rtt{0};
    /**
     * 发送端——服务端——接收端全链路数据传输往返时延。单位为 ms
     */
    int totalRtt{0};
    /**
     * 云端用户发送的音频流质量
     */
    int quality{0};
    /**
     * 因引入 jitter buffer 机制导致的延时。单位为 ms
     */
    int jitterBufferDelay{0};
    /**
     * 音频声道数
     */
    int numChannels{0};
    /**
     * 音频接收采样率。统计周期内接收到的云端音频采样率信息，单位为 Hz
     */
    int receivedSampleRate{0};
    /**
     * 云端用户在加入房间后发生音频卡顿的累计时长占音频总有效时长的百分比。<br>
     * 音频有效时长是指云端用户进房发布音频流后，除停止发送音频流和禁用音频模块之外的音频时长
     */
    int frozenRate{0};
    /**
     * 音频丢包补偿(PLC) 样点总个数
     */
    int concealedSamples{0};
    /**
     * 音频丢包补偿(PLC) 累计次数
     */
    int concealmentEvent{0};
    /**
     * 音频解码采样率。统计周期内的音频解码采样率信息，单位为 Hz
     */
    int decSampleRate{0};
    /**
     * 解码时长。对此次统计周期内接收的云端音频流进行解码的总耗时，单位为 s
     */
    int decDuration{0};
    /**
     * 音频下行网络抖动，单位为 ms
     */
    int jitter{0};

};

/**
 * @type api
 *
 * @brief 云游戏视频帧状态
 * 
 * @locale zh
 */
struct VideoStats {
    /**
     * 清晰度 id
     */
    uint8_t clarity{0};
    /**
     * 视频宽度
     */
    int32_t width{0};
    /**
     * 视频高度
     */
    int32_t height{0};
    /**
     * 视频丢包率。统计周期内的视频下行丢包率，单位为 % ，取值范围：[0，1]
     */
    float videoLossRate{0};
    /**
     * 接收码率。统计周期内的视频接收码率，单位为 kbps
     */
    int32_t receivedKbitrate{0};
    /**
     * 解码器输出帧率。统计周期内的视频解码器输出帧率，单位 fps
     */
    int32_t decoderOutputFrameRate{0};
    /**
     * 渲染帧率。统计周期内的视频渲染帧率，单位 fps
     */
    int32_t rendererOutputFrameRate{0};
    /**
     * 卡顿次数。统计周期内的卡顿次数
     */
    int32_t stallCount{0};
    /**
     * 卡顿时长。统计周期内的视频卡顿总时长。单位 ms
     */
    int32_t stallDuration{0};
    /**
     * 用户体验级别的端到端延时。从发送端采集完成编码开始到接收端解码完成渲染开始的延时，单位为 ms
     */
    int64_t e2eDelay{0};
    /**
     * 所属用户的媒体流是否为屏幕流。你可以知道当前数据来自主流还是屏幕流
     */
    bool isScreen{false};
    /**
     * 统计间隔，此次统计周期的间隔，单位为 ms，此字段用于设置回调的统计周期，目前设置为 2s
     */
    int32_t statsInterval{0};
    /**
     * 往返时延，单位为 ms
     */
    int32_t rtt{0};
    /**
     * 云端用户在进房后发生视频卡顿的累计时长占视频总有效时长的百分比（%）。<br>
     * 视频有效时长是指云端用户进房发布视频流后，除停止发送视频流和禁用视频模块之外的视频时长
     */
    int32_t frozenRate{0};
    /**
     * 对应多种分辨率的流的下标
     */
    int32_t videoIndex{0};
    /**
     * 视频下行网络抖动，单位为 ms
     */
    int32_t jitter{0};

};

/**
* @type enum
*
* @brief 视频旋转枚举定义
* 
* @locale zh
*/
enum class VideoRotation {
    /**
     * 顺时针旋转 0 度
     */
    R0 = 0,
    /**
     * 顺时针旋转 90 度
     */
    R90 = 90,
    /**
     * 顺时针旋转 180 度
     */
    R180 = 180,
    /**
     * 顺时针旋转 270 度
     */
    R270 = 270
};

/**
 * @type api
 *
 * @brief 视频帧信息
 * 
 * @locale zh
 */
struct VideoFrameInfo {
    /**
     * 视频帧宽（像素）
     */
    int32_t width{ 0 };
    /**
     * 视频帧高（像素）
     */
    int32_t height{ 0 };
    /**
     * 视频帧顺时针旋转角度
     */
    VideoRotation rotation{ VideoRotation::R0 };

};

/**
 * @type api
 * 
 * @brief 自定义的编码配置
 * 
 * @locale zh
 */
struct VideoEncodeConfig {
    /**
     * 视频宽度，单位像素
     */
    int width;

    /**
     * 视频高度，单位像素
     */
    int height;

    /**
     * 帧率
     */
    int fps;

    /**
     * 最大发送码率
     */
    int max_send_kbps;
};

#pragma pack(pop)

} // namespace vecommon