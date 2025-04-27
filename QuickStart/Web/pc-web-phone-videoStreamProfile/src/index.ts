import { MODE } from "@volcengine/vegame";
import "./styles/index.css";
import {
  AndroidVideoStreamProfile,
  androidVideoStreamProfileOptions,
  PCVideoStreamProfile,
  pcVideoStreamProfileOptions,
} from "./constants";
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

// 和sdk无关
const initDom = async () => {
  // 设置清晰度选项
  const videoStreamProfileOptions =
    mode === MODE.CLOUD_PHONE_GAME
      ? androidVideoStreamProfileOptions
      : pcVideoStreamProfileOptions;
  const videoStreamProfileSelect = document.getElementById(
    "video-stream-profile-select"
  ) as HTMLSelectElement;

  // 清空现有选项
  videoStreamProfileSelect.innerHTML = "";

  // 添加新的选项
  videoStreamProfileOptions.forEach((option) => {
    const optionElement = document.createElement("option");
    optionElement.value = option.value.toString();
    optionElement.textContent = option.label;
    videoStreamProfileSelect.appendChild(optionElement);
  });

  // 初始化分辨率
  videoStreamProfileSelect.value =
    mode === MODE.CLOUD_PHONE_GAME
      ? AndroidVideoStreamProfile.SD.toString()
      : PCVideoStreamProfile.Default.toString();
};

// 启动成功后才可以调用sdk的方法
const startSuccessCallback = (gameController) => {
  document.getElementById("control-panel-detail")?.classList.remove("hidden");

  // 绑定分辨率切换事件
  const videoStreamProfileSelect = document.getElementById(
    "video-stream-profile-select"
  ) as HTMLSelectElement;
  videoStreamProfileSelect.addEventListener("change", (e) => {
    const videoStreamProfile = (e.target as HTMLSelectElement).value;
    gameController.sdkInstance.switchVideoStreamProfile(
      Number(videoStreamProfile)
    );
    console.log(`清晰度已切换为: ${videoStreamProfile}`);
  });
};

const stopSuccessCallback = () => {
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

initDom();
