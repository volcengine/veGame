import { MODE } from "@volcengine/vegame";
import "./styles/index.css";
import GameController from "./core";

// 端游模式
const mode: MODE = MODE.CLOUD_PC;

// 动态设置容器样式，容器必须有宽高后续sdk初始化才会成功
const playerContainer = document.getElementById("player");
if (playerContainer) {
  if (mode === MODE.CLOUD_PC) {
    playerContainer.className = "pc-landscape";
  } else {
    playerContainer.className = "pc-portrait";
  }
}

// 初始化
const gameController = new GameController({
  initConfig: {
    mode,
    isPC: true, // 表示是pc端
    domId: "player", // 容器id
  },
  startConfig: {
    // 游戏的横竖屏，mode为CLOUD_PHONE_GAME时传portrait，mode为CLOUD_GAME时传landscape
    gameRotation: mode === MODE.CLOUD_PC ? "landscape" : "portrait",
  },
});

// 绑定UI事件
document
  .getElementById("start-btn")
  ?.addEventListener("click", () => gameController.start());
document
  .getElementById("stop-btn")
  ?.addEventListener("click", () => gameController.stop());

// 控制面板切换功能
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
