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
    sessionMode: 0, // 游戏挂机状态，0 为普通模式，1 为挂机模式
  },
});

const startSuccessCallback = (gameController) => {
  // 展示操作的UI
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  const gameIdleModeSelect = document.getElementById(
    "session-mod"
  ) as HTMLSelectElement;

  if (gameIdleModeSelect) {
    gameIdleModeSelect.addEventListener("change", async () => {
      const selectedMode = gameIdleModeSelect.value;
      await gameController.sdkInstance.setSessionMode(Number(selectedMode));
      console.log(
        `游戏挂机状态已设置为: ${
          selectedMode === "0" ? "普通模式" : "挂机模式"
        }`
      );
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
