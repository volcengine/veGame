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
    isPC: false, // 表示是h5端
    domId: "player", // 容器id
    // enableLocalKeyboard: true, // 开启本地键盘
    useCustomLocalKeyboardInput: true, // 开启自定义 Input 模式
    customLocalKeyboardInputId: "custom-keyboard-input-id", // 自定义 Input 元素的 DOM ID（在使用本地输入法场景下，用于对输入框的 UI 和位置进行自定义）
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

  // 设置 本地键盘 Input 元素的 DOM ID
  const submitButton = document.getElementById("submit-custom-keyboard-id");
  if (submitButton) {
    submitButton.addEventListener("click", async () => {
      // SDK: 将本地键盘的隐藏或关闭状态同步到云端游戏
      gameController.sdkInstance.syncLocalKeyboardCloseStatus();
      console.log("同步本地键盘的隐藏或关闭状态同步到云端游戏成功");
      const inputContainer = document.getElementById(
        "custom-keyboard-input-id"
      ) as HTMLInputElement;
      inputContainer.style.display = "none";
    });
  }

  // SDK: 本地键盘打开事件：打开后可 调用 setCustomLocalKeyboardInputId 方法设置自定义本地键盘 Input 框的 DOM ID，设置自定义样式
  gameController.sdkInstance.on("local-keyboard-open", (localmessage) => {
    console.log("local-keyboard-open: ", localmessage);
    const inputContainer = document.getElementById(
      "custom-keyboard-input-id"
    ) as HTMLInputElement;
    inputContainer.style.display = "block";
    gameController.sdkInstance.setCustomLocalKeyboardInputId("customInputId");
  });

  // SDK: 本地键盘关闭事件：关闭后可 清除自定义本地键盘 Input 框的相关状态
  gameController.sdkInstance.on("local-keyboard-close", () => {
    console.log("local-keyboard-close");
    const inputContainer = document.getElementById(
      "custom-keyboard-input-id"
    ) as HTMLInputElement;
    inputContainer.style.display = "none";
    gameController.sdkInstance.clearCustomLocalKeyboardInput();
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
