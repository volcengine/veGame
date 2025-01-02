import veGameSDK from "@volcengine/vegame";
import $ from "jquery";

import "./scss/styles.scss";
import { initConfig, startConfig, isPC } from "./config";

console.log("isPc", isPC);
console.log("jquery version is %s", $.fn.jquery);

// 初始化 Demo ，跟 SDK 逻辑无关
const initApp = () => {
  if (isPC) {
    $("#player").addClass("pc-player pc-landscape");
  }

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
  return veGameSdkInstance;
}

function bindInstanceEvent(veGameSdkInstance) {
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
        rotation: isPC ? "landscape" : "portrait", // 以竖屏状态启动
        isScreenLock: !isPC, // 移动端是开启锁定屏幕横竖屏显示（即默认开启自动旋转功能，移动端时监听 orientation 进行画面旋转）
      });
      console.log("start success", startRes);
      // 隐藏启动云游戏按钮， 展示关闭云游戏按钮
      $("#start-btn").hide();
      $("#stop-btn").show();
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
    } catch (err) {
      // 关闭失败时，回滚状态
      isStart = true;
      console.log(err);
    }
  };
  console.log($("#start-btn"));
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
  // 初始化 Demo ，跟 SDK 逻辑无关
  initApp();
  // 初始化 SDK
  const veGameSdkInstance = initSdk();
  // 绑定 SDK 启动或关闭事件
  bindInstanceEvent(veGameSdkInstance);
})();
