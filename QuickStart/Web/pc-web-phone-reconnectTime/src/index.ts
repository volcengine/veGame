import { MODE } from "@volcengine/vegame";
import "./styles/index.css";
import GameController from "./core";

// 手游模式
const mode = MODE.CLOUD_PHONE_GAME;

// 动态设置容器样式，容器必须有宽高后续sdk初始化才会成功
const playerContainer = document.getElementById("player");
if (playerContainer) {
  // 手游容器默认是竖屏pc-portrait，端游容器是横屏pc-landscape
  if (mode === MODE.CLOUD_PHONE_GAME) {
    playerContainer.className = "pc-portrait";
  } else {
    playerContainer.className = "pc-landscape";
  }
}

// 初始化 sdk
const gameController = new GameController({
  initConfig: {
    mode,
    isPC: true, // 表示是pc端
    domId: "player", // 容器id
  },
  startConfig: {
    // 游戏的横竖屏，mode为CLOUD_PHONE_GAME时传portrait，mode为CLOUD_GAME时传landscape
    gameRotation: mode === MODE.CLOUD_PHONE_GAME ? "portrait" : "landscape",
  },
});

// 获取/设置自动回收时长：游戏启动后才可以调用实例方法
const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  // 初始化重连时长
  const reconnectTimeInput = document.getElementById(
    "reconnect-time"
  ) as HTMLInputElement;

  // 绑定提交修改按钮事件
  const submitButton = document.getElementById("submit-reconnect-time");
  if (submitButton) {
    submitButton.addEventListener("click", async () => {
      const reconnectTime = reconnectTimeInput?.value?.trim();
      if (reconnectTime !== undefined && reconnectTime !== "") {
        // 具体接口逻辑需要根据实际情况修改
        await gameController.sdkInstance.setReconnectTime(
          Number(reconnectTime)
        );
        console.log(`重连时长已设置为: ${reconnectTime}毫秒`);
      } else {
        console.log("重连时长输入框不存在有效值");
      }
    });
  }

  // reconnect回调事件
  // 游戏视频流开始重连的回调
  gameController.sdkInstance.on("stream-reconnecting", () => {
    console.log("重连中");
  });
  // 游戏视频流重连成功的回调
  gameController.sdkInstance.on("stream-reconnected", () => {
    console.log("重连成功");
  });
  // 游戏视频流重连成功的回调
  gameController.sdkInstance.on("error", ({ errorCode, errorMessage }) => {
    if (errorCode === 40010) {
      console.log("重连失败，超过指定时长", errorMessage);
    }
    console.log("重连失败：", errorMessage);
  });
};

const stopSuccessCallback = () => {
  // 隐藏操作的UI
  document.getElementById("control-panel-detail")?.classList.add("hidden");
};

// 绑定UI事件
document
  .getElementById("start-btn")
  ?.addEventListener("click", () =>
    gameController.start(() => startSuccessCallback(gameController))
  );
document
  .getElementById("stop-btn")
  ?.addEventListener("click", () =>
    gameController.stop(() => stopSuccessCallback())
  );

// 控制面板切换功能：与sdk无关
const togglePanel = document.getElementById("toggle-panel");
const controlPanel = document.getElementById("control-panel");

togglePanel?.addEventListener("click", () => {
  const isShowing = controlPanel?.classList.contains("show");
  controlPanel?.classList.toggle("show");
  controlPanel?.classList.toggle("hidden");
  if (togglePanel) {
    togglePanel.textContent = isShowing ? "隐藏控制面板" : "显示控制面板";
  }
});
