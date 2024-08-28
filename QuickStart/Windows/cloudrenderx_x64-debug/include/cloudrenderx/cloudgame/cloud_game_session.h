#pragma once
#include "ve_xsession.h"
#include "ve_cloud_defines.h"
#include "game_session_listener.h"
#include <memory>

namespace vegame {
   class CloudGameKeyboardService;
   class CloudGameInputMethodService;
   class CloudGameClipboardService;
   class CloudGameDelayDetectService;
   class CloudGameMessageChannelService;
   class CloudGameMouseService;
   class CloudgameExternalRenderService;
   class CloudGamePadService;
   class CloudGamePodManagerService;
   class CloudGameSEIService;
   class CloudgameAudioUpstreamService;
   class CloudgameVideoUpstreamService;
   class CloudgameCameraService;
   class CloudgameStreamProfileService;
}

/**
 * @type api
 * 
 * @locale zh
 * 
 * @brief 云游戏场景会话
 */
class NATIVE_SDK_API CloudGameSession : public XSession {

public: // session自身相关API

    CloudGameSession() = default;

    virtual ~CloudGameSession() = default;

    /**
     * @type api
     * @brief 设置监听回调
	 * @locale zh
     * @note  建议在调用session start流程之前设置监听
     */
    virtual void setSessionListener(std::shared_ptr<CloudGameSessionListener> listener) = 0;

public: // 相关服务获取API

    /**
     * @type api
     * @brief 获取消息通道服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法
     * @return 消息通道服务；如果调用时状态不对或者当前版本不支持消息通道服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameMessageChannelService> getMessageChannelService() = 0;


    /**
     * @type api
     * @brief 获取鼠标操作服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法
     * @return 鼠标操作服务；如果调用时状态不对或者当前版本不支持鼠标操作服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameMouseService> getMouseService() = 0;

    /**
     * @type api
     * @brief 获取键盘操作服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法
     * @return 键盘操作服务；如果调用时状态不对或者当前版本不支持键盘操作服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameKeyboardService> getKeyboardService() = 0;


    /**
     * @type api
     * @brief 获取输入法操作服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法
     * @return 输入法操作服务；如果调用时状态不对或者当前版本不支持输入法操作服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameInputMethodService> getInputMethodService() = 0;

    /**
     * @type api
     * @brief 获取粘贴板操作服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法
     * @return 粘贴板操作服务；如果调用时状态不对或者当前版本不支持粘贴板操作服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameClipboardService> getClipboardService() = 0;

    /**
     * @type api
     * @brief 获取延迟测试服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法
     * @return 延迟探测服务服务；如果调用时状态不对或者当前版本不支持延迟探测服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameDelayDetectService> getDelayDetectService() = 0;

    /**
     * @type api
     * @brief 获取外部渲染服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
     *        CloudGameSessionConfig中没有设置外部渲染模式，则一定返回nullptr
     * 
     * @return 外部渲染服务实例；如果调用时状态不对或者当前版本不支持外部渲染服务实例，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudgameExternalRenderService> getExternalRenderService() = 0;

    /**
     * @type api
     * @brief 获取游戏手柄服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
     *
     * @return 游戏手柄服务；如果调用时状态不对或者当前版本不支持游戏手柄服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGamePadService> getGamepadService() = 0;

    /**
     * @type api
     * @brief 获取Pod管理服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
     *
     * @return 游戏Pod管理服务；如果调用时状态不对，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGamePodManagerService> getPodManagerService() = 0;

    /**
     * @type api
     * @brief 获取sei服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
     *
     * @return 游戏sei服务；如果调用时状态不对，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudGameSEIService> getSEIService() = 0;

    /**
     * @type api
     * @brief 获取上行音频服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
     *
     * @return 上行音频服务实例；如果调用时状态不对或者当前版本不支持上行音频服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudgameAudioUpstreamService> getAudioUpstreamService() = 0;

    /**
     * @type api
     * @brief 获取上行视频服务实例
	 * @locale zh
     * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
     *
     * @return 上行视频服务实例；如果调用时状态不对或者当前版本不支持上行视频服务，则会返回空指针。
     */
    virtual std::shared_ptr<vegame::CloudgameVideoUpstreamService> getVideoUpstreamService() = 0;

    /**
    * @type api
    * @brief 获取摄像头服务实例
	* @locale zh
    * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
    *
    * @return 摄像头服务实例；如果调用时状态不对或者当前版本不支持摄像头服务，则会返回空指针。
    */
    virtual std::shared_ptr<vegame::CloudgameCameraService> getCameraService() = 0;

    /**
    * @type api
    * @brief 获取stream profile服务实例
	* @locale zh
    * @note  建议CloudGameSessionListener#onStartSuccess回调执行后，调用此方法;
    *
    * @return stream profile服务实例；如果调用时状态不对或者当前版本不支持stream profile服务，则会返回空指针。
    */
    virtual std::shared_ptr<vegame::CloudgameStreamProfileService> getStreamProfileService() = 0;

};