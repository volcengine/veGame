#pragma once
#include <stdint.h>
#include <memory>
#include "ve_define.h"
#include "ve_cloud_defines.h"
namespace vecommon {

    /**
     * @type enum
	 * @locale zh
     * @brief 外部渲染视频帧编码格式
     */
    enum class FrameFormat {

        /**
         * @brief YUV I420 格式
         */
        YUVI420 = 1,

        /**
         * @brief YUY2 格式
         */
        YUY2,

        /**
         * @brief ARGB 格式, 字节序为 A8 R8 G8 B8
         */
        ARGB,

        /**
         * @brief RGBA 格式, 字节序为 R8 G8 B8 A8
         */
        RGBA,

        /**
         * @brief BGRA 格式, 字节序为 B8 G8 R8 A8
         */
        BGRA,

        UNKNOWN
    };

    /**
     * @type keytype
	 * @locale zh
     * @region 视频管理
     * @brief 视频YUV格式颜色空间
     */
    enum ColorSpace {
        /**
         * 未知的颜色空间，默认使用kColorSpaceYCbCrBT601LimitedRange颜色空间
         */
        kColorSpaceUnknown = 0,
        /**
         * @brief BT.601数字编码标准，颜色空间[16-235]
         */
        kColorSpaceYCbCrBT601LimitedRange,
        /**
         *  @brief BT.601数字编码标准，颜色空间[0-255]
         */
        kColorSpaceYCbCrBT601FullRange,
        /**
         * @brief BT.7091数字编码标准，颜色空间[16-235]
         */
        kColorSpaceYCbCrBT709LimitedRange,
        /**
         * @brief BT.7091数字编码标准，颜色空间[0-255]
         */
        kColorSpaceYCbCrBT709FullRange,
    };

    /**
     * @type type
	 * @locale zh
     * @brief 外部视频帧
     */
    class ExternalVideoFrame {
    public:

        /**
         *
         * @brief 析构函数
         *
         */
        virtual ~ExternalVideoFrame() = default;

        /**
         * @brief 获取视频帧时间戳，单位：微秒
         */
        virtual int64_t timestampUs() const = 0;

        /**
         * @brief 获取视频帧宽度，单位：px
         */
        virtual int width() const = 0;

        /**
         * @brief 获取视频帧高度，单位：px
         */
        virtual int height() const = 0;

        /**
         * @brief 视频帧颜色 plane 数量
         *
         * @notes RGBA、ARGB、BGRA 格式下返回 1     <br\>
         *        YUV 数据存储格式分为打包（packed）存储格式和平面（planar）存储格式 <br\>
         *        +   planar 格式中 Y、U、V 分平面存储    <br\>
         *        +   packed 格式中 Y、U、V 交叉存储
         */
        virtual int numberOfPlanes() const = 0;

        /**
         * @brief 获取 plane 数据指针
         * @param [in] planeIndex plane数据索引
         *
         * @note 获取ARGB、RGBA、BGRA格式数据，getPlaneData(0)   <br\>
         *   +    获取YUVI420格式数据，(Y)getPlaneData(0)  (U)getPlaneData(1) (V)getPlaneData(2)
         */
        virtual uint8_t* getPlaneData(int planeIndex) const = 0;

        /**
         * @brief 获取 plane 中数据行的长度(步幅)
         * @param [in] planeIndex plane 数据索引
         *
         * @note 获取ARGB、RGBA、BGRA格式数据，getPlaneStride(0)   <br>
         *   +   获取YUVI420格式数据Stride，(Y)getPlaneStride(0)  (U)getPlaneStride(1) (V)getPlaneStride(2)
         */
        virtual int getPlaneStride(int planeIndex) const = 0;

        /**
         * @brief 获取视频帧格式
         * 
         * @return 视频帧的具体格式，详见FrameFormat
         * 
         */
        virtual FrameFormat getVideoFrameFormat() const = 0;

        /**
         * @locale zh
         * @brief 获取视频帧颜色空间，参看 ColorSpace{@link #ColorSpace}
         */
        virtual ColorSpace colorSpace() const = 0;

        /**
         * @locale zh
         * @brief 获取视频帧旋转角度，参看 VideoRotation{@link #VideoRotation}
         */
        virtual VideoRotation rotation() const = 0;

        /**
         *
         * @brief 获取扩展数据指针
         * @param size 扩展数据字节数
         */
        virtual uint8_t* getExtraDataInfo(int& size) const = 0;

    };

    /**
       * @type api
	   * @locale zh
       *
       * @brief     创建外部视频帧。
       *
       * @param [in] width        : 视频帧宽度。
       * @param [in] height       : 视频帧高度。
       * @param [in] format       : 视频帧格式。
       * @param [in] extra_params : 额外的创建参数。
       *
       * @return nullptr表示设置失败;
       */
    NATIVE_SDK_API std::shared_ptr<ExternalVideoFrame> createExternalVideoFrame(
        int width,
        int height,
        vecommon::FrameFormat format,
        void* extra_params = nullptr
    );

    /**
      * @type api
	  * @locale zh
      *
      * @brief   更新外部音频帧
      *
      *
      * @return  发送结果；具体结果码说明：
      *						0  : 成功；
      *                     -1 : 无效参数导致的更新失败
      *          其他负数结果码: 其他原因导致的更新失败
      *
      */
    NATIVE_SDK_API int updateExternalVideoFrame(
        std::shared_ptr<ExternalVideoFrame> external_frame,
        const uint8_t* video_frame_data,
        const size_t& video_frame_data_len);
}