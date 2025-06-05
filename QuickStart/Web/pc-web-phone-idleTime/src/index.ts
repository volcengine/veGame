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

  // 初始化空闲时长
  const idleTimeInput = document.getElementById(
    "idle-time"
  ) as HTMLInputElement;
  idleTimeInput.value = "300"; // 默认值为300秒

  // 绑定提交修改按钮事件
  const submitButton = document.getElementById("submit-idle-time");
  if (submitButton) {
    submitButton.addEventListener("click", async () => {
      const idleTime = idleTimeInput?.value?.trim();
      if (idleTime !== undefined && idleTime !== "") {
        // 具体接口逻辑可见：https://www.volcengine.com/docs/6512/75597#%E8%AE%BE%E7%BD%AE-%E8%8E%B7%E5%8F%96%E6%97%A0%E6%93%8D%E4%BD%9C%E5%9B%9E%E6%94%B6%E6%9C%8D%E5%8A%A1%E6%97%B6%E9%97%B4
        await gameController.sdkInstance.setIdleTime(Number(idleTime));
        console.log(`空闲时长已设置为: ${idleTime}秒`);
      } else {
        console.log("空闲时长输入框不存在有效值");
      }
    });
  }
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
