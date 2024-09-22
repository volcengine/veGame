import "./scss/styles.scss";
import { Popover } from "bootstrap";
import veGameSDK from "@volcengine/vegame";
import { initConfig, startConfig } from "./config";
import { initTosFileChannel } from "./samples/file-channel";
import * as features from "./features";
import { isPC } from "./utils.js";
import "./libs/veGameVirtualInputSuite-Web_1.0.1.js";
import "./libs/veGamePadSDK_1.2.0.js";

console.log("isPc", isPC);

// Create an example popover
document.querySelectorAll('[data-bs-toggle="popover"]').forEach((popover) => {
  new Popover(popover);
});

console.log("jquery version is %s", $.fn.jquery);
console.log("features", features);

// 初始化 Demo ，跟 SDK 逻辑无关
const initApp = () => {
  if (isPC) {
    $("#player").addClass("pc-player pc-portrait");
  }

  $("#show-action-btn").draggable({ containment: document.body });

  $("#hide-action-btn").on("click", () => {
    $(".action-container").hide();
    $("#show-action-btn").show();
  });

  $("#show-action-btn").on("click", () => {
    $(".action-container").show();
    $("#show-action-btn").hide();
  });

  $(document).ready(function () {
    $(".dropdown-toggle").dropdown();
  });
};

function initSdk() {
  const veGameSdkInstance = new veGameSDK({
    ...initConfig,
    isDebug: import.meta.env.DEV, // 本地调试开启 debug 模式，线上关闭
    enableLocalKeyboard: false,
    enableSyncClipboard: true, //是否开启剪切板同步
    enableLocalMouseScroll: true, //是否开启本地鼠标滑轮滚动映射
  });
  window.veGameSdkInstance = veGameSdkInstance;
  console.log("veGameSDK version", veGameSdkInstance.getVersion());

  veGameSdkInstance.on("message-channel-connected", (message) => {
    console.log("message-channel-connected", message);
  });

  veGameSdkInstance.on("error", (error) => {
    console.log("error", error);
  });

  veGameSdkInstance.on("message", (message) => {
    console.log("message", message);
  });

  veGameSdkInstance.on("message-received", ({ msg }) => {
    console.log("message-received", msg);
    const { command } = msg;
    if (command === 8) {
      alert("游戏超时退出");
    }
  });

  // 如果启动时开启了排队服务，可以通过以下方式监听排队状态
  veGameSdkInstance.on("queue-update", ({ remainTime }) => {
    alert(`排队中，前面还有：${remainTime}个用户`);
  });
  veGameSdkInstance.on("queue-success", (data) => {
    alert("排队成功");
  });

  // 在页面 unmount 时检查云游戏是否处在运行状态，如果是正在运行，则调用 stop
  // 这样做的好处是：1. 释放 webrtc 链接，减少浏览器内存占用 2. 释放云游戏并发
  window.addEventListener("beforeunload", () => {
    /**
     * 这里采用 beforeunload 事件里调用 stop
     * 如果业务在 beforeunload 有业务逻辑，会调用 event.preventDefault
     * 则需要再 unload 事件里调用 stop
     */
    const connectionState = veGameSdkInstance.getConnectionState();
    // connectionState 是 CONNECTED 代表云游戏处在运行状态，此时需要 stop
    if (connectionState === "CONNECTED") {
      veGameSdkInstance?.stop();
      veGameSdkInstance.destroy();
    }
  });

  initTosFileChannel(veGameSdkInstance);
  return veGameSdkInstance;
}

const initFeatures = (veGameSdkInstance) => {
  const startSuccessCallbackList = [];
  const stopSuccessCallbackList = [];

  Object.keys(features)
    .map((featName) => features[featName](veGameSdkInstance))
    .forEach((result) => {
      if (result?.startSuccess) {
        startSuccessCallbackList.push(result?.startSuccess);
      }
      if (result?.stopSuccess) {
        stopSuccessCallbackList.push(result?.stopSuccess);
      }
    });

  return {
    startSuccess() {
      for (const fn of startSuccessCallbackList) {
        try {
          fn();
        } catch (err) {
          console.error("execute startSuccess callback error", err);
        }
      }
    },
    stopSuccess() {
      for (const fn of stopSuccessCallbackList) {
        try {
          fn();
        } catch (err) {
          console.error("execute stopSuccess callback error", err);
        }
      }
    },
  };
};

function bindEventListener(veGameSdkInstance, callback) {
  let isStart = false;
  const handleStart = async () => {
    console.log("start btn click");
    // 如果已经调用过 start，不再处理
    // sdk.start 是一个异步过程，实际业务中可以给启动云游戏按钮添加一个 loading 态，提高用户体验
    if (isStart) {
      return;
    }
    console.log("starting");
    try {
      isStart = true;
      // 接口文档：https://www.volcengine.com/docs/6512/75597
      const startRes = await veGameSdkInstance?.start({
        ...startConfig,
        rotation: "portrait", // 以竖屏状态启动
        isScreenLock: !isPC, // 移动端是开启锁定屏幕横竖屏显示（即默认开启自动旋转功能，移动端时监听 orientation 进行画面旋转）
        mute: true,
        audioAutoPlay: true, // mute & audioAutoPlay 都配置 true 是处理自动播放的策略，这里配置为 **初始化静音，用户首次点击时开启声音**
        /*
          设置游戏玩家排队功能：
            0：不设置排队功能
            1~99：队列优先级，数值越小优先级越高
        */
        queuePriority: 0,
        isDebug: import.meta.env.DEV, // 本地调试开启 debug 模式，可看到Pod打印日志，线上关闭
        // 根据业务需要，在申请云游戏服务时，可通过 extra 参数透传的自定义业务参数和相关说明如下：https://www.volcengine.com/docs/6512/334937
        extra: {
          /*
            是否开启在云端游戏中点击链接打开游览器的功能, 开启该功能，需要同时在云游戏管理后台启用该功能
          */
          enable_browser: "true",
          /*
            开启日志上传功能所需要的对象存储信息:
            注：开启日志上传至对象存储功能，需向云游戏技术支持提供火山引擎云游戏业务 ID（可通过云游戏控制台 业务配置 页面获取）
           */
          debug_info: {
            schema: "btd_tos", // 固定值，必填
            config: {
              ak: "your_ak",
              sk: "your_sk",
              bucket: "your_bucket",
              endpoint: "tos-cn-beijing.volces.com",
              region: "cn-beijing",
            },
            key: "test/adc.zip", // 仅支持zip格式
          },
          // 云游戏改机参数列表，具体配置可联系云游戏相关人员提供技术支持
          characteristics: JSON.stringify({
            manufacturer: "xxx", // 厂商
            brand: "xxx", // 品牌
          }),
          // 指定运行游戏需要的伴随程序列表, 已上传的伴随程序列表以及伴随程序包名可通过调用服务端 伴随程序管理 接口获取
          userFrameworkApp: JSON.stringify([
            "com.pkg1",
            "com.pkg2",
            "com.pkg3",
          ]),
        },
      });
      console.log("start success", startRes);

      // 隐藏启动云游戏按钮， 展示关闭云游戏按钮
      $("#start-btn").hide();
      $("#stop-btn").show();

      callback?.startSuccess?.();
    } catch (err) {
      // 启动失败时，回滚状态
      isStart = false;
      console.error("start error", err);
    }
  };
  const handleStop = async () => {
    console.log("stop btn click", isStart);
    // 如果没有启动，不处理
    // sdk.stop 是一个异步过程，实际业务中可以给关闭云游戏按钮添加一个 loading 态，提高用户体验
    if (!isStart) {
      return;
    }
    console.log("stopping");
    try {
      isStart = false;
      // 接口文档：https://www.volcengine.com/docs/6512/75597
      const stopRes = await veGameSdkInstance?.stop();
      console.log("stop success", stopRes);

      // 展示启动云游戏按钮， 隐藏关闭云游戏按钮
      $("#start-btn").show();
      $("#stop-btn").hide();

      callback?.stopSuccess?.();
    } catch (err) {
      // 关闭失败时，回滚状态
      isStart = true;
      console.log(err);
    }
  };
  $("#start-btn").on("click", handleStart);
  $("#stop-btn").on("click", handleStop);
}

(async () => {
  // 在启动云游戏之前，先检测用户的浏览器是否支持 rtc， 如果不支持，提示用户更换浏览器
  const isSupportRtc = await veGameSDK.isRtcSupported();
  if (!isSupportRtc) {
    console.log("当前浏览器不支持 WebRTC，请更换浏览器");
    return;
  }
  initApp();
  const veGameSdkInstance = initSdk();
  const callback = initFeatures(veGameSdkInstance);

  bindEventListener(veGameSdkInstance, callback);
})();
